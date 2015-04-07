#include <tinyxml.h>
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
    VNUM num = 0;

    if (_rnums.empty())
        {
            CalculateVnumRanges();
            if (_rnums.empty())
                {
                    throw(std::runtime_error("No more vnums available"));
                }
        }

    room = new Room();
    num = _rnums.top();
    _rnums.pop();
    room->SetOnum(num);
    room->SetZone(this);
    _rooms[num] = room;
    _roomobjs.push_back(room);
    omanager->AddRoom(room);
    return room;
}
Room* Zone::AddRoom(VNUM num)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();

    VNUM cur = 0;
    std::stack<VNUM> temp;
    Room* room = NULL;
    BOOL found = false;

    if (_rnums.empty())
        {
            CalculateVnumRanges();
            if (_rnums.empty())
                {
                    throw(std::runtime_error("No more vnums available"));
                }
        }

    room = new Room();
    while (!_rnums.empty())
        {
            cur = _rnums.top();
            _rnums.pop();
            if (cur == num)
                {
                    found = true;
                    continue;
                }
            temp.push(cur);
        }
    if (!found)
        {
            throw(std::runtime_error("That num wasn't found."));
        }

    _rnums = temp;
    room->SetOnum(num);
    room->SetZone(this);
    _rooms[num] = room;
    _roomobjs.push_back(room);
    omanager->AddRoom(room);
    return room;
}
BOOL Zone::RemoveRoom(VNUM num)
{
    std::vector<Room*>::iterator it, itEnd;

    if (_rooms.count(num))
        {
            itEnd = _roomobjs.end();
            for (it = _roomobjs.begin(); it != itEnd; ++it)
                {
                    if ((*it)->GetOnum() == num)
                        {
                            _roomobjs.erase(it);
                            delete _rooms[num];
                            _rooms.erase(num);
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
BOOL Zone::RoomExists(VNUM num)
{
    return (_rooms.count(num) ? true : false);
}

StaticObject* Zone::AddVirtual()
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    StaticObject* obj = NULL;
    VNUM num = 0;

    if (_onums.empty())
        {
            CalculateVnumRanges();
            if (_onums.empty())
                {
                    throw(std::runtime_error("No more vnums available"));
                }
        }

    obj = new StaticObject();
    num = _onums.top();
    _onums.pop();
    obj->SetOnum(num);
    _virtuals[num] = obj;
    _virtualobjs.push_back(obj);
    omanager->AddVirtual(obj);
    return obj;
}
BOOL Zone::RemoveVirtual(VNUM num)
{
    std::vector<StaticObject*>::iterator it, itEnd;

    itEnd = _virtualobjs.end();
    for (it = _virtualobjs.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetOnum() == num)
                {
                    _virtualobjs.erase(it);
                    delete _virtualobjs[num];
                    _virtuals.erase(num);
                    return true;
                }
        }

    return false;
}
StaticObject* Zone::GetVirtual(VNUM num)
{
    if (VirtualExists(num))
        {
            return _virtuals[num];
        }

    return nullptr;
}
void Zone::GetVirtuals(std::vector<StaticObject*>* objects)
{
    for (auto it: _virtuals)
        {
            objects->push_back(it.second);
        }
}
BOOL Zone::VirtualExists(VNUM num)
{
    return (_virtuals.count(num) ? true : false);
}

Npc* Zone::AddNpc()
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    Npc* mob = nullptr;
    VNUM num = 0;

    if (_mnums.empty())
        {
            CalculateVnumRanges();
            if (_mnums.empty())
                {
                    throw(std::runtime_error("No more vnums available"));
                }
        }

    mob = new Npc();
    num = _mnums.top();
    _mnums.pop();
    mob->SetOnum(num);
    _mobs[num] = mob;
    _mobobjs.push_back(mob);
    omanager->AddNpc(mob);
    return mob;
}
BOOL Zone::RemoveNpc(VNUM num)
{
    std::vector<Npc*>::iterator it, itEnd;

    itEnd = _mobobjs.end();
    for (it = _mobobjs.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetOnum() == num)
                {
                    _mobobjs.erase(it);
                    delete _mobs[num];
                    _mobs.erase(num);
                    return true;
                }
        }

    return false;
}
Npc* Zone::GetNpc(VNUM num)
{
    if (NpcExists(num))
        {
            return _mobs[num];
        }

    return nullptr;
}
void Zone::GetNpcs(std::vector<Npc*>* npcs)
{
    std::copy(_mobobjs.begin(), _mobobjs.end(), std::back_inserter(*npcs));
}
BOOL Zone::NpcExists(VNUM num)
{
    return (_mobs.count(num) ? true : false);
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
void Zone::CalculateVnumRanges()
{
    int i = 0; //for counter

//find all empty virtual objects.
    for (i = _vnumrange.max; i >= _vnumrange.min; i--)
        {
            if (!VirtualExists(i))
                {
                    if (_onums.size() == VNUMKEEP)
                        {
                            break;
                        }
                    _onums.push(i);
                }
        }

//find all empty room objects.
    for (i = _vnumrange.max; i >= _vnumrange.min; i--)
        {
            if (!RoomExists(i))
                {
                    if (_rnums.size() == VNUMKEEP)
                        {
                            break;
                        }
                    _rnums.push(i);
                }
        }

//find all empty npc objects
    for (i = _vnumrange.max; i >= _vnumrange.min; i--)
        {
            if (_mnums.size() == VNUMKEEP)
                {
                    break;
                }
            if (!NpcExists(i))
                {
                    _mnums.push(i);
                }
        }
}

void Zone::Update()
{
}

void Zone::Serialize(TiXmlElement* root)
{
    root->SetAttribute("name", _name.c_str());
    root->SetAttribute("flags", _flags);
    root->SetAttribute("creation", _creation);
    root->SetAttribute("opened", _opened);
    root->SetAttribute("resetmsg", _resetmsg.c_str());
    root->SetAttribute("resetfreq", _resetfreq);
    root->SetAttribute("minvnum", _vnumrange.min);
    root->SetAttribute("maxvnum", _vnumrange.max);

    SerializeList<StaticObject, std::vector<StaticObject*>, std::vector<StaticObject*>::iterator>("vobjes", "vobj", root, _virtualobjs);
    SerializeList<Room, std::vector<Room*>, std::vector<Room*>::iterator>("rooms", "room", root, _roomobjs);
    SerializeList<Npc, std::vector<Npc*>, std::vector<Npc*>::iterator>("npcs", "npc", root, _mobobjs);
}
void Zone::Deserialize(TiXmlElement* zone)
{
    unsigned int u = 0;
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();

    _name = zone->Attribute("name");
    zone->Attribute("flags", &_flags);
    zone->Attribute("creation", &u);
    _creation = u;
    zone->Attribute("opened", &u);
    _opened = u;
    _resetmsg = zone->Attribute("resetmsg");
    zone->Attribute("resetfreq", &_resetfreq);
    zone->Attribute("minvnum", &(_vnumrange.min));
    zone->Attribute("maxvnum", &(_vnumrange.max));

    DeserializeList<StaticObject, std::vector<StaticObject*> >(zone, "vobjes", _virtualobjs);
    DeserializeList<Room, std::vector<Room*> >(zone, "rooms", _roomobjs);
    DeserializeList<Npc, std::vector<Npc*> >(zone, "npcs", _mobobjs);

    for (auto it: _roomobjs)
        {
            omanager->AddRoom(it);
            _rooms[it->GetOnum()] = it;
            it->SetZone(this);
        }

    for (auto it: _virtualobjs)
        {
            _virtuals[it->GetOnum()] = it;
            omanager->AddVirtual(it);
        }

    for (auto it: _mobobjs)
        {
            omanager->AddNpc(it);
            _mobs[it->GetOnum()] = it;
        }
}

BOOL InitializeZones()
{
    World* world = World::GetPtr();
    point p;
    struct stat FInfo;

    world->WriteLog("Initializing areas.");
    if ((stat(AREA_STARTFILE,&FInfo))!=-1)
        {
            Zone::LoadZones();
        }
    else
        {
#ifdef NO_INIT_DEFAULTS
            world->WriteLog("No area file exists, and NO_INIT_DEFAULTS was enabled, exiting.");
            return false;
        }
#else
            world->WriteLog("No area found, creating default.");
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
            zone->CalculateVnumRanges();
            Room* room = zone->AddRoom(ROOM_START);
            room->SetName("A blank room");
            room->SetCoord(p);
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

BOOL Zone::SaveZones()
{
    World* world = World::GetPtr();
    std::vector<Zone*> *zones=new std::vector<Zone*>();
    std::string path;

    world->GetZones(zones);
    if (zones->size())
        {
            for (Zone* zone:*zones)
                {
                    TiXmlDocument doc;
                    TiXmlElement* zelement = new TiXmlElement("zone");
                    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
                    doc.LinkEndChild(decl);
                    zone->Serialize(zelement);
                    doc.LinkEndChild(zelement);
                    path = AREA_DIR+zone->GetName();
                    Lower(path);
                    doc.SaveFile(path.c_str());
                }
        }
    delete zones;
    return true;
}
BOOL Zone::LoadZones()
{
    World* world = World::GetPtr();
    Zone* zone=nullptr;
    TiXmlElement* root = nullptr;
    TiXmlNode* node = nullptr;
    dirent* cdir = NULL;
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

            TiXmlDocument doc((std::string(AREA_DIR)+cdir->d_name).c_str());
            if (!doc.LoadFile())
                {
                    closedir(dir);
                    return false;
                }

            zone = new Zone();
            node = doc.FirstChild("zone");
            if (!node)
                {
                    closedir(dir);
                    return false;
                }

            root = node->ToElement();
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
