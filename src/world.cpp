#include <tinyxml2.h>
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
#include "log.h"
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
    _server = nullptr;
    _motd = nullptr;
    _banner = nullptr;
    _updates = 0;
    _totalUpdateTime = 0;
    _totalSleepTime = 0;
    _commands = 0;
    _commandElapsed = 0;
//events
    events.RegisterEvent("LivingPulse");
    events.RegisterEvent("WorldPulse");
    events.RegisterEvent("PlayerConnect");
    events.RegisterEvent("PlayerDisconnect");
    events.RegisterEvent("PlayerCreated");
    events.RegisterEvent("PlayerDeleted");
    events.RegisterEvent("Shutdown");
    events.RegisterEvent("Copyover");
    events.RegisterEvent("ObjectLoaded");
    events.RegisterEvent("ObjectDestroyed");
}
World::~World()
{
    if (_motd)
        {
            delete [] _motd;
        }
    if (_banner)
        {
            delete [] _banner;
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
    _pmanager.Shutdown();
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
    _users = _pmanager.GetPlayers();
    for (auto person: *_users)
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

PlayerManager& World::GetPlayerManager()
{
    return _pmanager;
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

bool World::ChannelExists(Channel* chan)
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
bool World::AddChannel(Channel* chan,bool command)
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

bool World::InitializeFiles()
{
    struct stat fs; //holds file stats
//load our banner:
//retrieve size of file so we can create the buffer:
    if(stat(LOGIN_FILE, &fs))
        {
            WriteLog(SeverityLevel::Fatal, "Could not stat login file.");
            return false;
        }

    _banner=new char[fs.st_size+1];
    _banner[fs.st_size] = '\0';

//open and load the banner:
    FILE* banner_fd=fopen(LOGIN_FILE,"r");
    if (!banner_fd)
        {
            WriteLog(SeverityLevel::Fatal, "Could not fopen banner file.");
            delete []_banner;
            _banner = NULL;
            return false;
        }
    if (fread(_banner,1, static_cast<size_t>(fs.st_size), banner_fd) != static_cast<size_t>(fs.st_size))
        {
            WriteLog("SeverityLevel::Fatal, Error loading banner.");
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
            WriteLog(SeverityLevel::Fatal, "Could not stat MOTD file.");
            delete []_banner;
            _banner = NULL;
            return false;
        }

    _motd=new char[fs.st_size+1];
    _motd[fs.st_size] = '\0';

    FILE* motd_fd=fopen(MOTD_FILE,"r");
    if (!motd_fd)
        {
            WriteLog(SeverityLevel::Fatal, "Could not fopen MOTD.");
            delete [] _banner;
            delete [] _motd;
            _motd = _banner = NULL;
            return false;
        }

    if (fread(_motd,1,static_cast<size_t>(fs.st_size), motd_fd) != static_cast<size_t>(fs.st_size))
        {
            WriteLog(SeverityLevel::Fatal, "Error loading MOTD.");
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
    _pmanager.Update();
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

bool World::RegisterComponent(IComponentMeta* meta)
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

bool World::AddProperty(const std::string &name,void* ptr)
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
bool World::RemoveProperty(const std::string &name)
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
bool World::DoCommand(Player* mobile,std::string args)
{
    timeval start, end; //measure execution time
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
    /*
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
    */
    return false;
}

bool World::AddZone(Zone* zone)
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
bool World::RemoveZone(Zone* zone)
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
bool World::GetZones(std::vector<Zone*> *zones)
{
    std::copy(_zones.begin(), _zones.end(), std::back_inserter(*zones));
    return true;
}

bool World::IsRunning() const
{
    return _running;
}
void World::SetRunning(bool running)
{
    _running = running;
}

bool World::PromptExists(char prompt)
{
    return (_prompts.count(prompt)==0? false:true);
}
bool World::RegisterPrompt(char c, PROMPTCB callback)
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

bool World::AddState(const std::string &name, ISerializable* s)
{
    if (StateExists(name))
        {
            return false;
        }

    _state[name] = s;
    return true;
}
bool World::RemoveState(const std::string &name)
{
    if (!StateExists(name))
        {
            return false;
        }

    _state.erase(name);
    return true;
}
bool World::StateExists(const std::string &name)
{
    return (_state.count(name)==1?true:false);
}

bool World::SaveState()
{
    tinyxml2::XMLElement* element = nullptr;

    for (auto sit: _state)
        {
            tinyxml2::XMLDocument doc;
            doc.InsertEndChild(doc.NewDeclaration());
            element = doc.NewElement("state");
            element->SetAttribute("name", sit.first.c_str());
            sit.second->Serialize(element);
            doc.InsertEndChild(element);
            doc.SaveFile((STATE_DIR+sit.first).c_str());
            element = nullptr;
        }

    return true;
}
bool World::LoadState()
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

            tinyxml2::XMLDocument doc;
            std::string name;
            if (doc.LoadFile((std::string(STATE_DIR)+dir->d_name).c_str()) != tinyxml2::XML_NO_ERROR)
                {
                    WriteLog(SeverityLevel::Warning, "Could not load"+std::string(dir->d_name)+" state file.");
                    closedir(statedir);
                    return false;
                }

            tinyxml2::XMLElement* root = doc.FirstChildElement("state")->ToElement();
            name = root->Attribute("name");
            if (!StateExists(name))
                {
                    WriteLog(SeverityLevel::Warning, "Could not find a matching registered state for "+name+" in the state register. This state will not be deserialized.");
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
