#include <list>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>
#include "com_wiz.h"
#include "command.h"
#include "player.h"
#include "world.h"
#include "utils.h"
#include "calloutManager.h"

void InitializeWizCommands()
{
    World* world = World::GetPtr();

    world->WriteLog("Initializing wizard commands.");
    world->commands.AddCommand(new CMDCopyover());
    world->commands.AddCommand(new CMDMkgod());
    world->commands.AddCommand(new CMDShutdown());
    world->commands.AddCommand(new CMDMkbuilder());
    world->commands.AddCommand(new CMDBan());
    world->commands.AddCommand(new CMDSilence());
    world->commands.AddCommand(new CMDUnsilence());
    world->commands.AddCommand(new CMDDisconnect());
    world->commands.AddCommand(new CMDEcho());
    world->commands.AddCommand(new CMDSstatus());
    world->commands.AddCommand(new CMDForce());
}

CMDCopyover::CMDCopyover()
{
    SetName("copyover");
    SetAccess(RANK_GOD);
}
BOOL CMDCopyover::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    world->Copyover(mobile);
    return true;
}

CMDMkgod::CMDMkgod()
{
    SetName("mkgod");
    SetAccess(RANK_GOD);
}
BOOL CMDMkgod::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Player* target = NULL;

    if (!args.size())
        {
            mobile->Message(MSG_ERROR,"You must specify the person that you would like to make a god.\n");
            return false;
        }

    target=world->FindPlayer(args[0]);

    if (target==mobile)
        {
            mobile->Message(MSG_ERROR,"You may not make yourself a god.");
            return false;
        }
    if (target==NULL)
        {
            mobile->Message(MSG_ERROR,"That person couldn't be found.");
            return false;
        }
    if (BitIsSet(target->GetRank(),RANK_GOD))
        {
            mobile->Message(MSG_ERROR,"That person is already a god.");
            return false;
        }

    target->SetRank(BitSet(mobile->GetRank(), RANK_PLAYTESTER|RANK_NEWBIEHELPER|RANK_BUILDER|RANK_ADMIN|RANK_GOD));
    target->Message(MSG_INFO,"You suddenly feel more godlike.");
    mobile->Message(MSG_INFO, Capitalize(target->GetName())+" has been made a god.");
    return true;
}

CMDMkbuilder::CMDMkbuilder()
{
    SetName("mkbuilder");
    SetAccess(RANK_GOD);
}
BOOL CMDMkbuilder::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Player* target = NULL;

    if (!args.size())
        {
            mobile->Message(MSG_ERROR,"You must specify the person that you would like to make a builder.\n");
            return false;
        }

    target=world->FindPlayer(args[0]);
    if (target==NULL)
        {
            mobile->Message(MSG_ERROR,"That person couldn't be found.");
            return false;
        }
    if (mobile == target)
        {
            mobile->Message(MSG_INFO, "You may not make yourself a builder.");
            return false;
        }
    if (BitIsSet(target->GetRank(),RANK_BUILDER))
        {
            mobile->Message(MSG_ERROR,"That person is already a builder.");
            return false;
        }

    target->SetRank(BitSet(mobile->GetRank(),RANK_BUILDER));
    target->Message(MSG_INFO,"You were made a builder.");
    mobile->Message(MSG_INFO, Capitalize(target->GetName())+" was made a builder.");
    return true;
}

CMDShutdown::CMDShutdown()
{
    SetName("shutdown");
    SetAccess(RANK_ADMIN);
}
BOOL CMDShutdown::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    world->SetRunning(false);
    return true;
}

CMDBan::CMDBan()
{
    SetName("ban");
    SetAccess(RANK_ADMIN);
}
BOOL CMDBan::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (args.size() != 1)
        {
            World*world = World::GetPtr();
            BanList* blist = world->GetServer()->GetBanList();
            std::vector<std::string>* addresses = new std::vector<std::string>();
            std::vector<std::string> header;

            int i = 0;
            for (i = 0; i < 5; i++)
                {
                    header.push_back("address");
                }

            blist->ListAddresses(addresses);
            mobile->Message(MSG_LIST, Columnize(addresses, 5, &header));
            delete addresses;
            return true;
        }

    World*world = World::GetPtr();
    BanList* blist = world->GetServer()->GetBanList();
    if (blist->AddressExists(args[0]))
        {
            mobile->Message(MSG_ERROR, "That address is already in the ban list.");
            return false;
        }
    if (!blist->AddAddress(args[0]))
        {
            mobile->Message(MSG_ERROR, "Malformed address.");
            return false;
        }

    mobile->Message(MSG_INFO, "Added.");
    return true;
}

CMDSilence::CMDSilence()
{
    SetName("silence");
    SetAccess(RANK_ADMIN);
}
BOOL CMDSilence::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Player* targ = NULL;
    World* world = World::GetPtr();

    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Syntax: silence <player>");
            return false;
        }

    targ = world->FindPlayer(args[0]);
    if (!targ)
        {
            mobile->Message(MSG_ERROR, "Could not find the specified player.");
            return false;
        }
    if (targ->HasAccess(RANK_ADMIN) || targ->HasAccess(RANK_GOD))
        {
            mobile->Message(MSG_ERROR, "That player should be demoted before you try to silence them.");
            return false;
        }

    if (BitIsSet(mobile->GetPflag(), PF_SILENCE))
        {
            mobile->Message(MSG_ERROR, "That player has already been silenced.");
            return false;
        }

    targ->SetPflag(BitSet(targ->GetPflag(), PF_SILENCE));
    mobile->Message(MSG_INFO, Capitalize(targ->GetName())+" was silenced.");
    world->WriteLog(Capitalize(targ->GetName())+" was silenced by "+Capitalize(mobile->GetName())+".");
    targ->Message(MSG_ERROR, "You were silenced by "+Capitalize(mobile->GetName())+".");

    return true;
}

