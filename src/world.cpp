#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <list>
#include <algorithm>
#include <unistd.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <tinyxml.h>
#include "world.h"
#include "channel.h"
#include "player.h"
#include "socket.h"
#include "command.h"
#include "component.h"
#include "componentMeta.hpp"
#include "ComponentFactory.h"
#include "utils.h"
#include "zone.h"
#include "option.h"
#include "optionManager.h"
#include "objectManager.h"
#include "serializer.h"
#include "event.h"
#include "delayedEvent.h"
#include "eventManager.h"
#include "calloutManager.h"
#include "com_gen.h"

World* World::_ptr;
World* World::GetPtr()
{
    if (!World::_ptr)
        {
            World::_ptr = new World();
        }
    return World::_ptr;
}

World::World()
{
    _running = true;
    _chanid=1;
    _pmanager = new PlayerManager();
    _server = nullptr;
    _motd = nullptr;
    _banner = nullptr;
    _updates = 0;
    _totalUpdateTime = 0;
    _totalSleepTime = 0;
    _commands = 0;
    _commandElapsed = 0;
    RegisterLog(EVENT_FILE, EVENT_NAME);
//events
    events.RegisterEvent("LivingPulse",new DelayedEvent(LIVING_PULSE,0));
    events.RegisterEvent("WorldPulse", new DelayedEvent(WORLD_PULSE, 0));
    events.RegisterEvent("PlayerConnect", new Event());
    events.RegisterEvent("PlayerDisconnect", new Event());
    events.RegisterEvent("PlayerCreated", new Event());
    events.RegisterEvent("PlayerDeleted", new Event());
    events.RegisterEvent("Shutdown", new Event());
    events.RegisterEvent("Copyover", new Event());
    events.RegisterEvent("ObjectLoaded", new Event());
    events.RegisterEvent("ObjectDestroyed", new Event());
}
World::~World()
{
    if (_pmanager)
        {
            delete _pmanager;
        }
    if (_motd)
        {
            delete [] _motd;
        }
    if (_banner)
        {
            delete [] _banner;
        }

    for (auto lit: _logs)
        {
            delete lit.second;
        }

    for (auto cit: _channels)
        {
            delete cit.second;
        }

    for (auto cit: _state)
        {
            delete cit.second;
        }

    for (Zone* zone: _zones)
        {
            delete zone;
        }

    delete _server;
}

void World::InitializeServer()
{
    _server=new Server();
}

void World::Shutdown()
{
    _pmanager->Shutdown();
    SaveState();
    events.CallEvent("Shutdown", NULL, static_cast<void*>(this));
    _running = false;
}

void World::Copyover(Player* mobile)
{
    std::list<Player*>* _users;
    int ruptime = (int)GetRealUptime();

    FILE* copyover = NULL;
    char buff[16];

    copyover=fopen(COPYOVER_FILE,"wb");
    if (copyover==NULL)
        {
            mobile->Message(MSG_ERROR,"couldn't open the copyover file.\nCopyover will not continue.");
            return;
        }

    fprintf(copyover, "%d\n", ruptime);
    sockaddr_in* addr=NULL;
//itterate through the players and write info to their copyover file:
    _users = _pmanager->GetPlayers();
    for (Player* person: *_users)
        {
            if (person->GetSocket()->GetConnectionType() != CON_Game)
                {
                    person->Write("We're sorry, but we are currently rebooting; please come back again soon.\n");
                    person->GetSocket()->Kill();
                    continue;
                }

            addr=person->GetSocket()->GetAddr();
            person->Save();
            fprintf(copyover,"%d %s %hu %lu %s\n",
                    person->GetSocket()->GetControl(), person->GetName().c_str(),
                    addr->sin_port,(long int)addr->sin_addr.s_addr, person->GetSocket()->GetHost().c_str());
            person->Write("Copyover initiated by "+mobile->GetName()+".\n");
        }

    fprintf(copyover,"-1\n");
    fclose(copyover);

    events.CallEvent("Copyover", NULL, static_cast<void*>(this));
    snprintf(buff,16,"%d",_server->GetListener());

    Update();
    SaveState();
    execl(BIN_FILE,BIN_FILE,"-c",buff,(char*)NULL);

    mobile->Write("Copyover failed!\n");
}

Server* World::GetServer() const
{
    return _server;
}

OlcManager* World::GetOlcManager()
{
    return &_olcs;
}

ComponentFactory* World::GetComponentFactory()
{
    return &_cfactory;
}

PlayerManager* World::GetPlayerManager()
{
    return _pmanager;
}

Log* World::GetLog(const std::string &name)
{
    if (LogExists(name))
        {
            return (_logs)[name];
        }
    else
        {
            return NULL;
        }
}
OptionManager* World::GetOptionManager()
{
    return &_options;
}

