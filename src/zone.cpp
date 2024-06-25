#include <tinyxml2.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include "zone.h"
#include "world.h"
#include "log.h"
#include "objectManager.h"
#include "room.h"
#include "staticObject.h"
#include "event.h"
#include "utils.h"
#include "serializationHelpers.h"

static int zone_saves;

Zone::Zone()
{
    _vnumrange.min = 0;
    _vnumrange.max = 0;
    _resetfreq = 240;
    _resetmsg = "With a pop, the area resets around you.";
    _lastreset=time(NULL);
    _creation = time(NULL);
    _opened = 0;
    _flags = 0;
}
Zone::~Zone()
{
    for(auto it: _virtualobjs)
        {
            delete it;
        }

    for (auto it: _objects)
        {
            delete it;
        }

    for (auto it: _roomobjs)
        {
            delete it;
        }

    for (auto it: _mobobjs)
        {
            delete it;
        }
}

std::string Zone::GetName() const
{
    return _name;
}
void Zone::SetName(const std::string &name)
{
    _name=name;
}
void Zone::SetRange(int min, int max)
{
    _vnumrange.min = min;
    _vnumrange.max = max;
}
int  Zone::GetMinVnum()
{
    return _vnumrange.min;
}
int  Zone::GetMaxVnum()
{
    return _vnumrange.max;
}
Room* Zone::AddRoom()
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    Room* room = NULL;
    VNUM num = omanager->GetFreeRoomVnum(_vnumrange.min, _vnumrange.max);

    if (!num)
        {
            throw(std::runtime_error("No more vnums available"));
        }

    room = new Room();
    room->SetOnum(num);
    room->SetZone(this);
    _roomobjs.push_back(room);
    omanager->AddRoom(room);
    return room;
}
Room* Zone::AddRoom(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    Room* room = NULL;
    if (!omanager->GetFreeRoomVnum(_vnumrange.min, _vnumrange.max, num))
        {
            throw(std::runtime_error("No more vnums available"));
        }

    room = new Room();
    room->SetOnum(num);
    room->SetZone(this);
    _roomobjs.push_back(room);
    omanager->AddRoom(room);
    return room;
}
bool Zone::RemoveRoom(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    std::vector<Room*>::iterator it, itEnd;

    if (omanager->RoomExists(num))
        {
            itEnd = _roomobjs.end();
            for (it = _roomobjs.begin(); it != itEnd; ++it)
                {
                    if ((*it)->GetOnum() == num)
                        {
                            _roomobjs.erase(it);
                            omanager->RemoveRoom(num);
                            return true;
                        }
                }
        }

    return false;
}
void Zone::GetRooms(std::vector<Room*> *rooms)
{
    std::copy(_roomobjs.begin(), _roomobjs.end(), std::back_inserter(*rooms));
}
bool Zone::RoomExists(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    if (num < _vnumrange.min || num > _vnumrange.max)
        {
            return false;
        }

    return omanager->RoomExists(num);
}

StaticObject* Zone::AddVirtual()
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    StaticObject* obj = NULL;
    VNUM num = omanager->GetFreeVirtualVnum(_vnumrange.min, _vnumrange.max);

    if (!num)
        {
            throw(std::runtime_error("No more vnums available"));
        }

    obj = new StaticObject();
    obj->SetOnum(num);
    _virtualobjs.push_back(obj);
    omanager->AddVirtual(obj);
    return obj;
}
bool Zone::RemoveVirtual(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    std::vector<StaticObject*>::iterator it, itEnd;

    itEnd = _virtualobjs.end();
    for (it = _virtualobjs.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetOnum() == num)
                {
                    _virtualobjs.erase(it);
                    omanager->RemoveVirtual(num);
                    return true;
                }
        }

    return false;
}
StaticObject* Zone::GetVirtual(VNUM num)
{
    World*world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();

    if (VirtualExists(num))
        {
            return omanager->GetVirtual(num);
        }

    return nullptr;
}
void Zone::GetVirtuals(std::vector<StaticObject*>* objects)
{
    std::copy(_virtualobjs.begin(), _virtualobjs.end(), std::back_inserter(*objects));
}
bool Zone::VirtualExists(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    if (num > _vnumrange.max || num < _vnumrange.min)
        {
            return false;
        }

    return omanager->VirtualExists(num);
}

