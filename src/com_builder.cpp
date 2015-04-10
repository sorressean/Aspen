#include <ctype.h>
#include <list>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include "com_builder.h"
#include "world.h"
#include "objectManager.h"
#include "player.h"
#include "zone.h"
#include "room.h"
#include "utils.h"
#include "baseObject.h"
#include "objectContainer.h"

void InitializeBuilderCommands()
{
    World* world = World::GetPtr();

    world->WriteLog("Initializing builder commands.");
    world->commands.AddCommand(new CMDZlist());
    world->commands.AddCommand(new CMDRlist());
    world->commands.AddCommand(new CMDDig());
    world->commands.AddCommand(new CMDVCreate());
    world->commands.AddCommand(new CMDVList());
    world->commands.AddCommand(new CMDMCreate());
    world->commands.AddCommand(new CMDMList());
    world->commands.AddCommand(new CMDMLoad());
    world->commands.AddCommand(new CMDAddComponent());
    world->commands.AddCommand(new CMDGoto());
    world->commands.AddCommand(new CMDZcreate());
    world->commands.AddCommand(new CMDAStats());
    world->commands.AddCommand(new CMDRcreate());
}

//zlist
CMDZlist::CMDZlist()
{
    SetAccess(RANK_BUILDER);
    SetName("zlist");
    SetType(CommandType::Builder);
}
void CMDZlist::Syntax(Player* mobile, int subcmd) const
{
    mobile->Message(MSG_INFO, "Syntax: zlist.");
    mobile->Message(MSG_INFO, "Shows a list of all zones.");
}
BOOL CMDZlist::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::stringstream st;
    std::vector<Zone*> zones;

    world->GetZones(&zones);
    if (!zones.size())
        {
            st << "No zones found.";
        }
    else
        {
            for (auto it: zones)
                {
                    st << (it->GetName()) << " (" << it->GetMinVnum() << "-" << it->GetMaxVnum() << ")" << "\n";
                }
        }

    mobile->Message(MSG_LIST,st.str());
    return true;
}

//rlist
CMDRlist::CMDRlist()
{
    SetName("rlist");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
void CMDRlist::Syntax(Player* mobile, int subcmd) const
{
    std::stringstream st;
    st << "Syntax: rlist [zone]>" << std::endl;
    st << "Lists all rooms in the specified zone, or the current zone if no argument is provided.";
    mobile->Message(MSG_INFO, st.str());
}
BOOL CMDRlist::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::stringstream st;
    std::vector<Room*> rooms;
    Zone* zone = nullptr;

    if (!args.size())
        {
            zone = mobile->GetLocation()->GetZone();
        }
    else
        {
            zone=world->GetZone(args[0]);
        }
    if (!zone)
        {
            mobile->Message(MSG_ERROR,"That zone doesn't exist.");
            return false;
        }

    zone->GetRooms(&rooms);
    for (auto it: rooms)
        {
            st << "[" << it->GetOnum() << "] " << it->GetName() << "\n";
        }
    st << "Found " << (int)rooms.size() << ((int)rooms.size()==0?"room":"rooms") << ".\n";
    mobile->Message(MSG_LIST,st.str());
    return true;
}

