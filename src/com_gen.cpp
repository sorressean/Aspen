#include <unistd.h>
#include <list>
#include <map>
#include <sstream>
#include <iomanip>
#include <cmath>
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
#include "log.h"

void InitializeGenCommands()
{
    World* world = World::GetPtr();

    WriteLog("Initializing general commands.");
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
    world->commands.AddCommand(new CMDSuicide());
    world->commands.AddCommand(new CMDSay());
    world->commands.AddCommand(new CMDEmote());
    world->commands.AddCommand(new CMDPrompt());
    world->commands.AddCommand(new CMDSockstats());
    world->commands.AddCommand(new CMDExits());
}

CMDQuit::CMDQuit()
{
    SetName("quit");
    SetType(CommandType::Misc);
}
bool CMDQuit::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    mobile->Write("Thanks for playing, please come back again!\n");
    mobile->GetSocket()->Kill();
    return true;
}

CMDSave::CMDSave()
{
    SetName("save");
    SetType(CommandType::Misc);
}
bool CMDSave::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!mobile->Save())
        {
            mobile->Message(MSG_NORMAL, "You just recently saved, please wait before trying again.");
        }
    else
        {
            mobile->Message(MSG_NORMAL, "Saved.");
        }

    return true;
}

CMDBackup::CMDBackup()
{
    SetName("backup");
    SetType(CommandType::Misc);
}
bool CMDBackup::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
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

CMDWho::CMDWho()
{
    SetName("who");
    AddAlias("wh");
    SetType(CommandType::Information);
}
bool CMDWho::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::list<Player*> *players;
    std::stringstream st;

    st << Center(Capitalize(MUD_NAME), 80) << std::endl;
    st << Repeat("-", 80);

    players = world->GetPlayerManager().GetPlayers();
    for (auto it: *players)
        {
            if (it->HasAccess(RANK_GOD))
                {
                    st << C_BLUE << "[ADMIN] " << C_RESET;
                }
            st << Capitalize(it->GetName()) << " " << it->GetTitle();
        }
    st << Repeat("-", 80);
    st << MUD_NAME << " currently has " << players->size() << (players->size()==1?" user ":" users ") << "online.\n";
    mobile->Message(MSG_LIST, st.str());

    return true;
}

CMDToggle::CMDToggle()
{
    SetName("toggle");
    AddAlias("tog");
    SetType(CommandType::Misc);
}
bool CMDToggle::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
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
                            cols.push_back(pit->GetMeta()->GetName());
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

CMDScore::CMDScore()
{
    SetName("score");
    AddAlias("sc");
    SetType(CommandType::Information);
}
bool CMDScore::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    using std::endl;
    using std::setw;
    using std::left;
    using std::right;

    std::stringstream st;

    st << Center(Capitalize(mobile->GetName()), 80) << endl;
    st << Repeat("-", 80) << endl << endl;

    TimeInfo tm(mobile->GetOnlineTime());
    st << "Online time:\t"+tm.ToString()+"\n";
    st << Repeat("-",80) << endl;;
    mobile->Message(MSG_INFO, st.str());
    return true;
}

CMDChan::CMDChan()
{
    SetName("channels");
    AddAlias("chan");
    SetType(CommandType::Communication);
}
bool CMDChan::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Channel* chan = world->FindChannel(subcmd);
    std::string message=Explode(args);

    if (chan==nullptr)
        {
            mobile->Message(MSG_ERROR,"That channel doesn't exist.");
            return false;
        }

    chan->Broadcast(mobile,message,true);
    return true;
}

CMDCommands::CMDCommands()
{
    SetName("commands");
    SetType(CommandType::Information);
}

void CMDCommands::PrintCommandInfo(Player* mobile, CommandType filter)
{
    std::vector<std::string> names;
    std::stringstream st;
    GetCommands(mobile, names, filter);
    if (!names.size())
        {
            mobile->Message(MSG_INFO, "["+GetCommandTypeName(filter)+"](none)");
            return;
        }
    mobile->Message(MSG_LIST, "["+GetCommandTypeName(filter)+"]\n"+Columnize(&names, 3));
}

void CMDCommands::GetCommands(Player*mobile, std::vector<std::string>& names, CommandType filter)
{
    World* world = World::GetPtr();
    std::vector<std::string> commands;
    world->commands.ListCommands(&commands,mobile, filter);
    std::copy(commands.begin(), commands.end(), std::back_inserter(names));
}

void CMDCommands::Syntax(Player* mobile, int subcmd) const
{
    std::stringstream st;

    st << "Syntax: commands [";
    if (mobile->HasAccess(RANK_BUILDER))
        {
            st << "builder|";
        }
    if (mobile->HasAccess(RANK_ADMIN))
        {
            st << "admin|";
        }
    if (mobile->HasAccess(RANK_GOD))
        {
            st << "god|";
        }
    st << "information|object|movement|communication]";

    mobile->Message(MSG_INFO, st.str());
}