Npc* Zone::AddNpc()
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    Npc* mob = nullptr;
    VNUM num = omanager->GetFreeNpcVnum(_vnumrange.min, _vnumrange.max);

    if (!num)
        {
            throw(std::runtime_error("No more vnums available"));
        }

    mob = new Npc();
    mob->SetOnum(num);
    _mobobjs.push_back(mob);
    omanager->AddNpc(mob);
    return mob;
}
bool Zone::RemoveNpc(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    std::vector<Npc*>::iterator it, itEnd;

    itEnd = _mobobjs.end();
    for (it = _mobobjs.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetOnum() == num)
                {
                    _mobobjs.erase(it);
                    omanager->RemoveNpc(num);
                    return true;
                }
        }

    return false;
}
Npc* Zone::GetNpc(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    if (NpcExists(num))
        {
            return omanager->GetNpc(num);
        }

    return nullptr;
}
void Zone::GetNpcs(std::vector<Npc*>* npcs)
{
    std::copy(_mobobjs.begin(), _mobobjs.end(), std::back_inserter(*npcs));
}
bool Zone::NpcExists(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    if (num > _vnumrange.max || num < _vnumrange.min)
        {
            return false;
        }

    return omanager->NpcExists(num);
}
Npc* Zone::CreateNpc(VNUM num, Room* origin)
{
    Npc* templ = GetNpc(num);
    Npc* ret = nullptr;

    if (!templ)
        {
            return nullptr;
        }

    ret = new Npc();
    templ->Copy(ret);
    ret->SetOrigin(origin);
    ret->EnterGame();
    ret->Initialize();
    if (!ret->MoveTo(origin))
        {
            delete ret;
            return nullptr;
        }

    return ret;
}

Entity* Zone::CreateObject(VNUM num)
{
    const auto templ = GetVirtual(num);
    if (!templ)
        {
            return nullptr;
        }

    auto object = templ->Create();
    return object;
}

void Zone::Update()
{
}

void Zone::Serialize(tinyxml2::XMLElement* root)
{
    root->SetAttribute("name", _name.c_str());
    root->SetAttribute("flags", _flags);
    root->SetAttribute("creation", (unsigned int)_creation);
    root->SetAttribute("opened", (unsigned int)_opened);
    root->SetAttribute("resetmsg", _resetmsg.c_str());
    root->SetAttribute("resetfreq", (unsigned int)_resetfreq);
    root->SetAttribute("minvnum", _vnumrange.min);
    root->SetAttribute("maxvnum", _vnumrange.max);

    SerializeList<StaticObject, std::vector<StaticObject*>>("vobjes", "vobj", root, _virtualobjs);
    SerializeList<Room, std::vector<Room*>>("rooms", root, _roomobjs);
    SerializeList<Npc, std::vector<Npc*>>("npcs", "npc", root, _mobobjs);
}
void Zone::Deserialize(tinyxml2::XMLElement* zone)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();

    _name = zone->Attribute("name");
    _flags = (int)zone->IntAttribute("flags");
    _creation = (time_t)zone->IntAttribute("creation");
    _opened = (time_t)zone->IntAttribute("opened");
    _resetmsg = zone->Attribute("resetmsg");
    _resetfreq = (time_t)zone->Attribute("resetfreq");
    _vnumrange.min = zone->IntAttribute("minvnum");
    _vnumrange.max = zone->IntAttribute("maxvnum");

    DeserializeList<StaticObject, std::vector<StaticObject*> >(zone, "vobjes", _virtualobjs);
    DeserializeList<Room, std::vector<Room*> >(zone, "rooms", _roomobjs);
    DeserializeList<Npc, std::vector<Npc*> >(zone, "npcs", _mobobjs);

    for (auto it: _roomobjs)
        {
            omanager->AddRoom(it);
            it->SetZone(this);
        }

    for (auto it: _virtualobjs)
        {
            omanager->AddVirtual(it);
        }

    for (auto it: _mobobjs)
        {
            omanager->AddNpc(it);
        }
}

