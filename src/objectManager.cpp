#include "mud.h"
#include "conf.h"
#include "objectManager.h"
#include "baseObject.h"
#include "staticObject.h"
#include "entity.h"
#include "npc.h"
#include "room.h"
#include "world.h"

ObjectManager::ObjectManager()
{
}
ObjectManager::~ObjectManager()
{
}

void ObjectManager::Update()
{
    for (auto npc: _npcs)
        {
            (npc.second)->Update();
        }
}
Entity* ObjectManager::CreateObject(VNUM obj)
{
    Entity* object = NULL;
    StaticObject* virt = NULL;
    World* world = World::GetPtr();

    if (obj != 0 && !VirtualExists(obj))
        {
            world->WriteLog("Tried to create virtual with nonexistant vnum", ERR);
            return NULL;
        }

    virt = _objects[obj];
    object = virt->Create();
    return object;
}
BOOL ObjectManager::RecycleObject(ObjectContainer* obj)
{
    World* world = World::GetPtr();
    ObjectContainer* location = NULL;
    StaticObject* vobj = NULL;

    if(!obj)
        {
            return false;
        }

//we recursively recycle everything in contents.
    std::list<Entity*>::iterator it, itEnd;
    std::list<Entity*>* contents = obj->GetContents();

//recursively delete objects held by the object being deleted.
    itEnd = contents->end();
    for (it = contents->begin(); it != itEnd; ++it)
        {
            RecycleObject((*it));
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

    world->events.CallEvent("ObjectDestroyed", NULL, (void*)obj);
    delete obj;
    return true;
}

BOOL ObjectManager::AddVirtual(StaticObject* obj)
{
    VNUM num = 0;
    if (!obj)
        {
            return false;
        }

    num = obj->GetOnum();
    if (VirtualExists(num))
        {
            return false;
        }

    _objects[num] = obj;
    return true;
}
BOOL ObjectManager::VirtualExists(VNUM num)
{
    return (_objects.count(num)? true:false);
}
StaticObject* ObjectManager::GetVirtual(VNUM num)
{
    if (!VirtualExists(num))
        {
            return NULL;
        }

    return _objects[num];
}
BOOL ObjectManager::RemoveVirtual(VNUM num)
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

BOOL ObjectManager::AddRoom(Room* room)
{
    VNUM num = 0;
    if (!room)
        {
            return false;
        }

    num = room->GetOnum();
    if (RoomExists(num))
        {
            return false;
        }

    _rooms[num] = room;
    return true;
}
BOOL ObjectManager::RemoveRoom(VNUM num)
{
    Room* room = NULL;

    room = GetRoom(num);
    if (!room)
        {
            return false;
        }

    _rooms.erase(room->GetOnum());
    return true;
}
BOOL ObjectManager::RoomExists(VNUM num)
{
    return _rooms.count(num)? true : false;
}
Room* ObjectManager::GetRoom(VNUM num)
{
    if (!RoomExists(num))
        {
            return NULL;
        }

    return _rooms[num];
}

BOOL ObjectManager::AddNpc(Npc* mob)
{
    VNUM num = 0;

    if (!mob)
        {
            return false;
        }

    num = mob->GetOnum();
    if (NpcExists(num))
        {
            return false;
        }

    _npcs[num] = mob;
    return true;
}
BOOL ObjectManager::RemoveNpc(VNUM num)
{
    Npc* mob = NULL;

    mob = GetNpc(num);
    if (!mob)
        {
            return false;
        }

    _npcs.erase(mob->GetOnum());
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
            return NULL;
        }

    return _npcs[num];
}
