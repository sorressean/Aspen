#include <list>
#include <map>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <functional>
#include "mud.h"
#include "conf.h"
#include "com_gen.h"
#include "utils.h"
#include "player.h"
#include "world.h"
#include "option.h"
#include "optionMeta.h"
#include "inputHandlers.h"
#include "property.h"
#include "living.h"
#include "constants.h"

void InitializeGenCommands()
{
    World* world = World::GetPtr();

    world->WriteLog("Initializing general commands.");
    world->commands.AddCommand(new CMDQuit());
    world->commands.AddCommand(new CMDSave());
    world->commands.AddCommand(new CMDBackup());
    world->commands.AddCommand(new CMDWho());
    world->commands.AddCommand(new CMDToggle());
    world->commands.AddCommand(new CMDScore());
    world->commands.AddCommand(new CMDCommands());
    world->commands.AddCommand(new CMDHist());
    world->commands.AddCommand(new CMDUptime());
    world->commands.AddCommand(new CMDWhois());
    world->commands.AddCommand(new CMDLook());
    world->commands.AddCommand(new CMDCoord());
    world->commands.AddCommand(new CMDSuicide());
    world->commands.AddCommand(new CMDSay());
    world->commands.AddCommand(new CMDEmote());
    world->commands.AddCommand(new CMDPrompt());
    world->commands.AddCommand(new CMDSockstats());
    world->commands.AddCommand(new CMDExits());
    world->commands.AddCommand(new CMDExperience());
}

//quit
CMDQuit::CMDQuit()
{
    SetName("quit");
}
BOOL CMDQuit::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    mobile->Write("Thanks for playing, please come back again!\n");
    mobile->GetSocket()->Kill();
    return true;
}

//save
CMDSave::CMDSave()
{
    SetName("save");
}
BOOL CMDSave::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!mobile->Save())
        {
            mobile->Message(MSG_NORMAL, "You just recently saved, please wait before trying again.");
        }
    else
        {
            mobile->Message(MSG_NORMAL, "saved.");
        }
    return true;
}
CMDBackup::CMDBackup()
{
    SetName("backup");
}
BOOL CMDBackup::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!args.size())
        {
            mobile->Message(MSG_CRITICAL, "Please type backup confirm to confirm the backup process.\nPlease note: Backups are to prevent you from losing equipment or data. Backing up after you have lost equipment or data will make it impossible to recover your information, thus please use backup -BEFORE-.");
            return true;
        }
    if (args[0] != "confirm")
        {
            mobile->Message(MSG_CRITICAL, "Please type backup confirm to confirm the backup process.\nPlease note: Backups are to prevent you from losing equipment or data. Backing up after you have lost equipment or data will make it impossible to recover your information, thus please use backup -BEFORE-.");
            return true;
        }

    if (!mobile->Backup())
        {
            mobile->Message(MSG_ERROR, "You have used backup to recently, please wait before trying again.");
            return false;
        }
    else
        {
            mobile->Message(MSG_INFO, "Your data has been backed up.");
        }

    return true;
}

//who
CMDWho::CMDWho()
{
    SetName("who");
    AddAlias("wh");
}
BOOL CMDWho::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    std::list <Player*>::iterator it;
    std::list <Player*>::iterator itEnd;
    std::stringstream st;

    mobile->Write(Center(Capitalize(MUD_NAME),80)+"\n");
    mobile->Write(Repete("-",80));

    itEnd=world->GetPlayers()->end();
    for (it = world->GetPlayers()->begin(); it != itEnd; ++it)
        {
            st << "[" << std::left << std::setw(4) << (*it)->GetLevel() << "] ";
            if ((*it)->HasAccess(RANK_GOD))
                {
                    st << C_BLUE << "[ADMIN] " << C_RESET;
                }
            st << Capitalize((*it)->GetName()) << " " << (*it)->GetTitle();
            mobile->Message(MSG_LIST, st.str());
            st.str("");
        }
    mobile->Write(Repete("-",80));
    st << MUD_NAME << " currently has " << (int)world->GetPlayers()->size() << ((int)world->GetPlayers()->size()==1?"user ":"users ") << "online.\n";
    mobile->Write(st.str());
    return true;
}

