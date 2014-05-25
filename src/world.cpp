#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <list>
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
#include "componentMeta.h"
#include "ComponentFactory.h"
#include "utils.h"
#include "zone.h"
#include "option.h"
#include "optionManager.h"
#include "serializer.h"
#include "event.h"
#include "delayedEvent.h"
#include "eventManager.h"
#include "calloutManager.h"
#include "com_gen.h"
#include "staticObject.h"

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
    _logs = new std::map<std::string, Log*>();
    RegisterLog(EVENT_FILE, EVENT_NAME);
    _users=new std::list<Player*>();
    _prompts = new std::unordered_map<char, PROMPTCB>();
    _channels=new std::map<int,Channel*>();
    _options = new OptionManager();
    _cfactory=new ComponentFactory();
    _properties=new std::map<std::string,void*>();
    _zones=new std::vector<Zone*>();
    _objects=new std::unordered_map<VNUM,StaticObject*>();
    _rooms = new std::unordered_map<VNUM, Room*>();
    _npcs = new std::unordered_map<VNUM, Npc*>();
    _olcs = new OlcManager();
    _state = new std::map<std::string, ISerializable*>();
    _chanid=1;
    _server = NULL;
    _updates = 0;
    _totalUpdateTime = 0;
    _totalSleepTime = 0;
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
    if (_motd)
        {
            delete [] _motd;
        }
    if (_banner)
        {
            delete [] _banner;
        }
    if (_server)
        {
            delete _server;
        }
    delete _users;

    for (auto lit: *_logs)
        {
            delete lit.second;
        }
    delete _logs;

    for (auto cit: *_channels)
        {
            delete cit.second;
        }
    delete _channels;

    for (auto cit: *_state)
        {
            delete cit.second;
        }
    delete _state;

    for (Zone* zone: *_zones)
        {
            delete zone;
        }
    delete _zones;

    delete _options;
    delete _cfactory;
    delete _properties;
    delete _prompts;
    delete _rooms;
    delete _npcs;
    delete _objects;
    delete _olcs;
}

void World::InitializeServer()
{
    _server=new Server();
}

