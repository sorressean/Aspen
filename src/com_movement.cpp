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
    world->commands.AddCommand(new CMDMove(DIR_NORTH));
    world->commands.AddCommand(new CMDMove(DIR_SOUTH));
    world->commands.AddCommand(new CMDMove(DIR_EAST));
    world->commands.AddCommand(new CMDMove(DIR_WEST));
    world->commands.AddCommand(new CMDMove(DIR_NORTHEAST));
    world->commands.AddCommand(new CMDMove(DIR_NORTHWEST));
    world->commands.AddCommand(new CMDMove(DIR_SOUTHEAST));
    world->commands.AddCommand(new CMDMove(DIR_SOUTHWEST));
    world->commands.AddCommand(new CMDMove(DIR_UP));
    world->commands.AddCommand(new CMDMove(DIR_DOWN));
}

static bool DoMove(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    ExitDirection direction = (ExitDirection)subcmd;
    Room* room = nullptr;
    Room* dest = nullptr;
    Exit* exit = nullptr;

    room=(Room*)mobile->GetLocation();
    if (!room)
        {
            mobile->Message(MSG_ERROR,"There are no exits here.");
            return false;
        }
    if (!room->ExitExists(direction))
        {
            mobile->Message(MSG_ERROR,"You can't go that way.");
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
            mobile->Message(MSG_ERROR, "Something is preventing you from leaving the room.");
            return false;
        }

    dest = omanager->GetRoom(exit->GetTo());
    mobile->MoveTo(dest);
    dest->events.CallEvent("OnEnter", NULL, (void*)dest);
    mobile->Message(MSG_INFO, dest->DoLook(mobile));
    return true;
}

CMDMove::CMDMove(int direction)
{
    SetSubcmd(direction);
    SetType(CommandType::Movement);
    switch(direction)
        {
        case DIR_NORTH:
            SetName("north");
            AddAlias("n");
            break;
        case DIR_SOUTH:
            SetName("south");
            AddAlias("s");
            break;
        case DIR_EAST:
            SetName("east");
            AddAlias("e");
            break;
        case DIR_WEST:
            SetName("west");
            AddAlias("w");
            break;
        case DIR_NORTHEAST:
            SetName("northeast");
            AddAlias("ne");
            break;
        case DIR_SOUTHEAST:
            SetName("southeast");
            AddAlias("se");
            break;
        case DIR_SOUTHWEST:
            SetName("southwest");
            AddAlias("sw");
            break;
        case DIR_NORTHWEST:
            SetName("northwest");
            AddAlias("nw");
            break;
        case DIR_UP:
            SetName("up");
            AddAlias("u");
            break;
        case DIR_DOWN:
            SetName("down");
            AddAlias("d");
            break;
        default:
            World* world = World::GetPtr();
            world->WriteLog("Invalid direction specified.", ERR);
        }
}
bool CMDMove::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return DoMove(verb, mobile, args, subcmd);
}
