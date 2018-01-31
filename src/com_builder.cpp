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

bool CMDZlist::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::stringstream st;
    std::vector<Zone*> zones;

    world->GetZones(&zones);
    if (!zones.size())
        {
            st << "No zones found.";
            mobile->Message(MSG_ERROR, st.str());
            return true;
        }

    for (auto it: zones)
        {
            st << (it->GetName()) << " (" << it->GetMinVnum() << "-" << it->GetMaxVnum() << ")" << "\n";
        }

    mobile->Message(MSG_LIST,st.str());
    return true;
}


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

bool CMDRlist::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    std::stringstream st;
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

    std::vector<Room*> rooms;
    zone->GetRooms(&rooms);
    for (auto it: rooms)
        {
            st << "[" << it->GetOnum() << "] " << it->GetName() << "\n";
        }

    st << "Found " << (int)rooms.size() << ((int)rooms.size()==0?"room":"rooms") << ".\n";
    mobile->Message(MSG_LIST,st.str());
    return true;
}


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

bool CMDDig::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
//initial error checking and argument parsing
    if (!args.size())
        {
            Syntax(mobile);
            return false;
        }

    Room* location = (Room*)mobile->GetLocation();
    if (!location)
        {
            mobile->Message(MSG_ERROR, "You can't dig here.");
            return false;
        }

    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    std::stringstream st;
    VNUM toid = 0;

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

    std::string dir=SwapExit(args[0]);

    Room* room = nullptr;
    if (!toid)
        {
            Zone* zone = nullptr;

            zone=location->GetZone();
            try
                {
                    room = zone->AddRoom();
                }
            catch(std::runtime_error& e)
                {
                    mobile->Message(MSG_ERROR, e.what());
                    return true;
                }
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

    Exit* orig=new Exit(room->GetOnum());
    orig->SetDirection(GetDirectionByName(args[0]));
    location->AddExit(orig);
    st << "Added exit from " << location->GetName() << "(" << location->GetOnum() << ") to newly created room.\n";
    if (dir!="")
        {
            Exit* other=new Exit(location->GetOnum());
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

bool CMDAStats::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Zone* area = nullptr;

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

bool CMDVCreate::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    ObjectContainer* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    StaticObject* vobj = nullptr;
    try
        {
            vobj = zone->AddVirtual();
        }
    catch(std::runtime_error e)
        {
            mobile->Message(MSG_ERROR, e.what());
            return false;
        }

    std::stringstream st;
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

bool CMDVList::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    using std::left;
    using std::setw;
    using std::endl;
    using std::right;

    BaseObject* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    std::vector<StaticObject*> objects;
    zone->GetVirtuals(&objects);
    size_t count = objects.size();

    if (!count)
        {
            mobile->Message(MSG_INFO, "No virtuals found.");
            return false;
        }

    std::stringstream st;
    st << left << setw(10) << "vnum" << right << "name" << Repeat(" ", 20) << "Zone: " << zone->GetName() << endl;
    st << Repeat('-', 80) << endl;
    for (auto it: objects)
        {
            st << left << setw(10) << it->GetOnum() << right << it->GetName() << endl;
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

bool CMDMCreate::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    ObjectContainer* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    Npc* npc = nullptr;
    try
        {
            npc = zone->AddNpc();
        }
    catch(std::runtime_error e)
        {
            mobile->Message(MSG_ERROR, e.what());
            return false;
        }

    std::stringstream st;
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

bool CMDMList::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    using std::left;
    using std::setw;
    using std::endl;
    using std::right;


    ObjectContainer* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    std::vector<Npc*> npcs;
    zone->GetNpcs(&npcs);

    size_t count = npcs.size();
    if (!count)
        {
            mobile->Message(MSG_INFO, "No npcs found.");
            return true;
        }

    std::stringstream st;
    st << left << setw(10) << "vnum" << right << "name" << endl;
    st << Repeat('-', 80) << endl;
    for (auto it: npcs)
        {
            st << left << setw(10) << it->GetOnum() << right << it->GetName() << endl;
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

bool CMDMLoad::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    ObjectContainer* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
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

    std::string onum = args[0];
    for(auto ch: onum)
        {
            if (!isdigit(ch))
                {
                    mobile->Message(MSG_ERROR, "Syntax: mload <vnum>");
                    return false;
                }
        }

    Npc* npc = zone->CreateNpc(atoi(onum.c_str()), (Room*)location);
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

bool CMDAddComponent::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    ObjectContainer* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
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

    std::string onum = args[1];
    for(auto ch:onum)
        {
            if (!isdigit(ch))
                {
                    mobile->Message(MSG_ERROR, "Syntax: addcomponent <-m|-o> <vnum>.");
                    return false;
                }
        }

    BaseObject* obj = nullptr;
    if (args[0] == "-m")
        {
            obj = zone->GetNpc(atoi(args[1].c_str()));
        }
    else
        {
            obj = zone->GetVirtual(atoi(args[1].c_str()));
        }

    World* world = World::GetPtr();
    std::string component = args[1];
    Component* comobj = nullptr;
    comobj = world->CreateComponent(component);
    if (comobj == nullptr)
        {
            mobile->Message(MSG_ERROR, "That component does not exist.");
            return false;
        }

    if (!obj->AddComponent(comobj))
        {
            mobile->Message(MSG_ERROR, "Could not add that component.");
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

bool CMDGoto::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (args.size() != 1)
        {
            mobile->Message(MSG_ERROR,"Syntax: goto <room number> teleports you to that room.\n");
            return false;
        }

    VNUM toid = atoi(args[0].c_str());
    if (!toid)
        {
            mobile->Message(MSG_ERROR, "Invalid rnum given.");
            return false;
        }

    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    Room* room = omanager->GetRoom(toid);
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

bool CMDZcreate::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (args.size() != 3)
        {
            mobile->Message(MSG_ERROR,"Syntax: zcreate <name of zone> <min> <max> creates a new zone with the name and rooms available in the range from min to max\n");
            return false;
        }

    World* world = World::GetPtr();
    int min  = atoi(args[1].c_str());
    int max   = atoi(args[2].c_str());
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

    std::vector<Zone*> zones;
    world->GetZones(&zones);
    for (auto it: zones)
        {
            if((min >= it->GetMinVnum() && min <= it->GetMaxVnum()) || (max  >= it->GetMinVnum() && max  <= it->GetMaxVnum()))
                {
                    mobile->Message(MSG_ERROR,"Error: Min and max values overlap with " + it->GetName() + " zone \n");
                    return false;
                }
        }

    Zone* newZone = new Zone();
    newZone->SetName(args[0]);
    newZone->SetRange(min, max);
    Room* room = newZone->AddRoom();
    room->SetName("An empty room");
    world->AddZone(newZone);

    std::stringstream st;
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

bool CMDRcreate::Execute(const std::string &verb, Player* mobile, std::vector<std::string> &args, int subcmd)
{
    ObjectContainer* location = mobile->GetLocation();
    if (!location || !location->IsRoom())
        {
            mobile->Message(MSG_ERROR, "You are not in a room.");
            return false;
        }

    Zone* zone = location->GetZone();
    if (!zone)
        {
            mobile->Message(MSG_ERROR, "The room you are in does not have a zone.");
            return false;
        }

    Room* room = nullptr;
    try
        {
            room = zone->AddRoom();
        }
    catch (std::runtime_error e)
        {
            mobile->Message(MSG_ERROR, e.what());
            return false;
        }

    std::stringstream st;
    st << "Created Room " << room->GetOnum() << ".";
    mobile->Message(MSG_INFO, st.str());
    return true;
}