void World::Shutdown()
{
    std::list <Player*>::iterator it, itEnd;
    std::list<Player*> players(_users->begin(), _users->end());

    Player* person = NULL;

    itEnd = players.end();
    for (it = players.begin(); it != itEnd; ++it)
        {
            person = (*it);
            person->Message(MSG_CRITICAL,"The mud is shutting down now. Your Character will be autosaved.");
            person->GetSocket()->Kill();
        }
    _running = false;
    SaveState();
    events.CallEvent("Shutdown", NULL, static_cast<void*>(this));
}
void World::Copyover(Player* mobile)
{
    int cuptime = (int)time(NULL);
    int ruptime = (int)GetRealUptime();
    FILE* copyover = NULL;
    char buff[16];

    copyover=fopen(COPYOVER_FILE,"wb");
    if (copyover==NULL)
        {
            mobile->Message(MSG_ERROR,"couldn't open the copyover file.\nCopyover will not continue.");
            return;
        }

    fprintf(copyover, "%d %d\n", cuptime, ruptime);
    sockaddr_in* addr=NULL;
//itterate through the players and write info to their copyover file:

    for (Player* person: *_users)
        {
            if (person->GetSocket()->GetConnectionType() != ConnectionType::Game)
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
    memset(buff,0,16);
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

OlcManager* World::GetOlcManager() const
{
    return _olcs;
}

ComponentFactory* World::GetComponentFactory()
{
    return _cfactory;
}

Log* World::GetLog(const std::string &name)
{
    if (LogExists(name))
        {
            return (*_logs)[name];
        }
    else
        {
            return NULL;
        }
}
OptionManager* World::GetOptionManager()
{
    return _options;
}
std::list <Player*> *World::GetPlayers() const
{
    return _users;
}

BOOL World::AddPlayer(Player* player)
{
    if (!player)
        {
            return false;
        }
    if (player->GetSocket()->GetConnectionType() != ConnectionType::Game)
        {
            return false;
        }

    _users->push_back(player);
    return true;
}
BOOL World::RemovePlayer(Player* player)
{
    std::list<Player*>::iterator it, itEnd;


    if (_users->size())
        {
            itEnd = _users->end();
            for (it = _users->begin(); it != itEnd; ++it)
                {
                    if ((*it) == player)
                        {
                            _users->erase(it);
                            return true;
                        }
                }
        }

    return false;
}
Player* World::FindPlayer(const std::string &name) const
{
    std::list<Player*>::iterator it, itEnd;

    if (_users->size())
        {
            itEnd=_users->end();
            for (it = _users->begin(); it != itEnd; ++it)
                {
                    if ((*it)->GetName()==name)
                        {
                            return (*it);
                        }
                }
        }

    return NULL;
}
Player* World::LoadPlayer(const std::string &name) const
{

    if (PlayerExists(name))
        {
            Player* p=new Player();
            p->SetName(name);
            p->Load();
            return p;
        }

    return NULL;
}

void World::GetChannelNames(std::list <std::string>* out)
{
    std::map<int,Channel*>::iterator it, itEnd;

    itEnd = _channels->end();
    for (it = _channels->begin(); it != itEnd; ++it)
        {
            out->push_back(((*it).second)->GetName());
        }
}

BOOL World::ChannelExists(Channel* chan)
{
    std::map<int, Channel*>::iterator it, itEnd;

    itEnd = _channels->end();
    for (it = _channels->begin(); it != itEnd; ++it)
        {
            if ((*it).second == chan)
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
            (*_channels)[_chanid]=chan;
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
            _options->AddOption(opt);
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

Channel* World::FindChannel(const int id) const
{
    if (!_channels->count(id))
        {
            return NULL;
        }
    return (*_channels)[id];
}

Channel* World::FindChannel(const std::string &name)
{
//This method is a bit slower because we have to iterate through the mapping ourselves.
    std::map<int,Channel*>::iterator it;
    std::map<int, Channel*>::iterator itEnd;

    itEnd = _channels->end();
    for (it = _channels->begin(); it != itEnd; ++it)
        {
            if ((*it).second->GetName()==name)
                {
                    return ((*it).second);
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
    if (!_banner)
        {
            return false;
        }

    memset(_banner,0, (size_t)fs.st_size+1);
//open and load the banner:
    FILE* banner_fd=fopen(LOGIN_FILE,"r");
    if (!banner_fd)
        {
            WriteLog("Could not fopen banner file.", CRIT);
            delete []_banner;
            _banner = NULL;
            return false;
        }
    if ((int)fread(_banner,1, (size_t)fs.st_size,banner_fd)!=(int)fs.st_size)
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
    if (!_motd)
        {
            delete []_banner;
            _banner = NULL;
            return false;
        }

    memset(_motd,0,(size_t)(fs.st_size+1));
    FILE* motd_fd=fopen(MOTD_FILE,"r");
    if (!motd_fd)
        {
            WriteLog("Could not fopen MOTD.", CRIT);
            delete [] _banner;
            delete [] _motd;
            _motd = _banner = NULL;
            return false;
        }

    if ((int)fread(_motd,1,fs.st_size,motd_fd)!=(int)fs.st_size)
        {
            WriteLog("Error loading MOTD.", CRIT);
            delete [] _banner;
            delete [] _motd;
            _motd = _banner = NULL;
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
    if (_users->size())
        {
            for (auto pit: *_users)
                {
                    pit->Update();
                }
        }
    for (auto npc: *_npcs)
        {
            (npc.second)->Update();
        }
    for (auto zone: *_zones)
        {
            zone->Update();
        }
    callouts->Update();

    _updates ++;
    gettimeofday(&end, NULL);
    _totalUpdateTime += ((end.tv_sec - start.tv_sec) * 1000000);
    _totalUpdateTime += (end.tv_usec - start.tv_usec);
//sleep so that we don't kill our cpu
    _totalSleepTime += _server->Sleep(PULSES_PER_SECOND);
}

BOOL World::RegisterComponent(ComponentMeta* meta)
{
    return _cfactory->RegisterComponent(meta->GetName(), meta);
}
Component*  World::CreateComponent(const std::string &name)
{
    return _cfactory->Create(name);
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
    if (!_properties->count(name))
        {
            (*_properties)[name]=ptr;
            return true;
        }
    return false;
}
void* World::GetProperty(const std::string &name) const
{
    if (_properties->count(name))
        {
            return (*_properties)[name];
        }
    return NULL;
}
BOOL World::RemoveProperty(const std::string &name)
{
    if (_properties->count(name))
        {
            _properties->erase(name);
            return true;
        }
    return false;
}

BOOL World::DoCommand(Player* mobile,std::string args)
{
    Room* location = NULL;
    std::vector<Command*>* cptr = commands.GetPtr();
    std::string cmd = ""; // the parsed command name
    const char *line = args.c_str(); // the command line
    int len = strlen(line); // get length of string
    int i = 0; // counter
    std::vector<std::string> params; // the parameters being passed to the command
    //std::list<Command*>* externals; //external commands
    std::vector <Command*>::iterator it; //an iterator for iterating through the command list
    std::vector <Command*>::iterator itEnd; //an iterator to point to the end of the commands list.

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
//make the command lowercase
    /*
        for (int n = 0; n < (int)cmd.length(); n++) {
            cmd[n] = tolower(cmd[n]);
        }
    */

    // are there any arguments to parse?
    if (i != len)
        {
            // parse arguments
            for (; i < len; i++)
                {
                    if (line[i] == ' ') continue;
                    // is it a quoated argument
                    /*
                                if ((line[i] == '\'') || (line[i] == '"')) {
                                    char match = line[i];
                                    int arg_start = i + 1;
                                    i++;
                                    // loop until we reach the closing character
                                    for (; i < len; i++) if (line[i] == match) break;
                                    int arg_end = i;
                                    params.push_back(args.substr(arg_start, arg_end - arg_start));
                                }
                    */
                    if (isprint(line[i]))
                        {
                            int arg_start = i;
                            for (; i < len; i++)
                                if ((line[i] == ' '))
                                    break;
                            int arg_end = i;
                            params.push_back(args.substr(arg_start, arg_end - arg_start));
                        }
                }
        }
//locate and execute the command:
//check the built-in commands first, then contents, then location.
    itEnd = cptr->end();
    for (it = cptr->begin(); it != itEnd; ++it)
        {
            if (((*it)->GetName() == cmd)||((*it)->HasAlias(cmd, true)))
                {
                    if (!mobile->HasAccess((*it)->GetAccess()))
                        {
                            return false;
                        }
                    switch((*it)->GetType())
                        {
                        default:
                            WriteLog("Invalid command type.");
                            break;
                        case normal:
                        case social:
                        case movement:
                        case channel:
                            (*it)->Execute((*it)->GetName(), mobile, params, (*it)->GetSubcmd());
                            return true;
                            break;
                        case script:
                            break;
                        }
                }
        }
//todo: check inventory and room commands here.
    location = (Room*)mobile->GetLocation();
    if (location)
        {
            cptr = location->commands.GetPtr();
            itEnd = cptr->end();
            for (it = cptr->begin(); it != itEnd; ++it)
                {
                    if (((*it)->GetName() == cmd)||((*it)->HasAlias(cmd, true)))
                        {
                            if (!mobile->HasAccess((*it)->GetAccess()))
                                {
                                    return false;
                                }
                            switch((*it)->GetType())
                                {
                                default:
                                    WriteLog("Invalid command type.");
                                    break;
                                case normal:
                                case social:
                                case movement:
                                case channel:
                                    (*it)->Execute((*it)->GetName(), mobile, params, (*it)->GetSubcmd());
                                    return true;
                                    break;
                                case script:
                                    break;
                                }
                        }
                }
        }
    return false;
}

Entity* World::MatchKeyword(const std::string &name, Player* caller)
{
    if ((name=="me")||(name==caller->GetName()))
        {
            return (Entity*)caller;
        }
    if (name.length() < caller->GetName().length() && caller->GetName().substr(name.length()) == name)
        {
            return (Entity*)caller;
        }
    if (name == "here")
        {
            return (caller->GetLocation());
        }

    return NULL;
}

Entity* World::MatchObject(const std::string &name,Player* caller)
{
    std::list<Entity*> *contents; //holds contents for the location and current caller.
    std::list<Entity*>* val;
    std::list<Entity*>::iterator it, itEnd;
    Entity* obj = NULL;

    obj = MatchKeyword(name, caller);
    if (obj)
        {
            return obj;
        }

    contents = new std::list<Entity*>();
    val = caller->GetLocation()->GetContents();
    contents->insert(contents->begin(), val->begin(), val->end());
    val = caller->GetContents();
    contents->insert(contents->begin(), val->begin(), val->end());
    obj = MatchObjectInList(name, val);
    delete contents;
    return obj;
}
Entity* World::MatchObjectInList(const std::string &name, std::list<Entity*> *olist)
{
    std::list<Entity*>::iterator it, itEnd;
    std::string sub; //used for holding the subpart of the number.
    std::string temp; //used for holding a temp copy of the name after it is trimmed.
    std::string alias; //holds a copy of the alias.
    Entity* obj = NULL; //the object we are currently examining/returning.
    std::vector<std::string>* aliases = NULL; //a list of aliases.
    std::vector<std::string>::iterator ait, aitEnd;
    int number = 0; //used for holding the actual number.
    int count = 0; //used for holding the number of objects found.
    size_t marker; //used for holding the position of the '.'.

    if (!olist->size())
        {
            return NULL;
        }

//we check to see if the string has a '.', if so, there's a number.
    marker = name.find_first_of(".");
//check to see if it is 1) at the beginning, or 2) at the end.
    if ((marker == 0) || (marker == name.length()))
        {
            return NULL;
        }

    if (marker != std::string::npos)   //we found something.
        {
            sub = name.substr(marker); //the subnumber.
            temp = name.substr(marker+1); //trim off the x. bit
            number = atoi(sub.c_str());
            if (number == 0)
                {
                    return NULL;
                }

            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd && count < number; ++it)
                {
                    obj = *it;
                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    count++; //we found a match, increase the counter.
                                    continue;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            count++;
                            continue;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = (*ait);
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    count++; //we found a match, increase the counter.
                                                    continue;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            count++;
                                            continue;
                                        }
                                }
                        }
                }
            if (count != (number -1))
                {
                    return NULL;
                }
            else
                {
                    return obj;
                }
        }
    else
        {
            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd; ++it)
                {
                    obj = *it;

                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    return obj;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            return obj;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = *ait;
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    return obj;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            return obj;
                                        }
                                }
                        }
                }
        }

    return NULL;
}
Entity* World::MatchObjectInVector(const std::string &name, std::vector<Entity*> *olist)
{
    std::vector<Entity*>::iterator it, itEnd;
    std::string sub; //used for holding the subpart of the number.
    std::string temp; //used for holding a temp copy of the name after it is trimmed.
    std::string alias; //holds a copy of the alias.
    Entity* obj = NULL; //the object we are currently examining/returning.
    std::vector<std::string>* aliases = NULL; //a list of aliases.
    std::vector<std::string>::iterator ait, aitEnd;
    int number = 0; //used for holding the actual number.
    int count = 0; //used for holding the number of objects found.
    size_t marker; //used for holding the position of the '.'.

    if (!olist->size())
        {
            return NULL;
        }

//we check to see if the string has a '.', if so, there's a number.
    marker = name.find_first_of(".");
//check to see if it is 1) at the beginning, or 2) at the end.
    if ((marker == 0) || (marker == name.length()))
        {
            return NULL;
        }

    if (marker != std::string::npos)   //we found something.
        {
            sub = name.substr(marker); //the subnumber.
            temp = name.substr(marker+1); //trim off the x. bit
            number = atoi(sub.c_str());
            if (number == 0)
                {
                    return NULL;
                }

            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd && count < number; ++it)
                {
                    obj = *it;
                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    count++; //we found a match, increase the counter.
                                    continue;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            count++;
                            continue;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = (*ait);
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    count++; //we found a match, increase the counter.
                                                    continue;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            count++;
                                            continue;
                                        }
                                }
                        }
                }
            if (count != (number -1))
                {
                    return NULL;
                }
            else
                {
                    return obj;
                }
        }
    else
        {
            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd; ++it)
                {
                    obj = *it;

                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    return obj;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            return obj;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = *ait;
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    return obj;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            return obj;
                                        }
                                }
                        }
                }
        }

    return NULL;
}