//toggle
CMDToggle::CMDToggle()
{
    SetName("toggle");
    AddAlias("tog");
}
BOOL CMDToggle::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world  = World::GetPtr();
    OptionManager* omanager = world->GetOptionManager();
    OptionMeta* gopt = nullptr;
    std::vector<Option*> *poptions = nullptr;
    std::vector<OptionMeta*> goptions;
    std::vector<std::string> cols;
    std::vector<std::string> headers;

//if no args were given, print out a list of options:
    if (!args.size())
        {
            headers.push_back("option");
            headers.push_back("value");

            poptions=mobile->GetOptions();
            for (auto pit: *poptions)
                {
                    if (pit->GetMeta()->CanToggle())
                        {
                            cols.push_back(pit->GetMeta()->GetFullName());
                            cols.push_back(pit->GetValue().GetInt()==0?"off: ":"on ");
                        }
                }

            omanager->ListOptions(&goptions);
            for (OptionMeta* opt: goptions)
                {
                    if (mobile->OptionExists(opt->GetName()))
                        {
                            continue;
                        }
                    if (opt->CanToggle())
                        {
                            cols.push_back(opt->GetName());
                            cols.push_back(opt->GetValue().GetInt()==0?"off: ":"on ");
                        }
                }

            mobile->Message(MSG_LIST, Columnize(&cols, 2, &headers));
            mobile->Message(MSG_INFO, "Note: You can use toggle help <option> to get more help on a specific option.");
            return true;
        }

//see if they want help on a specific option.
    if (args[0] == "help" && args.size() == 2)
        {
            gopt = omanager->GetOption(args[1]);
            if (!gopt)
                {
                    mobile->Message(MSG_ERROR, "That option does not exist.");
                    return false;
                }

            mobile->Message(MSG_INFO, gopt->GetHelp());
            return true;
        }

    mobile->ToggleOption(args[0]);
    return true;
}

//score
CMDScore::CMDScore()
{
    SetName("score");
    AddAlias("sc");
}
BOOL CMDScore::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    using std::endl;
    using std::setw;
    using std::left;
    using std::right;

    std::stringstream st;

    st << Center(Capitalize(mobile->GetName()), 80) << endl;
    st << Repete("-", 80) << endl << endl;

    TimeInfo tm(mobile->GetOnlineTime());
    mobile->Write("Online time:\t"+tm.ToString()+"\n");
    mobile->Write(Repete("-",80)+"\n");

    st << left << setw(20) << "level: " << mobile->GetLevel() << std::endl;
    mobile->Write(st.str());
    return true;
}

//channels
CMDChan::CMDChan()
{
    SetName("channels");
    AddAlias("chan");
}
BOOL CMDChan::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    Channel* chan=(Channel*)world->FindChannel(subcmd);
    std::string message=Explode(args);

    if (chan==NULL)
        {
            mobile->Message(MSG_ERROR,"That channel doesn't exist.");
            return false;
        }

    chan->Broadcast((Player*)mobile,message,true);
    return true;
}

//commands
CMDCommands::CMDCommands()
{
    SetName("commands");
}
BOOL CMDCommands::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    std::vector <std::string> commands;
    std::vector<std::string> headers;
    headers.push_back("command");
    headers.push_back("command");
    headers.push_back("command");
    headers.push_back("command");

    COMMAND_TYPE type;

    if (!args.size())
        {
            type=all;
        }
    else     if (args[0] == "general")
        {
            type = normal;
        }
    else if (args[0] == "all")
        {
            type = all;
        }
    else if (args[0] == "social")
        {
            type = social;
        }
    else if (args[0] == "movement")
        {
            type = movement;
        }
    else if (args[0] == "channel")
        {
            type = channel;
        }
    else
        {
            mobile->Message(MSG_INFO, "You must specify a valid type.\nValid types are: movement, general, all, channel and social.");
            return false;
        }

    world->commands.ListCommands(&commands,mobile, type);
    mobile->Message(MSG_LIST, Columnize(&commands, 4, &headers));
    return true;
}

