#include <tinyxml2.h>
#include <cstdarg>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <cstring>
#include <sstream>
#include <functional>
#include "mud.h"
#include "conf.h"
#include "player.h"
#include "event.h"
#include "eventargs.h"
#include "utils.h"
#include "world.h"
#include "objectManager.h"
#include "exception.h"
#include "olc.h"
#include "calloutManager.h"
#include "option.h"
#include "optionMeta.h"
#include "optionManager.h"

Player::Player()
{
    _invalidPassword=0;
    _prompt=">";
    _title="the brave";
    _rank = RANK_PLAYER;
    _pflag = 0;
    _firstLogin=0;
    _onlineTime=0;
    _lastLogin=0;
    _lastSave = 0;
    _lastBackup = 0;
    _watching = NULL;
    _watchers = new std::list<Player*>();

//messages:
    _messages=new std::map<MessageType,std::string>();
    AddMessage(MSG_ERROR, C_RED);
    AddMessage(MSG_INFO, C_BLUE);
    AddMessage(MSG_CRITICAL, CB_CYAN);
    AddMessage(MSG_CHANNEL, C_YELLOW);
    AddMessage(MSG_LIST, C_CYAN);

//events
    events.RegisterEvent("EnterGame", new Event());
    events.RegisterEvent("LeaveGame", new Event());
    events.RegisterEvent("OptionChanged", new Event());
}
Player::~Player()
{
    if (_messages)
        {
            delete _messages;
            _messages=NULL;
        }

    for (Option* oit: _config)
        {
            delete oit;
        }
    delete _watchers;
}

bool Player::IsPlayer() const
{
    return true;
}
void Player::Serialize(tinyxml2::XMLElement* root)
{
    using tinyxml2::XMLElement;

    tinyxml2::XMLDocument* doc = root->GetDocument();
    std::string name;

//password info
    XMLElement* password = doc->NewElement("password");
    password->SetAttribute("value", _password.c_str());
    password->SetAttribute("invalid", _invalidPassword);
    root->InsertEndChild(password);

//time info
    XMLElement* timeinfo = doc->NewElement("timeinfo");
    timeinfo->SetAttribute("firstLogin", (unsigned int)_firstLogin);
    timeinfo->SetAttribute("onlineTime", (unsigned int)_onlineTime);
    timeinfo->SetAttribute("lastLogin", (unsigned int)_lastLogin);
    root->InsertEndChild(timeinfo);

    SerializeCollection<std::vector<Option*>, Option*>("options", "option", root, _config, [](tinyxml2::XMLElement* element, Option* opt)
    {
        OptionMeta* ometa = opt->GetMeta();
        std::string name = ometa->GetName();
        element->SetAttribute("name", name.c_str());
        opt->GetValue().Serialize(element);
    });

    root->SetAttribute("title", _title.c_str());
    root->SetAttribute("prompt", _prompt.c_str());
    root->SetAttribute("rank", _rank);
    root->SetAttribute("pflag", _pflag);
    Living::Serialize(root);
}
void Player::Deserialize(tinyxml2::XMLElement* root)
{
    using tinyxml2::XMLElement;

    XMLElement* ent = nullptr;
    World* world = World::GetPtr();
    OptionManager* omanager = world->GetOptionManager();
    Option* opt = nullptr;
    OptionMeta* ometa = nullptr;
    Variant var;
    std::string name;

    if (!root)
        {
            throw(FileLoadException("Error loading file: player element was not found."));
        }

    ent = root->FirstChildElement("password");
    if (!ent)
        {
            throw(FileLoadException("Error loading file: password element was not found."));
        }
    _password = ent->Attribute("value");
    _invalidPassword = ent->IntAttribute("invalid");

    ent = root->FirstChildElement("timeinfo");
    if (!ent)
        {
            throw(FileLoadException("Could not find timeinfo element."));
        }
    _firstLogin = ent->IntAttribute("firstLogin");
    _onlineTime = ent->IntAttribute("onlineTime");
    _lastLogin =ent->IntAttribute("lastLogin");

    DeserializeCollection(root, "options", [=](XMLElement* cur) mutable
    {
        name = cur->Attribute("name");
        ometa = omanager->GetOption(name);
        if (ometa)
            {
                var.Deserialize(cur->FirstChildElement("variable"));
                opt = new Option(ometa, var);
                _config.push_back(opt);
            }
    });

    _title = root->Attribute("title");
    _prompt = root->Attribute("prompt");
    _rank = root->IntAttribute("rank");
    _pflag = root->IntAttribute("pflag");
    Living::Deserialize(root->FirstChildElement("living"));
}