BOOL World::AddZone(Zone* zone)
{
    std::vector<Zone*>::iterator it;
    std::vector<Zone*>::iterator itEnd = _zones->end();

    if (_zones->size())
        {
            for (it=_zones->begin(); it != itEnd; ++it)
                {
                    if ((*it)==zone)
                        {
                            return false;
                        }
                }
        }
    _zones->push_back(zone);
    return true;
}
BOOL World::RemoveZone(Zone* zone)
{
    std::vector<Zone*>::iterator it;
    std::vector <Zone*>::iterator itEnd = _zones->end();
    for (it = _zones->begin(); it != itEnd; ++it)
        {
            if ((*it)==zone)
                {
                    _zones->erase(it);
                    return true;
                }
        }

    return false;
}
Zone* World::GetZone(const std::string &name)
{
    std::vector <Zone*>::iterator it;
    std::vector<Zone*>::iterator itEnd = _zones->end();


    for (it=_zones->begin(); it != itEnd; it++)
        {
            if (name==(*it)->GetName())
                {
                    return (*it);
                }
        }

    return NULL;
}
BOOL World::GetZones(std::vector<Zone*> *zones)
{
    if (!zones)
        {
            return false;
        }

    std::vector<Zone*>::iterator it;
    std::vector<Zone*>::iterator itEnd = _zones->end();

    if (_zones->size())
        {
            for (it=_zones->begin(); it != itEnd; ++it)
                {
                    zones->push_back((*it));
                }
        }
    return true;
}

