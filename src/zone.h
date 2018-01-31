#pragma once
#ifndef ZONE_H
#define ZONE_H
#include <tinyxml2.h>
#include <unordered_map>
#include "mud.h"
#include "conf.h"
#include "event.h"
#include "room.h"
#include "npc.h"

/*
*Used to control access on zone edits.
*/
enum zacl_t
{
    ZACL_EDIT = 1<<0,
    ZACL_RESET = 1<<1,
    ZACL_CLONE = 1<<2,
    ZACL_VIEW = 1<<3,
};

struct zacl
{
    std::string person; //uid;
    zacl_t access;
};

/**
*Zones are used to allow a builder to define an area, which will contain rooms, (a list of Room objects).
*This is useful for determining where a mob can wander (not allowing it outside it's zone), etc.
*/
class Room;
class Zone
{
    std::string _name;
    std::vector<StaticObject*> _virtualobjs; //a vector of pointers to all virtual objs.
    std::list<Entity*> _objects;
    std::vector<Room*> _roomobjs; //a vector to all current room objects;
    std::vector<Npc*> _mobobjs;
    FLAG _flags;
    time_t _creation;
    time_t _opened; //when was the zone opened for public?
    time_t _lastreset; //when did we last reset?
    std::string _resetmsg;
    unsigned int _resetfreq;
    range _vnumrange;
public:
    Zone();
    virtual ~Zone();
    /**
    *Retrieves the set name of the zone.
    *\return The name of the zone, or "" if a name hasn't been set.
    */
    std::string GetName() const;
    /**
    *Sets the name of the zone object.
    *\param name The name of the zone.
    */
    void SetName(const std::string &name);
    /*
    *Sets the min and max range for vnums on the zone.
    *Min: the minimum range.
    *Max: the maximum range.
    */
    void SetRange(int min, int max);
    int  GetMinVnum();
    int  GetMaxVnum();
    /*----------------------------------------------------------------------------*/
    /**
    *Adds the specified room.
    *\param num [in] the VNUM of the room to add.
    *\return True if the room was added successfully, false otherwise.
    *\note The zone pointer on the Room object is set to the zone to which the room was added.
    */
    Room* AddRoom();
    Room* AddRoom(VNUM num);
    BOOL RemoveRoom(VNUM num);
    void GetRooms(std::vector<Room*> *rooms);
    BOOL RoomExists(VNUM num);
    StaticObject* AddVirtual();
    BOOL RemoveVirtual(VNUM num);
    StaticObject* GetVirtual(VNUM num);
    void GetVirtuals(std::vector<StaticObject*>* objects);
    BOOL VirtualExists(VNUM num);
    Npc* AddNpc();
    BOOL RemoveNpc(VNUM num);
    Npc* GetNpc(VNUM num);
    BOOL NpcExists(VNUM num);
    void GetNpcs(std::vector<Npc*>* npcs);
    Npc* CreateNpc(VNUM num, Room* origin);
    void Update();
    virtual void Serialize(tinyxml2::XMLElement* root);
    virtual void Deserialize(tinyxml2::XMLElement* zone);

    static BOOL SaveZones();
    static BOOL LoadZones();
    static EVENT(Autosave);
    static EVENT(Shutdown);
};

BOOL InitializeZones();
#endif