bool CMDCommands::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!args.size())
        {
            if (mobile->HasAccess(RANK_GOD))
                {
                    PrintCommandInfo(mobile, CommandType::God);
                }
            if (mobile->HasAccess(RANK_ADMIN))
                {
                    PrintCommandInfo(mobile, CommandType::Admin);
                }
            if (mobile->HasAccess(RANK_BUILDER))
                {
                    PrintCommandInfo(mobile, CommandType::Builder);
                }

            PrintCommandInfo(mobile, CommandType::Information);
            PrintCommandInfo(mobile, CommandType::Movement);
            PrintCommandInfo(mobile, CommandType::Communication);
            PrintCommandInfo(mobile, CommandType::Misc);
            PrintCommandInfo(mobile, CommandType::Object);
            return true;
        }

    if (args[0] == "god")
        {
            if (!mobile->HasAccess(RANK_GOD))
                {
                    Syntax(mobile, subcmd);
                    return false;
                }
            PrintCommandInfo(mobile, CommandType::God);
            return true;
        }
    else if (args[0] == "admin")
        {
            if (!mobile->HasAccess(RANK_ADMIN))
                {
                    Syntax(mobile, subcmd);
                    return false;
                }
            PrintCommandInfo(mobile, CommandType::Admin);
            return true;
        }
    else if (args[0] == "builder")
        {
            if (!mobile->HasAccess(RANK_BUILDER))
                {
                    Syntax(mobile, subcmd);
                    return false;
                }
            PrintCommandInfo(mobile, CommandType::Builder);
            return true;
        }
    else if (args[0] == "misc")
        {
            PrintCommandInfo(mobile, CommandType::Misc);
            return true;
        }
    else if (args[0] == "information")
        {
            PrintCommandInfo(mobile, CommandType::Information);
            return true;
        }
    else if (args[0] == "object")
        {
            PrintCommandInfo(mobile, CommandType::Object);
            return true;
        }
    else if (args[0] == "movement")
        {
            PrintCommandInfo(mobile, CommandType::Movement);
            return true;
        }
    else if (args[0] == "social")
        {
            PrintCommandInfo(mobile, CommandType::Social);
        }
    else if (args[0] == "communication")
        {
            PrintCommandInfo(mobile, CommandType::Communication);
            return true;
        }
    else
        {
            Syntax(mobile, subcmd);
            return false;
        }

    return true;
}

CMDHist::CMDHist()
{
    SetName("history");
    AddAlias("hist");
    SetType(CommandType::Information);
}
bool CMDHist::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Channel* chan=nullptr;
    std::list <HistoryNode*> *history = nullptr;;
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

    for (auto it: *history)
        {
            tm.Calculate(time(NULL) - it->when);
            mobile->Message(MSG_LIST,tm.ToString() + ": " + it->message);
        }

    return true;
}

CMDUptime::CMDUptime()
{
    SetName("uptime");
    SetType(CommandType::Information);
}
bool CMDUptime::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    TimeInfo ruptime(time(NULL)-world->GetRealUptime());
    TimeInfo cuptime(time(NULL)-world->GetCopyoverUptime());

    mobile->Message(MSG_INFO,Capitalize(MUD_NAME)+" has been up for "+ruptime.ToString()+".");
    if (world->GetCopyoverUptime() == 0)
        {
            mobile->Message(MSG_INFO,"There hasn't been a copyover this reboot.");
        }
    else
        {
            mobile->Message(MSG_INFO,"The last copyover occured "+cuptime.ToString()+" ago.");
        }

    return true;
}

CMDWhois::CMDWhois()
{
    SetName("whois");
    AddAlias("finger");
    SetType(CommandType::Information);
}
bool CMDWhois::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Player* targ=nullptr;
    bool load=false;
    TimeInfo tm;

    if ((!args.size())||(args.size()!=1))
        {
            mobile->Message(MSG_ERROR,"Syntax: whois <player>.");
            return false;
        }

//check to see if the player is online:
    targ=world->GetPlayerManager().FindPlayer(args[0]);
    if (!targ)
        {
//check to see if the player can be loaded
            targ=world->GetPlayerManager().LoadPlayer(args[0]);
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
    mobile->Message(MSG_LIST,Repeat("-",80));
    mobile->Message(MSG_LIST,Capitalize(targ->GetName())+" has spent "+tm.ToString()+" online.");
    mobile->Message(MSG_LIST,Repeat("-",80));
    if (load)
        {
            delete targ;
        }

    return true;
}

CMDLook::CMDLook()
{
    SetName("look");
    AddAlias("l");
    SetType(CommandType::Information);
}
bool CMDLook::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    BaseObject* obj = nullptr;
    ObjectContainer* location = mobile->GetLocation();

//look at environment:
    if (!args.size())
        {
            if (location->IsRoom())
                {
                    mobile->Message(MSG_INFO, ((Room*)mobile->GetLocation())->DoLook(mobile));
                    return true;
                }
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

CMDSuicide::CMDSuicide()
{
    SetName("suicide");
    SetType(CommandType::Misc);
}
bool CMDSuicide::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    mobile->Message(MSG_NORMAL, "Are you sure you want to suicide? Please note that this will delete your current character, as well as any equipment you may own. Type yes if you want to continue, or no otherwise.");
    YesNoHandler::CreateHandler(mobile->GetSocket(), std::bind(&CMDSuicide::Confirm, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}
void CMDSuicide::Confirm(Socket* sock, bool choice)
{
    Player* mobile = sock->GetPlayer();
    std::string path = std::string(PLAYER_DIR)+mobile->GetName();

    if (!choice)
        {
            mobile->Message(MSG_INFO, "You will not suicide.");
            sock->ClearInput();
            return;
        }

    WriteLog(mobile->GetName()+" has just suicided.");
    sock->Kill();
    unlink(path.c_str());
}

CMDSay::CMDSay()
{
    SetName("say");
    SetType(CommandType::Communication);
}
bool CMDSay::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
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
    SetType(CommandType::Communication);
}
bool CMDEmote::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
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
    SetType(CommandType::Misc);
}
bool CMDPrompt::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
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
    SetType(CommandType::Information);
}
bool CMDSockstats::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Socket*sock = mobile->GetSocket();
    float ratio = 0;

    std::stringstream st;
    st << "Socket stats" << std::endl;
    st << Repeat('-', 80) << std::endl;
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
    SetType(CommandType::Movement);
}
bool CMDExits::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
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
