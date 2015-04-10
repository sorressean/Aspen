#include <string>
#include "com_movement.h"
#include "player.h"
#include "world.h"
#include "objectManager.h"
#include "room.h"
#include "exit.h"

void InitializeMovementCommands()
{
    World* world = World::GetPtr();

    world->WriteLog("Initializing movement commands.");
    world->commands.AddCommand(new CMDNorth());
    world->commands.AddCommand(new CMDSouth());
    world->commands.AddCommand(new CMDEast());
    world->commands.AddCommand(new CMDWest());
    world->commands.AddCommand(new CMDNortheast());
    world->commands.AddCommand(new CMDNorthwest());
    world->commands.AddCommand(new CMDSoutheast());
    world->commands.AddCommand(new CMDSouthwest());
    world->commands.AddCommand(new CMDUp());
    world->commands.AddCommand(new CMDDown());
}

BOOL DoMove(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();

    ExitDirection direction = (ExitDirection)subcmd;
    Room* room = NULL;
    Room* dest = NULL;
    Exit* exit = NULL;

    room=(Room*)mobile->GetLocation();
    if (!room)
        {
            mobile->Message(MSG_ERROR,"There are no exits here.");
            return false;
        }

    if (!room->ExitExists(direction))
        {
            mobile->Message(MSG_ERROR,"You can't go that way!");
            return false;
        }

    exit=room->GetExit(direction);
    if (!exit->CanEnter(mobile))
        {
            mobile->Message(MSG_ERROR,"Something prevents you from going that direction.");
            return false;
        }
    if (!mobile->FromRoom())
        {
            mobile->Message(MSG_ERROR, "You couldn't be moved.");
            return false;
        }

    dest = omanager->GetRoom(exit->GetTo());
    mobile->MoveTo(dest);
    dest->events.CallEvent("OnEnter", NULL, (void*)dest);

    mobile->Message(MSG_INFO, (omanager->GetRoom(exit->GetTo()))->DoLook(mobile));
    return true;
}

CMDNorth::CMDNorth()
{
    SetName("north");
    AddAlias("n");
    SetSubcmd(DIR_NORTH);
    SetType(CommandType::Movement);
}
BOOL CMDNorth::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDSouth::CMDSouth()
{
    SetName("south");
    AddAlias("s");
    SetSubcmd(DIR_SOUTH);
    SetType(CommandType::Movement);
}
BOOL CMDSouth::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDEast::CMDEast()
{
    SetName("east");
    AddAlias("e");
    SetSubcmd(DIR_EAST);
    SetType(CommandType::Movement);
}
BOOL CMDEast::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDWest::CMDWest()
{
    SetName("west");
    AddAlias("w");
    SetSubcmd(DIR_WEST);
    SetType(CommandType::Movement);
}
BOOL CMDWest::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDNortheast::CMDNortheast()
{
    SetName("northeast");
    AddAlias("ne");
    SetSubcmd(DIR_NORTHEAST);
    SetType(CommandType::Movement);
}
BOOL CMDNortheast::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDSoutheast::CMDSoutheast()
{
    SetName("southeast");
    AddAlias("se");
    SetSubcmd(DIR_SOUTHEAST);
    SetType(CommandType::Movement);
}
BOOL CMDSoutheast::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDSouthwest::CMDSouthwest()
{
    SetName("southwest");
    AddAlias("sw");
    SetSubcmd(DIR_SOUTHWEST);
    SetType(CommandType::Movement);
}
BOOL CMDSouthwest::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDNorthwest::CMDNorthwest()
{
    SetName("northwest");
    AddAlias("nw");
    SetSubcmd(DIR_NORTHWEST);
    SetType(CommandType::Movement);
}
BOOL CMDNorthwest::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDUp::CMDUp()
{
    SetName("up");
    AddAlias("u");
    SetSubcmd(DIR_UP);
    SetType(CommandType::Movement);
}
BOOL CMDUp::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}

CMDDown::CMDDown()
{
    SetName("down");
    AddAlias("d");
    SetSubcmd(DIR_DOWN);
    SetType(CommandType::Movement);
}
BOOL CMDDown::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}