CMDUnsilence::CMDUnsilence()
{
    SetName("unsilence");
    SetAccess(RANK_ADMIN);
}
BOOL CMDUnsilence::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Player* targ = NULL;
    World* world = World::GetPtr();

    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Syntax: unsilence <player>");
            return false;
        }

    targ = world->FindPlayer(args[0]);
    if (!targ)
        {
            mobile->Message(MSG_ERROR, "Could not find the specified player.");
            return false;
        }

    if (!BitIsSet(targ->GetPflag(), PF_SILENCE))
        {
            mobile->Message(MSG_INFO, "That player is not silenced.");
            return false;
        }

    targ->SetPflag(BitClear(mobile->GetPflag(), PF_SILENCE));
    mobile->Message(MSG_INFO, Capitalize(targ->GetName())+" has been unsilenced.");
    world->WriteLog(Capitalize(targ->GetName())+" was unsilenced by "+Capitalize(mobile->GetName())+".");
    targ->Message(MSG_INFO, "You were unsilenced by "+Capitalize(mobile->GetName())+".");

    return true;
}

CMDDisconnect::CMDDisconnect()
{
    SetName("disconnect");
    SetAccess(RANK_ADMIN);
}
BOOL CMDDisconnect::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Player* target = NULL;

    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Syntax: disconnect <player>");
            return false;
        }

    target=world->FindPlayer(args[0]);
    if (!target)
        {
            mobile->Message(MSG_ERROR, "I could not find that player.");
            return false;
        }
    if (mobile == target)
        {
            mobile->Message(MSG_ERROR, "If you really want to disconnect yourself, you could just use quit.");
            return false;
        }

    if ((BitIsSet(target->GetRank(), RANK_GOD)) || (BitIsSet(target->GetRank(), RANK_ADMIN)))
        {
            mobile->Message(MSG_ERROR, "You can't disconnect that person.");
            return false;
        }

    target->GetSocket()->Kill();
    return true;
}

CMDEcho::CMDEcho()
{
    SetName("echo");
    SetAccess(RANK_ADMIN);
}
BOOL CMDEcho::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();

    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Syntax: echo <message>");
            return false;
        }

    std::string msg = Explode(args);
    std::list<Player*>::iterator it, itEnd;
    std::list<Player*>* players = world->GetPlayers();

    itEnd = players->end();
    for (it = players->begin(); it != itEnd; ++it)
        {
            (*it)->Message(MSG_INFO, msg);
        }

    return true;
}

CMDPdelete::CMDPdelete()
{
    SetName("pdelete");
    SetAccess(RANK_ADMIN);
}
BOOL CMDPdelete::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
//  World* world = World::GetPtr();

    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Syntax: pdelete <message>");
            return false;
        }

    return true;
}

CMDSstatus::CMDSstatus()
{
    SetName("sstatus");
    AddAlias("sstat");
    SetAccess(RANK_ADMIN);
}
BOOL CMDSstatus::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::stringstream st;
    unsigned long long int updates = 0;
    unsigned long long int tu = 0; //total update time
    unsigned long long int ts = 0; //total sleep time.
    unsigned long long int commands = 0;
    unsigned long long int commandTime = 0;
    using std::setw;
    struct rusage usage;

    st << std::setprecision(3);
    getrusage(RUSAGE_SELF, &usage);
//cpu usage
    st << "userspace cpu usage: " << TimevalToString(&(usage.ru_utime)) << "." << std::endl;
    st << "Kernel cpu usage: " << TimevalToString(&(usage.ru_stime)) << "." << std::endl;
//memory:
    st << "Max rss: " << usage.ru_maxrss << "KB (" << (usage.ru_maxrss/1024.0F) << "mb)." << std::endl;
    st << Repete("-", 80) << std::endl;
//timing information
    updates = world->GetUpdates();
    tu = world->GetUpdateTime();
    ts = world->GetSleepTime();
    st << "Total updates: " << updates << std::endl;
    st << "Average work time: " << ((double)tu/(double)updates) / 1000.0F << "MS." << std::endl;
    st << "Average sleep time: " << ((double)ts / (double)updates) / 1000.0F << "MS." << std::endl;
    st << "Spent ~" << ((double)tu / (double)ts) * 100.0F  << "% time working." << std::endl;
    st << CalloutManager::GetInstance()->Profile() << std::endl;
    commands = world->GetCommands();
    commandTime = world->GetCommandTime();
    st << "Total commands executed (that succeeded): " << commands << std::endl;
    st << "Average command time: " << ((double)commandTime/commands)/1000 << " MS." << std::endl;
    mobile->Message(MSG_INFO, st.str());
    return true;
}

CMDForce::CMDForce()
{
    SetName("force");
    SetAccess(RANK_ADMIN);
}
BOOL CMDForce::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Player* target = NULL;
    std::string command;

    if (!args.size() || args.size() < 2)
        {
            mobile->Message(MSG_ERROR, "syntax: force <player> <command> [args]");
            return false;
        }

    target=world->FindPlayer(args[0]);
    if (target == mobile)
        {
            mobile->Message(MSG_ERROR, "Why would you need to force yourself to do that?");
            return false;
        }
    if (target == NULL)
        {
            mobile->Message(MSG_ERROR, "That player was not found.");
            return false;
        }
    if (BitIsSet(target->GetRank(),RANK_GOD))
        {
            mobile->Message(MSG_ERROR, "You probably shouldn't try to do that...");
            return false;
        }

    args.erase(args.begin());
    command = Explode(args);
    target->GetSocket()->AddCommand(command);
    return true;
}