void World::GetChannelNames(std::list <std::string>* out)
{
    for (auto it: _channels)
        {
            out->push_back(it.second->GetName());
        }
}

BOOL World::ChannelExists(Channel* chan)
{

    for (auto it: _channels)
        {
            if (it.second == chan)
                {
                    return true;
                }
        }

    return false;
}
BOOL World::AddChannel(Channel* chan,BOOL command)
{
    OptionMeta* opt = nullptr;
    if (!ChannelExists(chan))
        {
            _channels[_chanid]=chan;
            opt = new OptionMeta();
            opt->SetName(chan->GetName());
            opt->SetHelp("Toggles the channel.");
            opt->SetToggle(true);
            opt->SetSection(OptionSection::Channel);
            opt->SetAccess(chan->GetAccess());
            if (chan->GetName() == "newbie")
                {
                    opt->SetValue(Variant(1));
                }
            else
                {
                    opt->SetValue(Variant(0));
                }
            _options.AddOption(opt);

            if (command)
                {
                    CMDChan* com = new CMDChan();
                    com->SetName(chan->GetName());
                    com->SetAccess(chan->GetAccess());
                    com->SetSubcmd(_chanid);
                    if (chan->GetAlias() != "")
                        {
                            com->AddAlias(chan->GetAlias());
                        }

                    commands.AddCommand(com);
                }

            _chanid++;
            return true;
        }

    return false;
}

Channel* World::FindChannel(int id)
{
    if (!_channels.count(id))
        {
            return NULL;
        }

    return _channels[id];
}

Channel* World::FindChannel(const std::string &name)
{
//This method is a bit slower because we have to iterate through the mapping ourselves.

    for (auto it: _channels)
        {
            if ((it.second)->GetName()==name)
                {
                    return (it.second);
                }
        }

    return NULL;
}

BOOL World::InitializeFiles()
{
    struct stat fs; //holds file stats
//load our banner:
//retrieve size of file so we can create the buffer:
    if(stat(LOGIN_FILE, &fs))
        {
            WriteLog("Could not stat login file.", CRIT);
            return false;
        }

    _banner=new char[fs.st_size+1];
    _banner[fs.st_size] = '\0';

//open and load the banner:
    FILE* banner_fd=fopen(LOGIN_FILE,"r");
    if (!banner_fd)
        {
            WriteLog("Could not fopen banner file.", CRIT);
            delete []_banner;
            _banner = NULL;
            return false;
        }
    if (fread(_banner,1, static_cast<size_t>(fs.st_size), banner_fd) != static_cast<size_t>(fs.st_size))
        {
            WriteLog("Error loading banner.", CRIT);
            delete []_banner;
            _banner = NULL;
            fclose(banner_fd);
            return false;
        }
    fclose(banner_fd);

//load our motd:
//retrieve size of file so we can create the buffer:
    if (stat(MOTD_FILE, &fs))
        {
            WriteLog("Could not stat MOTD file.", CRIT);
            delete []_banner;
            _banner = NULL;
            return false;
        }

    _motd=new char[fs.st_size+1];
    _motd[fs.st_size] = '\0';

    FILE* motd_fd=fopen(MOTD_FILE,"r");
    if (!motd_fd)
        {
            WriteLog("Could not fopen MOTD.", CRIT);
            delete [] _banner;
            delete [] _motd;
            _motd = _banner = NULL;
            return false;
        }

    if (fread(_motd,1,static_cast<size_t>(fs.st_size), motd_fd) != static_cast<size_t>(fs.st_size))
        {
            WriteLog("Error loading MOTD.", CRIT);
            delete [] _motd;
            _banner = NULL;
            fclose(motd_fd);
            return false;
        }
    fclose(motd_fd);
    WriteLog("Files loaded successfully");
    return true;
}

const char* World::GetBanner() const
{
    return _banner;
}
const char* World::GetMotd() const
{
    return _motd;
}

void World::Update()
{
    CalloutManager* callouts = CalloutManager::GetInstance();
    timeval start, end;
    gettimeofday(&start, NULL);
//checks for incoming connections or commands
    _server->PollSockets();
//flushes the output buffers of all sockets.
    _server->FlushSockets();
//update living objects:
    _pmanager->Update();
    for (auto zone: _zones)
        {
            zone->Update();
        }
    _objectManager.Update();
    callouts->Update();

    _updates ++;
    gettimeofday(&end, NULL);
    _totalUpdateTime += ((end.tv_sec - start.tv_sec) * 1000000);
    _totalUpdateTime += (end.tv_usec - start.tv_usec);

//sleep so that we don't kill our cpu
    _totalSleepTime += _server->Sleep(PULSES_PER_SECOND);
}

BOOL World::RegisterComponent(IComponentMeta* meta)
{
    return _cfactory.RegisterComponent(meta->GetName(), meta);
}
Component*  World::CreateComponent(const std::string &name)
{
    return _cfactory.Create(name);
}