void Player::SetSocket(Socket* sock)
{
    _sock = sock;
}
std::string Player::GetShort() const
{
    std::string ret = GetName() + " " + GetTitle();
    ret += ".";
    return ret;
}

std::string Player::GetPassword() const
{
    return _password;
}
void Player::SetPassword(const std::string &s)
{
    _password=Sha256Hash(s);
}

std::string Player::GetTempPassword() const
{
    return _tempPassword;
}
void Player::SetTempPassword(const std::string &s)
{
    _tempPassword = Sha256Hash(s);
}
void Player::ClearTempPassword()
{
    _tempPassword.clear();
}
bool Player::ComparePassword()
{
    return _password == _tempPassword;
}

void Player::IncInvalidPassword()
{
    _invalidPassword++;
    Save(true);
}

bool Player::Save(bool force)
{
    if (!force)
        {
            if ((time(NULL)-_lastSave) < SAVE_INTERVAL)
                {
                    return false;
                }
            _lastSave = time(NULL);
        }

    tinyxml2::XMLDocument doc;
    doc.InsertEndChild(doc.NewDeclaration());
    tinyxml2::XMLElement* root = doc.NewElement("player");
    Serialize(root);
    doc.InsertEndChild(root);
    doc.SaveFile((PLAYER_DIR + GetName()).c_str());
    return true;
}
bool Player::Backup()
{
    if ((time(NULL)-_lastBackup) < BACKUP_INTERVAL)
        {
            return false;
        }
    /**
    * @todo update to make save take a backup param.
    */
    return true;
}
void Player::Load()
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile((std::string(PLAYER_DIR)+GetName()).c_str()) != tinyxml2::XML_NO_ERROR)
        {
            throw(FileLoadException("Error loading "+(std::string(PLAYER_DIR)+GetName())+"."));
        }
    tinyxml2::XMLElement* root = doc.FirstChildElement("player");
    Deserialize(root);
}

void Player::EnterGame()
{
    EnterGame(false);
}
void Player::EnterGame(bool quiet)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    ObjectContainer* location = nullptr;
    location = GetLocation();
    if (location == nullptr)
        {
            MoveTo(omanager->GetRoom(ROOM_START));
            location = GetLocation();
        }
    else
        {
            location->ObjectEnter(this);
        }

    Living::EnterGame();
//add the player to the users list:
    world->GetPlayerManager().AddPlayer(this);
//if there were password attempts, tell the player.
    if (_invalidPassword)
        {
            std::stringstream st;
            st << (char)7 << _invalidPassword << (_invalidPassword==1?" attempt was":" attempts were") << " made on your password.";
            Message(MSG_CRITICAL,st.str());
            _invalidPassword=0;
        }
    if (!quiet)
        {
//show the login banner:
            Write("\n"+std::string(world->GetMotd())+"\n");
        }
    world->events.CallEvent("PlayerConnect", NULL,this);
    events.CallEvent("EnterGame", NULL, this);
    Attach();
    Save();

    if (location->IsRoom())
        {
            Message(MSG_INFO, ((Room*)location)->DoLook(this));
        }
}
void Player::LeaveGame()
{
    World* world = World::GetPtr();

    Save(true);
    Living::LeaveGame();
//take the player from the users list:
    world->GetPlayerManager().RemovePlayer(this);
    world->events.CallEvent("PlayerDisconnect", NULL, this);
    events.CallEvent("LeaveGame", NULL,this);
}

void Player::Write(const std::string &text) const
{
    if (_sock)
        _sock->Write(text);
}

void Player::Message(const MessageType type,const std::string &data) const
{
    Write((*_messages)[type]+data+C_NORMAL+"\n");
}

Socket* Player::GetSocket()
{
    return _sock;
}

std::string Player::GetTitle() const
{
    return _title;
}
void Player::SetTitle(const std::string &s)
{
    _title=s;
}

