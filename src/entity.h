/*
*This comprises our game objects.
*/
#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include <tinyxml.h>
#include <list>
#include <map>
#include <string>
#include "mud.h"
#include "conf.h"
#include "event.h"
#include "eventManager.h"
#include "eventargs.h"
#include "variant.h"
#include "command.h"
#include "property.h"
#include "baseObject.h"
#include "uuid.h"

class Room;
class Entity:public BaseObject
{
protected:
    std::list <Entity*> _contents;
    Entity* _location;
    StaticObject* _parent;
Uuid _uuid;
public:
    Entity();
    virtual ~Entity();
    /*
    *Returns the location of this object.
    *Return: A pointer to the entity in which this object is located, or NULL if the object does not have  a parent location.
    */
    Entity* GetLocation() const;
    /*
    *Sets the location of the object
    *Param: [in] The object to move the entity to.
    */
    void SetLocation(Entity* location);
    /*
    *Returns a pointer to a list of objects that the object contains.
    *Return: A pointer to an std::list of pointers to objects.
    */
    std::list<Entity*>* GetContents();
    /*
    *Gets the object's id
    *Return: the id of the object.
    */
    StaticObject* GetParent() const;
    void SetParent(StaticObject* parent);
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
    /*
    *Called when the player looks at the object.
    *Param: [in] a pointer to the player who looked at the object.
    *Returns: The Object description.
    */
    virtual std::string DoLook(Player* mobile);
    /*
    *Checks to see if the object can enter the object on which this function is being called on.
    *Return: True on success, false on failure.
    */
    virtual BOOL CanReceive(Entity* obj) const;
    /*
    *Moves this object to the target
    *Param: [in] the destination where the object needs to go.
    *Return: true on success, false on failure.
    */
    virtual BOOL MoveTo(Entity* targ);
    /*
    *Called when an object is moved into another object.
    *For example, if you put a sword in a container, the ObjectEnter would be called on the container with the sword being passed as an argument.
    *Param: [in] the object entering the current object.
    */
    virtual void ObjectEnter(Entity* obj);
    /*
    *Called when an object leaves another object.
    *Param: [in] The object that is being removed.
    */
    virtual void ObjectLeave(Entity* obj);
    BOOL FromRoom();
    BOOL ToRoom(Room* room);
    virtual BOOL IsNpc() const;
    virtual BOOL IsPlayer() const;
    virtual BOOL IsLiving() const;
    virtual BOOL IsRoom() const;
virtual void Initialize();
Uuid& GetUuid();
    /*
    *GetStats is used for ostat and other commands.
    *Param: [in] the mobile requesting the stats.
    */
    virtual std::string Identify(Player* mob);
};

bool InitializeEntityOlcs();
#endif
