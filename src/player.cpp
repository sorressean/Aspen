#include <openssl/sha.h>
#include <cstdarg>
#include <cmath>
#include <tinyxml.h>
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
//config defaults:
    _config=new std::vector<Option*>();

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

    for (Option* oit:*_config)
        {
            delete oit;
        }
    if (_config)
        {
            delete _config;
            _config=nullptr;
        }

    delete _watchers;
}

BOOL Player::IsPlayer() const
{
    return true;
}
void Player::SerializeDoc(TiXmlDocument* doc)
{
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
    doc->LinkEndChild(decl);
    TiXmlElement* root = new TiXmlElement("player");
    Serialize(root);
    doc->LinkEndChild(root);
}
void Player::Serialize(TiXmlElement* root)
{
    OptionMeta* ometa = nullptr;
    std::string name;

//password info
    TiXmlElement* password = new TiXmlElement("password");
    password->SetAttribute("value", _password.c_str());
    password->SetAttribute("invalid", _invalidPassword);
    root->LinkEndChild(password);

//time info
    TiXmlElement* timeinfo = new TiXmlElement("timeinfo");
    timeinfo->SetAttribute("firstLogin", _firstLogin);
    timeinfo->SetAttribute("onlineTime", _onlineTime);
    timeinfo->SetAttribute("lastLogin", _lastLogin);
    root->LinkEndChild(timeinfo);

//options
    TiXmlElement* options = new TiXmlElement("options");
    if (_config->size())
        {
//we serialize if there is actually something to serialize.
            for (Option* opt:*_config)
                {
                    ometa = opt->GetMeta();
                    /*     if (ometa->GetValue() == opt->GetValue())
                             {
                                 continue;
                             }*/
                    TiXmlElement* option = new TiXmlElement("option");
                    name =/* OptionSectionToString(ometa->GetSection()) + "." +*/ ometa->GetName();
                    option->SetAttribute("name", name.c_str());
                    opt->GetValue().Serialize(option);
                    options->LinkEndChild(option);
                }
        }
    root->LinkEndChild(options);

    root->SetAttribute("title", _title.c_str());
    root->SetAttribute("prompt", _prompt.c_str());
    root->SetAttribute("rank", _rank);
    root->SetAttribute("pflag", _pflag);
    Living::Serialize(root);
    /*	delete option;
    	delete options;
    	delete timeinfo;
    	delete password;*/
}
void Player::Deserialize(TiXmlElement* root)
{
    World* world = World::GetPtr();
    OptionManager* omanager = world->GetOptionManager();
    Option* opt = nullptr;
    OptionMeta* ometa = nullptr;
    int tmp = 0;
    TiXmlElement* password = NULL;
    TiXmlElement* tinfo = NULL;
    TiXmlElement* option = NULL;
    TiXmlElement* options = NULL;
    TiXmlNode* node = NULL;
    Variant var;
    std::string name;

    if (!root)
        {
            throw(FileLoadException("Error loading file: player element was not found."));
        }

    node = root->FirstChild("password");
    if (!node)
        {
            throw(FileLoadException("Error loading file: password element was not found."));
        }
    password = node->ToElement();
    _password = password->Attribute("value");
    password->Attribute("invalid", &_invalidPassword);

    node = root->FirstChild("timeinfo");
    if (!node)
        {
            throw(FileLoadException("Could not find timeinfo element."));
        }
    tinfo = node->ToElement();
    tinfo->Attribute("firstLogin", &tmp);
    _firstLogin = tmp;
    tinfo->Attribute("onlineTime", &tmp);
    _onlineTime = tmp;
    tinfo->Attribute("lastLogin", &tmp);
    _lastLogin = tmp;

    node = root->FirstChild("options")->ToElement();
    if (!node)
        {
            throw(FileLoadException("Error: options node was not found."));
        }
    options = node->ToElement();
//now we iterate through the options list, and pull in the options to deserialize.
    for (node = options->FirstChild(); node; node = node->NextSibling())
        {
            option = node->ToElement();
            name = option->Attribute("name");
            ometa = omanager->GetOption(name);
            if (ometa)
                {
                    var.Deserialize(option->FirstChild("variable")->ToElement());
                    opt = new Option(ometa, var);
                    _config->push_back(opt);
                }
        }

    _title = root->Attribute("title");
    _prompt = root->Attribute("prompt");
    root->Attribute("rank", &_rank);
    root->Attribute("pflag", &_pflag);
    Living::Deserialize(root->FirstChild("living")->ToElement());
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
    std::stringstream st;
    int i = 0;
    unsigned char str[MAX_PASSWORD+1];
    memcpy(str, s.c_str(), s.length()+1);
    unsigned char password[SHA256_DIGEST_LENGTH+1];
    memset(password, 0, SHA256_DIGEST_LENGTH+1);
    SHA256(str, s.length(), password);
    for (i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        {
            st << std::hex << (int)password[i];
        }
    _password = st.str();
}

std::string Player::GetTempPassword() const
{
    return _tempPassword;
}
void Player::SetTempPassword(const std::string &s)
{
    std::stringstream st;
    int i = 0;
    unsigned char password[SHA256_DIGEST_LENGTH+1];
    unsigned char str[MAX_PASSWORD+1];
    memcpy(str, s.c_str(), s.length()+1);
    SHA256(str, s.length(), password);
    for (i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        {
            st << std::hex << (int)password[i];
        }
    _tempPassword = st.str();
}
void Player::ClearTempPassword()
{
    _tempPassword.clear();
}
BOOL Player::ComparePassword()
{
    return _password == _tempPassword;
}

void Player::IncInvalidPassword()
{
    _invalidPassword++;
    Save(true);
}

BOOL Player::Save(BOOL force)
{
    if (!force)
        {
            if ((time(NULL)-_lastSave) < SAVE_INTERVAL)
                {
                    return false;
                }

            _lastSave = time(NULL);
        }

    TiXmlDocument doc;
    SerializeDoc(&doc);
    doc.SaveFile((std::string(PLAYER_DIR)+GetName()).c_str());
    return true;
}
BOOL Player::Backup()
{
    if ((time(NULL)-_lastBackup) < BACKUP_INTERVAL)
        {
            return false;
        }

    TiXmlDocument doc;
    SerializeDoc(&doc);
    doc.SaveFile((std::string(BACKUP_DIR)+GetName()).c_str());
    return true;
}
void Player::Load()
{
    TiXmlDocument doc((std::string(PLAYER_DIR)+GetName()).c_str());
    if (!doc.LoadFile())
        {
            throw(FileLoadException("Error loading "+(std::string(PLAYER_DIR)+GetName())+"."));
        }
    TiXmlElement* root = doc.FirstChild("player")->ToElement();
    Deserialize(root);
}

void Player::EnterGame()
{
    EnterGame(false);
}
void Player::EnterGame(BOOL quiet)
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
    world->GetPlayerManager()->AddPlayer(this);
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
    world->GetPlayerManager()->RemovePlayer(this);
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
                    _config->push_back(opt);
                }
        }
}
Option* Player::GetOption(const std::string &option) const
{
    for (Option* opt:*_config)
        {
            if (opt->GetMeta()->GetName() == option)
                {
                    return opt;
                }
        }

    return nullptr;
}

BOOL Player::OptionExists(const std::string &option) const
{
    for (Option* opt: *_config)
        {
            if (opt->GetMeta()->GetName() == option)
                {
                    return true;
                }
        }

    return false;
}
BOOL Player::ToggleOption(const std::string &option)
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
                    _config->push_back(node);
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
std::vector<Option*>* Player::GetOptions() const
{
    return _config;
}

BOOL Player::HasAccess(FLAG access) const
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
    std::list<Player*>* players = world->GetPlayerManager()->GetPlayers();

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