time_t World::GetRealUptime() const
{
    return _ruptime;
}
void World::SetRealUptime(time_t tm)
{
    _ruptime=tm;
}

time_t World::GetCopyoverUptime() const
{
    return _cuptime;
}
void World::SetCopyoverUptime(time_t tm)
{
    _cuptime=tm;
}

BOOL World::AddProperty(const std::string &name,void* ptr)
{
    if (!_properties.count(name))
        {
            _properties[name]=ptr;
            return true;
        }

    return false;
}
void* World::GetProperty(const std::string &name)
{
    if (_properties.count(name))
        {
            return _properties[name];
        }

    return NULL;
}
BOOL World::RemoveProperty(const std::string &name)
{
    if (_properties.count(name))
        {
            _properties.erase(name);
            return true;
        }

    return false;
}

void World::ParseArguments(const std::string& args, int start, std::vector<std::string>& params)
{
    int i = start;
    const char* line = args.c_str();
    int len = strlen(line);

    // parse arguments
    for (; i < len; i++)
        {
            if (line[i] == ' ') continue;
            // is it a quoated argument
            if ((line[i] == '\'') || (line[i] == '"'))
                {
                    char match = line[i];
                    i++;
                    int arg_start = i;
                    // loop until we reach the closing character
                    for (; i < len; i++)
                        {
                            if (line[i] == match)
                                {
                                    break;
                                }
                        }
//push the quoted string.
                    params.push_back(args.substr(arg_start, i - arg_start));
                }

//no quoted string, get the entire argument until we see a space.
            if (isprint(line[i]))
                {
                    int arg_start = i;
                    for (; i < len; i++)
                        {
                            if ((line[i] == ' '))
                                {
                                    break;
                                }
                        }
                    params.push_back(args.substr(arg_start, i - arg_start));
                }
        }
}
BOOL World::DoCommand(Player* mobile,std::string args)
{
    timeval start, end; //measure execution time
    Room* location = nullptr;
    std::vector<Command*>* cptr = commands.GetPtr();
    std::string cmd = ""; // the parsed command name
    const char *line = args.c_str(); // the command line
    int len = strlen(line); // get length of string
    int i = 0; // counter
    std::vector<std::string> params; // the parameters being passed to the command
    //std::list<Command*>* externals; //external commands

//start measuring elapsed time.
    gettimeofday(&start, NULL);

//handle special commands.
    if (args[0] == '\"' || args[0] == '\'')
        {
            cmd="say";
            i = 1; //the arguments are just after the quote.
        }
    else if(args[0] == ':')
        {
            cmd="emote";
            i=1;
        }
    else
        {
            // parse command name
            for (i = 0; i < len; i++)
                {
                    if (line[i] == ' ') break;
                }
            // copy the command
            cmd = args.substr(0, i);
        }

    // are there any arguments to parse?
    if (i != len)
        {
            ParseArguments(args, i, params);
        }

//locate and execute the command:
//check the built-in commands first, then contents, then location.
    for (auto it: *cptr)
        {
            if ((it->GetName() == cmd)||(it->HasAlias(cmd, true)))
                {
                    if (!mobile->HasAccess(it->GetAccess()))
                        {
                            return false;
                        }

//execute command.
                    /*todo: add script command handling here.*/
                    it->Execute(it->GetName(), mobile, params, it->GetSubcmd());
                    gettimeofday(&end, NULL);
                    _commandElapsed += ((end.tv_sec - start.tv_sec) * 1000000);
                    _commandElapsed += (end.tv_usec-start.tv_usec);
                    _commands ++;
                    return true;
                }
        }
//todo: check inventory and room commands here.
    location = (Room*)mobile->GetLocation();
    if (location)
        {
            cptr = location->commands.GetPtr();
            for (auto it: *cptr)
                {
                    if ((it->GetName() == cmd)||(it->HasAlias(cmd, true)))
                        {
                            if (!mobile->HasAccess(it->GetAccess()))
                                {
                                    return false;
                                }
                            it->Execute(it->GetName(), mobile, params, it->GetSubcmd());
                            gettimeofday(&end, NULL);
                            _commandElapsed += ((end.tv_sec - start.tv_sec) * 1000000);
                            _commandElapsed += (float)(end.tv_usec-start.tv_usec);
                            _commands ++;
                            return true;
                        }
                }
        }
    return false;
}