//history
CMDHist::CMDHist()
{
    SetName("history");
    AddAlias("hist");
}
BOOL CMDHist::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    Channel* chan=NULL;
    std::list <HistoryNode*> *history;
    std::list <HistoryNode*>::iterator it, itEnd;
    TimeInfo tm;

    if (!args.size())
        {
            mobile->Message(MSG_ERROR,"Syntax: hist [channel name].");
            return false;
        }

    chan=world->FindChannel(args[0]);
    if (!chan)
        {
            mobile->Message(MSG_ERROR,"That channel doesn't exist.");
            return false;
        }

    history=chan->GetHistory();

    if ((!history) || (!history->size()))
        {
            mobile->Message(MSG_INFO,"There is no recorded history, perhaps you should say something?");
            return true;
        }

    for (it=history->begin(); it!=history->end(); it++)
        {
            tm.Calculate(time(NULL)-(*it)->when);
            mobile->Message(MSG_LIST,tm.ToString()+": "+(*it)->message);
        }

    return true;
}

//uptime
CMDUptime::CMDUptime()
{
    SetName("uptime");
}
BOOL CMDUptime::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    TimeInfo ruptime(time(NULL)-world->GetRealUptime());
    TimeInfo cuptime(time(NULL)-world->GetCopyoverUptime());

    mobile->Message(MSG_INFO,Capitalize(MUD_NAME)+" has been up for "+ruptime.ToString()+".");
    if (world->GetCopyoverUptime()==world->GetRealUptime())
        {
            mobile->Message(MSG_INFO,"There hasn't been a copyover this reboot.");
        }
    else
        {
            mobile->Message(MSG_INFO,"The last copyover occured "+cuptime.ToString()+" ago.");
        }

    return true;
}

//whois
CMDWhois::CMDWhois()
{
    SetName("whois");
    AddAlias("finger");
}
BOOL CMDWhois::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Player* targ=NULL;
    BOOL load=false;
    TimeInfo tm;

    if ((!args.size())||(args.size()!=1))
        {
            mobile->Message(MSG_ERROR,"Syntax: whois <player>.");
            return false;
        }
//check to see if the player is online:
    targ=world->FindPlayer(args[0]);
    if (!targ)
        {
//check to see if the player can be loaded
            targ=world->LoadPlayer(args[0]);
            load=true;
        }
//check to see if the load succeeded.
    if (!targ)
        {
            mobile->Message(MSG_ERROR,"Couldn't find the specified player.");
            return false;
        }

//check to see if a player is trying to retrieve info on a god
    if ((targ->HasAccess(RANK_GOD))&&(!mobile->HasAccess(RANK_GOD)))
        {
            mobile->Message(MSG_ERROR,"You can't whois the gods.");
//delete the pointer if it was allocated by load
            if (load)
                {
                    delete targ;
                }
            return false;
        }

    tm.Calculate(targ->GetOnlineTime());
    mobile->Message(MSG_LIST,Capitalize(targ->GetName()+" "+targ->GetTitle()));
    mobile->Message(MSG_LIST,Repete("-",80));
    mobile->Message(MSG_LIST,Capitalize(targ->GetName())+" has spent "+tm.ToString()+" online.");
    mobile->Message(MSG_LIST,Repete("-",80));
    if (load)
        {
            delete targ;
        }

    return true;
}

//look
CMDLook::CMDLook()
{
    SetName("look");
    AddAlias("l");
}
BOOL CMDLook::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Entity* obj = NULL;

    if (!args.size())
        {
            mobile->Message(MSG_INFO,mobile->GetLocation()->DoLook(mobile));
            return true;
        }
    obj =world->MatchObject(args[0],mobile);
    if (obj==NULL)
        {
            mobile->Message(MSG_ERROR,"You don't see that here.");
            return false;
        }

    mobile->Message(MSG_INFO,obj->DoLook(mobile));
    return true;
}

CMDCoord::CMDCoord()
{
    SetName("coord");
    AddAlias("coords");
}
BOOL CMDCoord::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Room* location = (Room*)mobile->GetLocation();
    std::stringstream st;
    point *p = NULL;

    if (!location)
        {
            mobile->Message(MSG_INFO, "You are not in a room.");
            return false;
        }

    p = location->GetCoord();
    st << "You are at coords (" << p->x << ", " << p->y << ", " << p->z << ").";
    mobile->Message(MSG_INFO, st.str());
    return true;
}