bool InitializeZones()
{
    World* world = World::GetPtr();
    struct stat FInfo;

    WriteLog("Initializing areas.");
    if ((stat(AREA_STARTFILE,&FInfo))!=-1)
        {
            Zone::LoadZones();
        }
    else
        {
#ifdef NO_INIT_DEFAULTS
            WriteLog("No area file exists, and NO_INIT_DEFAULTS was enabled, exiting.");
            return false;
        }
#else
            WriteLog("No area found, creating default.");
//no zones and rooms exist, create a first zone/room.
            Zone*zone=new Zone();
            if (!zone)
                {
                    return false;
                }
            zone->SetName("Start");
            if (!world->AddZone(zone))
                {
                    return false;
                }
            zone->SetRange(1, 100);
            Room* room = zone->AddRoom(ROOM_START);
            room->SetName("A blank room");
            if (!Zone::SaveZones())
                {
                    return false;
                }
        }
#endif

    zone_saves = 0;
    world->events.AddCallback("WorldPulse",
                              std::bind(&Zone::Autosave, std::placeholders::_1, std::placeholders::_2));
    world->events.AddCallback("Shutdown",
                              std::bind(&Zone::Shutdown, std::placeholders::_1, std::placeholders::_2));
    world->events.AddCallback("Copyover",
                              std::bind(&Zone::Shutdown, std::placeholders::_1, std::placeholders::_2));
    return true;
}

bool Zone::SaveZones()
{
    World* world = World::GetPtr();
    std::vector<Zone*> *zones=new std::vector<Zone*>();
    std::string path;

    world->GetZones(zones);
    if (zones->size())
        {
            for (Zone* zone:*zones)
                {
                    tinyxml2::XMLDocument doc;
                    tinyxml2::XMLElement* zelement = doc.NewElement("zone");
                    doc.InsertEndChild(doc.NewDeclaration());
                    zone->Serialize(zelement);
                    doc.InsertEndChild(zelement);
                    path = AREA_DIR+zone->GetName();
                    Lower(path);
                    doc.SaveFile(path.c_str());
                }
        }
    delete zones;
    return true;
}
bool Zone::LoadZones()
{
    World* world = World::GetPtr();
    Zone* zone=nullptr;
    tinyxml2::XMLElement* root = nullptr;
    dirent* cdir = nullptr;
    DIR* dir = opendir(AREA_DIR);

    if (!dir)
        {
            return false;
        }

    while ((cdir = readdir(dir)))
        {
            if (cdir->d_name[0] == '.')
                {
                    continue;
                }

            tinyxml2::XMLDocument doc;
            if (doc.LoadFile((AREA_DIR+std::string(cdir->d_name)).c_str()) != tinyxml2::XML_NO_ERROR)
                {
                    closedir(dir);
                    return false;
                }

            zone = new Zone();
            root = doc.FirstChildElement("zone");
            if (!root)
                {
                    closedir(dir);
                    return false;
                }

            zone->Deserialize(root);
            world->AddZone(zone);
        }

    closedir(dir);
    return true;
}

CEVENT(Zone, Autosave)
{
    zone_saves++;
    if (zone_saves >= 100)
        {
            Zone::SaveZones();
            zone_saves = 0;
        }
}
CEVENT(Zone, Shutdown)
{
    Zone::SaveZones();
}
