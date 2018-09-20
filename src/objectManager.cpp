#include "mud.h"
#include "objectManager.h"
#include "baseObject.h"
#include "staticObject.h"
#include "log.h"
#include "entity.h"
#include "npc.h"
#include "room.h"
#include "world.h"

void ObjectManager::Update()
{
    for (auto npc: _npcs)
        {
            (npc.second)->Update();
        }
}
Entity* ObjectManager::CreateObject(VNUM obj)
{
    Entity* object = nullptr;
    StaticObject* virt = nullptr;

    if (obj != 0 && !VirtualExists(obj))
        {
            WriteLog(SeverityLevel::Fatal, "Tried to create virtual with nonexistant vnum");
            return nullptr;
        }

    virt = _objects[obj];
    object = virt->Create();
    return object;
}
bool ObjectManager::RecycleObject(ObjectContainer* obj)
{
    World* world = World::GetPtr();
    ObjectContainer* location = nullptr;
    StaticObject* vobj = nullptr;

    if(!obj)
        {
            return false;
        }

//we recursively recycle everything in contents.
    std::list<Entity*>* contents = obj->GetContents();

//recursively delete objects held by the object being deleted.
    for (auto it :*contents)
        {
            RecycleObject(it);
        }

//check to see if this object is stored in another. If so, we need to remove it.

    if (obj->IsObject())
        {
            location = ((Entity*)obj)->GetLocation();
            if (location)
                {
                    location->ObjectLeave((Entity*)obj);
                }

//recycle this instance with it's virtual object, if there is one.
            if (obj->GetOnum())
                {
                    vobj = GetVirtual(((Entity*)obj)->GetOnum());
                    if (vobj)
                        {
                            vobj->Recycle((Entity*)obj);
                        }
                }
        }

    world->events.CallEvent("ObjectDestroyed", nullptr, (void*)obj);
    delete obj;
    return true;
}

bool ObjectManager::AddVirtual(StaticObject* obj)
{
    if (!obj)
        {
            return false;
        }

    VNUM num = obj->GetOnum();
    if (VirtualExists(num))
        {
            return false;
        }

    _objects[num] = obj;
    return true;
}
bool ObjectManager::VirtualExists(VNUM num)
{
    return (_objects.count(num)? true:false);
}
StaticObject* ObjectManager::GetVirtual(VNUM num)
{
    if (!VirtualExists(num))
        {
            return nullptr;
        }

    return _objects[num];
}
bool ObjectManager::RemoveVirtual(VNUM num)
{
    StaticObject* virt = GetVirtual(num);
    if (!virt)
        {
            return false;
        }

    virt->RecycleContents();
    _objects.erase(num);
    return true;
}
VNUM ObjectManager::GetFreeVirtualVnum(VNUM min, VNUM max, VNUM num)
{
    if (num != 0)
        {
            return (!VirtualExists(num) ? num : 0);
        }

    for (int i = min; i <= max; ++i)
        {
            if (!VirtualExists(i))
                {
                    return i;
                }
        }

    return 0;
}

bool ObjectManager::AddRoom(Room* room)
{
    if (!room)
        {
            return false;
        }

    VNUM num = room->GetOnum();
    if (RoomExists(num))
        {
            return false;
        }

    _rooms[num] = room;
    return true;
}
bool ObjectManager::RemoveRoom(VNUM num)
{
    Room* room = nullptr;

    room = GetRoom(num);
    if (!room)
        {
            return false;
        }

    _rooms.erase(num);
    return true;
}
bool ObjectManager::RoomExists(VNUM num)
{
    return _rooms.count(num)? true : false;
}
Room* ObjectManager::GetRoom(VNUM num)
{
    if (!RoomExists(num))
        {
            return nullptr;
        }

    return _rooms[num];
}
VNUM ObjectManager::GetFreeRoomVnum(VNUM min, VNUM max, VNUM num)
{
    if (num)
        {
            return (RoomExists(num) ? 0 : num);
        }

    for (int i = min; i <= max; ++i)
        {
            if (!RoomExists(i))
                {
                    return i;
                }
        }

    return 0;
}

bool ObjectManager::AddNpc(Npc* mob)
{
    if (!mob)
        {
            return false;
        }

    VNUM num = mob->GetOnum();
    if (NpcExists(num))
        {
            return false;
        }

    _npcs[num] = mob;
    return true;
}
bool ObjectManager::RemoveNpc(VNUM num)
{
    Npc* mob = GetNpc(num);
    if (!mob)
        {
            return false;
        }

    _npcs.erase(num);
    return true;
}
bool ObjectManager::NpcExists(VNUM num)
{
    return  (_npcs.count(num) ? true : false);
}
Npc*  ObjectManager::GetNpc(VNUM num)
{
    if (!NpcExists(num))
        {
            return nullptr;
        }

    return _npcs[num];
}
VNUM ObjectManager::GetFreeNpcVnum(VNUM min, VNUM max, VNUM num)
{
    if (!num)
        {
            return (NpcExists(num) ? 0 : num);
        }

    for (int i = min; i <= max; ++i)
        {
            if (!NpcExists(i))
                {
                    return i;
                }
        }

    return 0;
}