CMDSuicide::CMDSuicide()
{
    SetName("suicide");
}
BOOL CMDSuicide::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    mobile->Message(MSG_NORMAL, "Are you sure you want to suicide? Please note that this will delete your current character, as well as any equipment you may own. Type yes if you want to continue, or no otherwise.");
    YesNoHandler::CreateHandler(mobile->GetSocket(), std::bind(&CMDSuicide::Confirm, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}
void CMDSuicide::Confirm(Socket* sock, BOOL choice)
{
    Player* mobile = sock->GetPlayer();
    std::string path = std::string(PLAYER_DIR)+mobile->GetName();
    World* world = World::GetPtr();

    if (!choice)
        {
            mobile->Message(MSG_INFO, "You will not suicide.");
            sock->ClearInput();
            return;
        }

    world->WriteLog(mobile->GetName()+" has just suicided.");
    sock->Kill();
    unlink(path.c_str());
}

CMDSay::CMDSay()
{
    SetName("say");
}
BOOL CMDSay::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Say what?");
            return false;
        }

    std::string message = Explode(args);
    ((Room*)mobile->GetLocation())->TellAllBut(Capitalize(mobile->GetName())+" says, '"+message+"'", mobile);
    mobile->Message(MSG_INFO, "You say, '"+message+"'");
    return true;
}

CMDEmote::CMDEmote()
{
    SetName("emote");
}
BOOL CMDEmote::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Emote what?");
            return false;
        }

    std::string message = Explode(args);
    ((Room*)mobile->GetLocation())->TellAll(Capitalize(mobile->GetName())+" "+message);
    return true;
}

CMDPrompt::CMDPrompt()
{
    SetName("prompt");
}
BOOL CMDPrompt::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!args.size())
        {
            mobile->Message(MSG_INFO, "Your current prompt is:\n"+mobile->GetPrompt());
            return false;
        }

    std::string prompt = Explode(args);
    mobile->SetPrompt(prompt);
    mobile->Message(MSG_INFO, "Prompt set to:\n"+prompt);
    return false;
}

CMDSockstats::CMDSockstats()
{
    SetName("sockstats");
}
BOOL CMDSockstats::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Socket*sock = mobile->GetSocket();
    float ratio = 0;

    std::stringstream st;
    st << "Socket stats" << std::endl;
    st << Repete('-', 80) << std::endl;
    st << "Window size: " << sock->GetWindowWidth() << "X" << sock->GetWindowHeight() << std::endl;
    st << "Terminal type: " << sock->GetTermtype() << "." << std::endl;
    st <<  "Connecting from: " << sock->GetHost() << "." << std::endl;
    st << "Total data received (from you to the mud): " << sock->GetTotalReceived() << "bytes (" << (float)sock->GetTotalReceived()/1024.0F << " KB)." << std::endl;
    if (!sock->IsCompressing())
        {
            st << "You do not appear to have MCCP2!" << std::endl;
            st << "Total data sent (from the mud to you) " << sock->GetTotalSent() << "bytes ( " << (float)sock->GetTotalSent()/1024.0F << " KB)." << std::endl;
        }
    else
        {
            ratio = ((float)sock->GetTotalCompressedSent() / (float)sock->GetTotalSent())*100.0F;
            st << "You appear to have MCCP2!" << std::endl;
            st << "Total data sent (from the mud to you) " << sock->GetTotalSent() << "bytes (" << (float)sock->GetTotalSent()/1024.0F << " KB)." << std::endl;
            st << "Total compressed data sent (from the mud  to you) " << sock->GetTotalCompressedSent() << "bytes ( " << sock->GetTotalCompressedSent()/1024.0F << " KB)." << std::endl;
            st << "Compression ratio: " << ratio << "%." << std::endl;
        }

    mobile->Message(MSG_NORMAL, st.str());
    return true;
}

CMDExits::CMDExits()
{
    SetName("exits");
}
BOOL CMDExits::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
{
    Room* location = (Room*)mobile->GetLocation();
    if (!location)
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    mobile->Message(MSG_INFO, location->TellObviousExits());
    return true;
}

CMDExperience::CMDExperience()
{
    SetName("experience");
    AddAlias("exp");
}
BOOL CMDExperience::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
{
    std::stringstream st;
    float perc = 0;
    int exp = mobile->GetExperience();
    int level = mobile->GetLevel();
    int cost = GetLevelCost(level+1);
    perc = (exp/cost) * 100.0F;
    st << "You have " << exp << "experience and need " << cost << " to reach level " << level+1;
    st << " (" << perc << "%)." << std::endl;
    st << "Your exp cap is " << mobile->GetCap() << std::endl;
    mobile->Message(MSG_INFO, st.str());
    return true;
}