//dig
CMDDig::CMDDig()
{
    SetName("dig");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
void CMDDig::Syntax(Player* mobile, int subcmd) const
{
    std::stringstream st;
    st << "Syntax: dig <direction> [to <vnum]" << std::endl;
    st << "Digs from the current room to the specified direction. If a vnum is provided, exits will be created to and from the provided room.";
    mobile->Message(MSG_INFO, st.str());
}
BOOL CMDDig::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    std::stringstream st;
    Room* room = NULL;
    Room* location = (Room*)mobile->GetLocation();
    point p;
    VNUM toid = 0;
    Zone* zone = NULL;
    Exit* orig = NULL; //from current room to another room.
    Exit* other = NULL; //from other room to current.
    std::string dir;

//initial error checking and argument parsing
    if (!location)
        {
            mobile->Message(MSG_ERROR, "You can't dig here.");
            return false;
        }
    if (!args.size())
        {
            Syntax(mobile);
            return false;
        }

    if (args.size() > 1)
        {
            if (args.size() == 2)
                {
                    toid = atoi(args[1].c_str());
                    if (!toid)
                        {
                            mobile->Message(MSG_ERROR, "Invalid rnum given.");
                            return false;
                        }
                }
            else if (args.size() == 3)
                {
                    toid = atoi(args[1].c_str());
                    if (!toid)
                        {
                            mobile->Message(MSG_ERROR, "Invalid rnum given.");
                            return false;
                        }
                }
            else
                {
                    mobile->Message(MSG_ERROR,"Syntax: dig <direction> Creates another room in the direction you specified, and creates exits back and forth.\n"
                                    "dig <direction> <rnum> Creates an exit in the specified direction to the specified rnum.\n"
                                    "dig <direction> to <rnum> Creates an exit in the specified room to the specified rnum.");
                    return false;
                }
        }

    if (!IsValidExit(args[0]))
        {
            mobile->Message(MSG_ERROR, "That is an invalid direction.\nYou can dig <north|n, south|s, east|e, west|w,\nnortheast|ne, southeast|se, northwest|sw, or southwest|sw");
            return false;
        }
    if (location->ExitExists(GetDirectionByName(args[0])))
        {
            mobile->Message(MSG_ERROR, "That exit already exists.");
            return false;
        }

    dir=SwapExit(args[0]);

    if (!toid)
        {
            zone=location->GetZone();
            room = zone->AddRoom();
            st << "Created room " << room->GetOnum() << "\n";
        }
    else
        {
            room = omanager->GetRoom(toid);
            if (!room)
                {
                    mobile->Message(MSG_ERROR, "That rnum does not exist.");
                    return false;
                }
            st << "Found room " << room->GetOnum() << "\n";
        }

    p = *location->GetCoord();
    if (args[0] == "north" || args[0] == "n")
        {
            p.y++;
        }
    else if (args[0] == "south" || args[0] == "s")
        {
            p.y--;
        }
    else if (args[0] == "east" || args[0] == "e")
        {
            p.x++;
        }
    else if (args[0] == "west" || args[0] == "w")
        {
            p.x--;
        }
    else if(args[0] == "northeast" || args[0] == "ne")
        {
            p.y++;
            p.x++;
        }
    else if(args[0] == "northwest" || args[0] == "nw")
        {
            p.y++;
            p.x--;
        }
    else if(args[0] == "southeast" || args[0] == "se")
        {
            p.x++;
            p.y--;
        }
    else if(args[0] == "southwest" || args[0] == "sw")
        {
            p.y--;
            p.x--;
        }
    else if (args[0] == "up" || args[0] == "u")
        {
            p.z++;
        }
    else if (args[0] == "down" || args[0] == "d")
        {
            p.z--;
        }
    room->SetCoord(p);

    orig=new Exit(room->GetOnum());
    orig->SetDirection(GetDirectionByName(args[0]));
    location->AddExit(orig);
    st << "Added exit from " << location->GetName() << "(" << location->GetOnum() << ") to newly created room.\n";
    if (dir!="")
        {
            other=new Exit(location->GetOnum());
            other->SetDirection(GetDirectionByName(dir));
            room->AddExit(other);
            st << "Added exit from newly created room to " << location->GetName() << "(" << location->GetOnum() << ").\nDone.\n";
        }

    mobile->Message(MSG_INFO, st.str());
    return true;
}

CMDAStats::CMDAStats()
{
    SetName("astat");
    AddAlias("zstat");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDAStats::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Zone* area = NULL;
    if (!args.size())
        {
//we show the stats for the current area
            if (!mobile->GetLocation() || !mobile->GetLocation()->IsRoom())
                {
                    mobile->Message(MSG_ERROR, "You are not in a room.");
                    return false;
                }
            area = mobile->GetLocation()->GetZone();
            if (!area)
                {
                    mobile->Message(MSG_ERROR, "Your location does not have a zone attached.");
                    return false;
                }
            mobile->Message(MSG_LIST, Stats(mobile, area));
            return true;
        }
    return true;
}
std::string CMDAStats::Stats(Player* mobile, Zone* area)
{
    std::stringstream st;
    st << area->GetName() << std::endl;
    st << Repeat('-', 80) << std::endl;
    st << "Range: " << "Minimum Vnum: " << area->GetMinVnum() << " Maximum Vnum: " << area->GetMaxVnum() << std::endl;
    return st.str();
}

CMDVCreate::CMDVCreate()
{
    SetAccess(RANK_BUILDER);
    SetName("vcreate");
    SetType(CommandType::Builder);
}
BOOL CMDVCreate::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Zone* zone = NULL;
    ObjectContainer* location = NULL;
    StaticObject* vobj = NULL;
    std::stringstream st;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    try
        {
            vobj = zone->AddVirtual();
        }
    catch(std::runtime_error e)
        {
            mobile->Message(MSG_ERROR, e.what());
            return false;
        }

    st << "Created static object " << vobj->GetOnum() << ".";
    mobile->Message(MSG_INFO, st.str());
    Zone::SaveZones();
    return true;
}

