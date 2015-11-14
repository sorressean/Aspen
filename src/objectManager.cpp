#include "mud.h"
#include "objectManager.h"
#include "baseObject.h"
#include "staticObject.h"
#include "entity.h"
#include "npc.h"
#include "room.h"
#include "world.h"
#include "zone.h"

Zone* ObjectManager::FindZone(VNUM num)
{
    World* world = World::GetPtr();
    std::vector<Zone*> zones;

    world->GetZones(&zones);
    for (auto it: zones)
        {
            if (num >= it->GetMinVnum() || num <= it->GetMaxVnum())
                {
                    return it;
                }
        }

    return nullptr;
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

Entity* ObjectManager::CreateObject(VNUM num)
{
    Entity* object = nullptr;
    StaticObject* virt = nullptr;

    virt = GetVirtual(num);
    if (!virt)
        {
            return nullptr;
        }

    object = virt->Create();
    return object;
}
BOOL ObjectManager::VirtualExists(VNUM num)
{
    return (GetVirtual(num)? true : false);
}
StaticObject* ObjectManager::GetVirtual(VNUM num)
{
    Zone* zone = nullptr;

    if (num <= 0)
        {
            return nullptr;
        }
    zone = FindZone(num);
    if (!zone)
        {
            return nullptr;
        }

    return zone->GetVirtual(num);
}

BOOL ObjectManager::RoomExists(VNUM num)
{
    return (GetRoom(num)? true : false);
}
Room* ObjectManager::GetRoom(VNUM num)
{
    Zone* zone = nullptr;

    if (num <= 0)
        {
            return nullptr;
        }
    zone = FindZone(num);
    if (!zone)
        {
            return nullptr;
        }

    return zone->GetRoom(num);
}

bool ObjectManager::NpcExists(VNUM num)
{
    return (GetNpc(num)? true : false);
}
Npc*  ObjectManager::GetNpc(VNUM num)
{
    Zone* zone = nullptr;

    if (num <= 0)
        {
            return nullptr;
        }
    zone = FindZone(num);
    if (!zone)
        {
            return nullptr;
        }

    return zone->GetNpc(num);
}
