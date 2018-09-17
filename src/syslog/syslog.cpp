#include <sstream>
#include "syslog.h"
#include "../mud.h"
#include "../log.h"
#include "../player.h"
#include "../optionManager.h"
#include "../optionMeta.h"
#include "../variant.h"

bool InitializeSyslog()
{
#ifdef MODULE_SYSLOG
    OptionMeta* ometa = nullptr;
    World* world = World::GetPtr();
    OptionManager* omanager = world->GetOptionManager();

    WriteLog("Initializing syslog.");

    ometa = new OptionMeta();
    ometa->SetName("enabled");
    ometa->SetHelp("Determines whether you will receive any alog notifications.");
    ometa->SetSection(OptionSection::Alog);
    ometa->SetAccess(RANK_BUILDER|RANK_ADMIN|RANK_GOD);
    ometa->SetValue(1);
    ometa->SetExpected(VAR_INT);
    ometa->SetToggle(true);
    omanager->AddOption(ometa);

    world->events.GetEvent("PlayerConnect")->Add(SYSLOG_PlayerConnect);
    world->events.GetEvent("PlayerDisconnect")->Add(SYSLOG_PlayerDisconnect);
    world->AddChannel(new SyslogChannel("syslog","",RANK_ADMIN),false);
#endif

    return true;
}

#ifdef MODULE_SYSLOG
SyslogChannel::SyslogChannel(const std::string &name,const std::string &alias,const FLAG access):
    Channel(name, alias, access)
{
}


bool SyslogChannel::CanBroadcastWithoutListening(Player* mobile) const
{
    return true;
}

EVENT(SYSLOG_PlayerConnect)
{
    World* world = World::GetPtr();

    Player* mobile=(Player*)caller;
    sockaddr_in* addr=mobile->GetSocket()->GetAddr();
    std::stringstream st;
    st << "Connected from: " << mobile->GetSocket()->GetHost() << " (" << inet_ntoa(addr->sin_addr) << ")";
    world->FindChannel("syslog")->Broadcast(mobile,st.str(),false);
    st.str("");
    st << mobile->GetName() << mobile->GetSocket()->GetHost() << " (" << inet_ntoa(addr->sin_addr) << ").";
    WriteLog(st.str());
}

EVENT(SYSLOG_PlayerDisconnect)
{
    World* world = World::GetPtr();

    Player* mobile=(Player*)caller;
    world->FindChannel("syslog")->Broadcast(mobile,"Disconnected",false);
    WriteLog(mobile->GetName()+" disconnected.");
}
#endif