Entity* World::CreateObject(VNUM obj)
{
    if (obj != 0 && !VirtualExists(obj))
        {
            WriteLog("Tried to create virtual with nonexistant vnum", ERR);
            return NULL;
        }

    Entity* object = NULL;
    StaticObject* virt = NULL;

    virt = (*_objects)[obj];
    object = virt->Create();
    return object;
}
BOOL World::RecycleObject(Entity* obj)
{
    if(!obj)
        {
            return false;
        }

//we recursively recycle everything in contents.
    Entity* location = NULL;
    StaticObject* vobj = NULL;
    std::list<Entity*>::iterator it, itEnd;
    std::list<Entity*>* contents = obj->GetContents();

//recursively delete objects held by the object being deleted.
    itEnd = contents->end();
    for (it = contents->begin(); it != itEnd; ++it)
        {
            RecycleObject((*it));
        }

//check to see if this object is stored in another. If so, we need to remove it.
    location = obj->GetLocation();
    if (location)
        {
            location->ObjectLeave(obj);
        }

//recycle this instance with it's virtual object, if there is one.
    if (obj->GetOnum())
        {
            vobj = GetVirtual(obj->GetOnum());
            if (vobj)
                {
                    vobj->Recycle(obj);
                }
        }

    events.CallEvent("ObjectDestroyed", NULL, (void*)obj);
    delete obj;
    return true;
}