CMDVList::CMDVList()
{
    SetAccess(RANK_BUILDER);
    SetName("vlist");
    SetType(CommandType::Builder);
}
BOOL CMDVList::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    using std::left;
    using std::setw;
    using std::endl;
    using std::right;

    Zone* zone = NULL;
    BaseObject* location = NULL;
    std::vector<StaticObject*> objects;
    std::vector<StaticObject*>::iterator it, itEnd;
    size_t count = 0;
    std::stringstream st;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    zone->GetVirtuals(&objects);
    count = objects.size();

    if (!count)
        {
            mobile->Message(MSG_INFO, "No virtuals found.");
            return false;
        }

    itEnd = objects.end();
    st << left << setw(10) << "vnum" << right << "name" << Repeat(" ", 20) << "Zone: " << zone->GetName() << endl;
    st << Repeat('-', 80) << endl;
    for (it = objects.begin(); it != itEnd; ++it)
        {
            st << left << setw(10) << (*it)->GetOnum() << right << (*it)->GetName() << endl;
        }
    st << Repeat('-', 80) << endl;
    st << count << (count == 1? "object" : "objects") << " found." << endl;
    mobile->Message(MSG_LIST, st.str());
    return true;
}

CMDMCreate::CMDMCreate()
{
    SetAccess(RANK_BUILDER);
    SetName("mcreate");
    SetType(CommandType::Builder);
}
BOOL CMDMCreate::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Zone* zone = NULL;
    ObjectContainer* location = NULL;
    Npc* npc = NULL;
    std::stringstream st;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    try
        {
            npc = zone->AddNpc();
        }
    catch(std::runtime_error e)
        {
            mobile->Message(MSG_ERROR, e.what());
            return false;
        }

    st << "Created NPC " << npc->GetOnum() << ".";
    mobile->Message(MSG_INFO, st.str());
    return true;
}

CMDMList::CMDMList()
{
    SetAccess(RANK_BUILDER);
    SetName("mlist");
    SetType(CommandType::Builder);
}
BOOL CMDMList::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    using std::left;
    using std::setw;
    using std::endl;
    using std::right;

    Zone* zone = NULL;
    ObjectContainer* location = NULL;
    std::vector<Npc*> npcs;
    std::vector<Npc*>::iterator it, itEnd;
    size_t count = 0;
    std::stringstream st;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    zone->GetNpcs(&npcs);
    count = npcs.size();

    if (!count)
        {
            mobile->Message(MSG_INFO, "No npcs found.");
            return true;
        }
    itEnd = npcs.end();
    st << left << setw(10) << "vnum" << right << "name" << endl;
    st << Repeat('-', 80) << endl;
    for (it = npcs.begin(); it != itEnd; ++it)
        {
            st << left << setw(10) << (*it)->GetOnum() << right << (*it)->GetName() << endl;
        }
    st << Repeat('-', 80) << endl;
    st << count << (count == 1? "npc" : "npcs") << " found." << endl;
    mobile->Message(MSG_LIST, st.str());
    return true;
}

CMDMLoad::CMDMLoad()
{
    SetName("mload");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDMLoad::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Zone* zone = NULL;
    ObjectContainer* location = NULL;
    Npc* npc = NULL;
    std::string onum;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    if (args.size() != 1)
        {
            mobile->Message(MSG_ERROR, "Syntax: mload <vnum>");
            return false;
        }

    onum = args[0];
    for(auto ch:onum)
        {
            if (!isdigit(ch))
                {
                    mobile->Message(MSG_ERROR, "Syntax: mload <vnum>");
                    return false;
                }
        }

    npc = zone->CreateNpc(atoi(onum.c_str()), (Room*)location);
    if (!npc)
        {
            mobile->Message(MSG_ERROR, "That NPC doesn't exist in the specified zone.");
            return false;
        }

    mobile->Message(MSG_INFO, "Npc created.");
    return true;
}