BOOL World::AddZone(Zone* zone)
{

    if (_zones.size())
        {
            for (auto it:_zones)
                {
                    if (it == zone)
                        {
                            return false;
                        }
                }
        }

    _zones.push_back(zone);
    return true;
}
BOOL World::RemoveZone(Zone* zone)
{
    std::vector<Zone*>::iterator it, itEnd;

    itEnd = _zones.end();
    for (it = _zones.begin(); it != itEnd; ++it)
        {
            if (*it ==zone)
                {
                    _zones.erase(it);
                    return true;
                }
        }

    return false;
}
Zone* World::GetZone(const std::string &name)
{

    for (auto it: _zones)
        {
            if (name==it->GetName())
                {
                    return it;
                }
        }

    return NULL;
}
BOOL World::GetZones(std::vector<Zone*> *zones)
{
    std::copy(_zones.begin(), _zones.end(), std::back_inserter(*zones));
    return true;
}

BOOL World::LogExists(const std::string &name)
{
    if (_logs.count(name))
        {
            return true;
        }

    return false;
}
BOOL World::RegisterLog(const std::string &path, const std::string &name)
{
    if (LogExists(name))
        {
            return false;
        }
    else
        {
            Log* log = new Log(path);
            if (log)
                {
                    _logs[name] = log;
                    return true;
                }
        }

    return false;
}
void World::WriteLog(const std::string &data, LOG_LEVEL l, const std::string &name)
{
    if (LogExists(name))
        {
            GetLog(name)->Write(data, l);
        }
}
BOOL World::IsRunning() const
{
    return _running;
}
void World::SetRunning(BOOL running)
{
    _running = running;
}

BOOL World::PromptExists(char prompt)
{
    return (_prompts.count(prompt)==0? false:true);
}
BOOL World::RegisterPrompt(char c, PROMPTCB callback)
{
    if (PromptExists(c))
        {
            return false;
        }

    _prompts[c] = callback;
    return true;
}
std::string World::BuildPrompt(const std::string &prompt, Player* mobile)
{
    std::string::const_iterator it, itEnd;
    std::string ret;

    itEnd = prompt.end();
    for (it = prompt.begin(); it != itEnd; ++it)
        {
            if ((*it) == '%' && ++it != itEnd)
                {
                    if (PromptExists((*it)))
                        {
                            ret += (_prompts[(*it)])(mobile);
                        }
                    else
                        {
                            ret += '%';

                            ret+=(*it);
                        }
                }
            else
                {
                    ret += (*it);
                }
        }

    return ret;
}

BOOL World::AddState(const std::string &name, ISerializable* s)
{
    if (StateExists(name))
        {
            return false;
        }

    _state[name] = s;
    return true;
}
BOOL World::RemoveState(const std::string &name)
{
    if (!StateExists(name))
        {
            return false;
        }

    _state.erase(name);
    return true;
}
BOOL World::StateExists(const std::string &name)
{
    return (_state.count(name)==1?true:false);
}

BOOL World::SaveState()
{
    std::map<std::string, ISerializable*>::iterator sit, sitEnd;
    TiXmlElement* element = NULL;
    sitEnd = _state.end();
    for (sit = _state.begin(); sit != sitEnd; ++sit)
        {
            TiXmlDocument doc;
            TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
            doc.LinkEndChild(decl);
            element = new TiXmlElement("state");
            element->SetAttribute("name", (*sit).first.c_str());
            (*sit).second->Serialize(element);
            doc.LinkEndChild(element);
            doc.SaveFile((std::string(STATE_DIR)+std::string((*sit).first)).c_str());
            element = NULL;
        }
    return true;
}
BOOL World::LoadState()
{
    DIR* statedir = opendir(STATE_DIR);
    dirent* dir = NULL;

//we need to open the directory for reading.
    if (!statedir)
        {
            return false;
        }

    while ((dir = readdir(statedir)))
        {
            if (dir->d_name[0] == '.')
                {
                    continue;
                }
            TiXmlDocument doc((std::string(STATE_DIR)+std::string(dir->d_name)).c_str());
            std::string name;
            if (!doc.LoadFile())
                {
                    WriteLog("Could not load"+std::string(dir->d_name)+" state file.", WARN);
                    closedir(statedir);
                    return false;
                }

            TiXmlElement* root = doc.FirstChild("state")->ToElement();
            name = root->Attribute("name");
            if (!StateExists(name))
                {
                    WriteLog("Could not find a matching registered state for "+name+" in the state register. This state will not be deserialized.",
                             WARN);
                    continue;
                }
            else
                {
                    _state[name]->Deserialize(root);
                }
        }

    closedir(statedir);
    return true;
}

unsigned long long int World::GetUpdates() const
{
    return _updates;
}
unsigned long long int World::GetUpdateTime() const
{
    return _totalUpdateTime;
}
unsigned long long int World::GetSleepTime() const
{
    return _totalSleepTime;
}

unsigned long long int World::GetCommands() const
{
    return _commands;
}
unsigned long long int World::GetCommandTime() const
{
    return _commandElapsed;
}

ObjectManager* World::GetObjectManager()
{
    return &_objectManager;
}
