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
#include "objectContainer.h"

class Room;
class Entity:public ObjectContainer
{
protected:
    ObjectContainer* _location;
    StaticObject* _parent;
    Uuid _uuid;
    std::string _short;
public:
    Entity();
    virtual ~Entity();

    virtual std::string GetShort() const;
    virtual void SetShort(const std::string &s);

    /*
    *Returns the location of this object.
    *Return: A pointer to the entity in which this object is located, or NULL if the object does not have  a parent location.
    */
    ObjectContainer* GetLocation() const;
    /*
    *Sets the location of the object
    *Param: [in] The object to move the entity to.
    */
    void SetLocation(ObjectContainer* location);
    /*
    *Moves this object to the target
    *Param: [in] the destination where the object needs to go.
    *Return: true on success, false on failure.
    */
    virtual BOOL MoveTo(ObjectContainer* targ);
    /*
        *Called when an object leaves another object.
        *Param: [in] The object that is being removed.
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
    BOOL FromRoom();
    BOOL ToRoom(Room* room);
    virtual void Initialize();
    Uuid& GetUuid();
    /*
    *GetStats is used for ostat and other commands.
    *Param: [in] the mobile requesting the stats.
    */
    virtual std::string Identify(Player* mob);
    virtual BOOL IsObject() const;
};

bool InitializeEntityOlcs();
#endif