CMDAddComponent::CMDAddComponent()
{
    SetName("addcomponent");
    AddAlias("adc");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDAddComponent::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    Zone* zone = NULL;
    ObjectContainer* location = NULL;
    BaseObject* obj = nullptr;
    std::string onum;
    std::string component;
    Component* comp;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }


    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    if (args.size() != 3)
        {
            mobile->Message(MSG_ERROR, "Syntax: addcomponent <-m|-o> <vnum>.");
            return false;
        }
    if (args[0] != "-m" && args[0] != "-o")
        {
            mobile->Message(MSG_ERROR, "Syntax: addcomponent <-m|-o> <vnum>.");
            return false;
        }

    onum = args[1];
    for(auto ch:onum)
        {
            if (!isdigit(ch))
                {
                    mobile->Message(MSG_ERROR, "Syntax: addcomponent <-m|-o> <vnum>.");
                    return false;
                }
        }

    if (args[0] == "-m")
        {
            obj = zone->GetNpc(atoi(args[1].c_str()));
        }
    else
        {
            obj = zone->GetVirtual(atoi(args[1].c_str()));
        }

    comp = world->CreateComponent(component);
    if (comp == nullptr)
        {
            mobile->Message(MSG_ERROR, "That component does not exist.");
            return false;
        }

    if (!obj->AddComponent(comp))
        {
            mobile->Message(MSG_ERROR, "Could not add that component.");
            if (comp)
                {
                    delete comp;
                }
            return false;
        }

    mobile->Message(MSG_INFO, "Component added.");
    return true;
}

CMDGoto::CMDGoto()
{
    SetName("goto");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDGoto::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    Room* room = nullptr;
    VNUM toid = 0;

    if (args.size() != 1)
        {
            mobile->Message(MSG_ERROR,"Syntax: goto <room number> teleports you to that room.\n");
            return false;
        }

    toid = atoi(args[0].c_str());
    if (!toid)
        {
            mobile->Message(MSG_ERROR, "Invalid rnum given.");
            return false;
        }

    room = omanager->GetRoom(toid);
    if (!room)
        {
            mobile->Message(MSG_ERROR, "That rnum does not exist.");
            return false;
        }

    mobile->MoveTo(room);
    mobile->Message(MSG_INFO, room->DoLook(mobile));

    return true;
}

CMDZcreate::CMDZcreate()
{
    SetName("zcreate");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDZcreate::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::stringstream st;
    Room* room = nullptr;
    int  min=0;
    int max = 0;
    Zone* newZone = nullptr;
    point p;
    std::vector<Zone*> zones;

    if (args.size() != 3)
        {
            mobile->Message(MSG_ERROR,"Syntax: zcreate <name of zone> <min> <max> creates a new zone with the name and rooms available in the range from min to max\n");
            return false;
        }

    min  = atoi(args[1].c_str());
    max   = atoi(args[2].c_str());

    if (!max || !min || min > max || min <= 0)
        {
            mobile->Message(MSG_ERROR,"Invalid num range. Syntax: zcreate <name of zone> <min> <max> creates a new zone with the name and rooms available in the range from min to max\n");
            return false;
        }

    if (world->GetZone(args[0]) != nullptr)
        {
            mobile->Message(MSG_ERROR,"Error: zone with name already  exists.\n");
            return false;
        }

    world->GetZones(&zones);
    for (auto it: zones)
        {
            if((min >= it->GetMinVnum() && min <= it->GetMaxVnum()) || (max  >= it->GetMinVnum() && max  <= it->GetMaxVnum()))
                {
                    mobile->Message(MSG_ERROR,"Error: Min and max values overlap with " + it->GetName() + " zone \n");
                    return false;
                }
        }

    newZone = new Zone();
    newZone->SetName(args[0]);
    newZone->SetRange(min, max);
    room = newZone->AddRoom();
    room->SetName("An empty room");
    room->SetCoord(p);
    world->AddZone(newZone);

    st << "Created zone with name " <<  newZone->GetName() << " with one empty room."  << "\n";
    mobile->Message(MSG_INFO, st.str());

    return true;
}

CMDRcreate::CMDRcreate()
{
    SetAccess(RANK_BUILDER);
    SetName("rcreate");
    SetType(CommandType::Builder);
}
BOOL CMDRcreate::Execute(const std::string &verb, Player* mobile, std::vector<std::string> &args, int subcmd)
{
    Zone* zone = NULL;
    ObjectContainer* location = NULL;
    Room* room = NULL;
    std::stringstream st;

    location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    try
        {
            room = zone->AddRoom();
        }
    catch (std::runtime_error e)
        {
            mobile->Message(MSG_ERROR, e.what());
            return false;
        }

    st << "Created Room " << room->GetOnum() << ".";
    mobile->Message(MSG_INFO, st.str());
    return true;
}