BOOL World::AddVirtual(StaticObject* obj)
{
    VNUM num = 0;
    if (!obj)
        {
            return false;
        }

    num = obj->GetOnum();
    if (VirtualExists(num))
        {
            return false;
        }

    (*_objects)[num] = obj;
    return true;
}
BOOL World::VirtualExists(VNUM num)
{
    return (_objects->count(num)? true:false);
}
StaticObject* World::GetVirtual(VNUM num)
{
    if (!VirtualExists(num))
        {
            return NULL;
        }

    return (*_objects)[num];
}
BOOL World::RemoveVirtual(VNUM num)
{
    StaticObject* virt = GetVirtual(num);
    if (!virt)
        {
            return false;
        }

    virt->RecycleContents();
    _objects->erase(num);
    return true;
}

BOOL World::AddRoom(Room* room)
{
    VNUM num = 0;
    if (!room)
        {
            return false;
        }

    num = room->GetOnum();
    if (RoomExists(num))
        {
            return false;
        }

    (*_rooms)[num] = room;
    return true;
}
BOOL World::RemoveRoom(VNUM num)
{
    Room* room = NULL;

    room = GetRoom(num);
    if (!room)
        {
            return false;
        }

    _rooms->erase(room->GetOnum());
    return true;
}
BOOL World::RoomExists(VNUM num)
{
    return _rooms->count(num)? true : false;
}
Room* World::GetRoom(VNUM num)
{
    if (!RoomExists(num))
        {
            return NULL;
        }

    return (*_rooms)[num];
}