FLAG Player::GetRank() const
{
    return _rank;
}
void Player::SetRank(const FLAG s)
{
    _rank=s;
}
FLAG Player::GetPflag() const
{
    return _pflag;
}
void Player::SetPflag(FLAG flag)
{
    _pflag = flag;
}
UINT Player::GetOnlineTime() const
{
    return (UINT)_onlineTime;
}
void Player::SetOnlineTime(UINT s)
{
    _onlineTime=(time_t)s;
}

UINT Player::GetFirstLogin() const
{
    return (UINT)_firstLogin;
}
void Player::SetFirstLogin(UINT first)
{
    _firstLogin=(time_t)first;
}

UINT Player::GetLastLogin() const
{
    return (time_t) _lastLogin;
}
void Player::SetLastLogin(UINT last)
{
    _lastLogin=(time_t)last;
}

std::string Player::GetPrompt() const
{
    return _prompt;
}
void Player::SetPrompt(const std::string &prompt)
{
    _prompt=prompt;
}

void Player::SetOption(const std::string &option, Variant &val)
{
    Option* opt = nullptr;
    OptionMeta* ometa = nullptr;

    opt = GetOption(option);
    if (OptionExists(option) && opt)
        {
            if (opt->GetMeta()->GetValue().Typeof() == val.Typeof())
                {
                    opt->GetValue().SetStr(val.GetStr());
                }
        }
    else
        {
//checks for the global existance of the option.
            ometa = opt->GetMeta();
            if (ometa->GetValue().Typeof() == val.Typeof())
                {
                    opt = new Option(ometa, val);
                    _config.push_back(opt);
                }
        }
}
Option* Player::GetOption(const std::string &option) const
{
    for (Option* opt: _config)
        {
            if (opt->GetMeta()->GetName() == option)
                {
                    return opt;
                }
        }

    return nullptr;
}

bool Player::OptionExists(const std::string &option) const
{
    for (Option* opt: _config)
        {
            if (opt->GetMeta()->GetName() == option)
                {
                    return true;
                }
        }

    return false;
}
bool Player::ToggleOption(const std::string &option)
{
    int temp = 0;
    World* world = World::GetPtr();
    OptionManager* omanager = world->GetOptionManager();
    OptionMeta* ometa = nullptr;
    Option* node = nullptr;

    if ((omanager->OptionExists(option)) && (!OptionExists(option)))
        {
            ometa = omanager->GetOption(option);
            if (ometa->GetValue().Typeof() == VAR_INT)
                {
                    if (ometa->GetValue().GetInt())
                        {
                            temp = 0;
                        }
                    else
                        {
                            temp = 1;
                        }
                    node = new Option(ometa, Variant(temp));
                    _config.push_back(node);
                    OptionChangedArgs arg(node);
                    events.CallEvent("OptionChanged", &arg, this);
                }
            return true;
        }
    node = GetOption(option);
    if (node)
        {
            if (node->GetValue().Typeof() == VAR_INT)
                {
                    if (node->GetValue().GetInt())
                        {
                            node->SetValue(Variant(0));
                        }
                    else
                        {
                            node->SetValue(Variant(1));
                        }
                    OptionChangedArgs arg(node);
                    events.CallEvent("OptionChanged", &arg, this);
                }
            return true;
        }

    return false;
}
std::vector<Option*>* Player::GetOptions()
{
    return &_config;
}

bool Player::HasAccess(FLAG access) const
{
    return BitIsSet(_rank,access);
}
void Player::AddMessage(MessageType type, const std::string &color)
{
    (*_messages)[type]=color;
}

void InitializePlayer()
{
    CalloutManager* manager = CalloutManager::GetInstance();
    World* world = World::GetPtr();
    std::list<Player*>* players = world->GetPlayerManager().GetPlayers();

    manager->RegisterCallout(60*30, 0, [players] (Callout* foo)
    {
        for(auto person:*players)
            {
                person->Save(true);
            }
    }, false);
    manager->RegisterCallout(3, 0, [players](Callout* callout)
    {
        for (auto person: *players)
            {
                person->SetOnlineTime(person->GetOnlineTime() + 3);
            }
    }, false);
}