BOOL World::AddNpc(Npc* mob)
{
    VNUM num = 0;

    if (!mob)
        {
            return false;
        }

    num = mob->GetOnum();
    if (NpcExists(num))
        {
            return false;
        }

    (*_npcs)[num] = mob;
    return true;
}
BOOL World::RemoveNpc(VNUM num)
{
    Npc* mob = NULL;

    mob = GetNpc(num);
    if (!mob)
        {
            return false;
        }

    _npcs->erase(mob->GetOnum());
    return true;
}
bool World::NpcExists(VNUM num)
{
    return  (_npcs->count(num) ? true : false);
}
Npc*  World::GetNpc(VNUM num)
{
    if (!NpcExists(num))
        {
            return NULL;
        }

    return (*_npcs)[num];
}

BOOL World::LogExists(const std::string &name)
{
    if (_logs->count(name))
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
                    (*_logs)[name] = log;
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
    return (_prompts->count(prompt)==0? false:true);
}
BOOL World::RegisterPrompt(char c, PROMPTCB callback)
{
    if (PromptExists(c))
        {
            return false;
        }

    (*_prompts)[c] = callback;
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
                            ret += ((*_prompts)[(*it)])(mobile);
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

    (*_state)[name] = s;
    return true;
}
BOOL World::RemoveState(const std::string &name)
{
    if (!StateExists(name))
        {
            return false;
        }

    _state->erase(name);
    return true;
}
BOOL World::StateExists(const std::string &name)
{
    return (_state->count(name)==1?true:false);
}

BOOL World::SaveState()
{
    std::map<std::string, ISerializable*>::iterator sit, sitEnd;
    TiXmlElement* element = NULL;
    sitEnd = _state->end();
    for (sit = _state->begin(); sit != sitEnd; ++sit)
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
                    WriteLog(std::string("Could not find a matching registered state for "+name+" in the state register. This state will not be deserialized."), WARN);
                    continue;
                }
            else
                {
                    (*_state)[name]->Deserialize(root);
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
