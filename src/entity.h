/**
* Contains: Entity
*
* Entities are generally what all in-game objects inherit from.
*/
#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include <tinyxml.h>
#include <list>
#include <map>
#include <string>
#include "mud.h"
#include "event.h"
#include "eventManager.h"
#include "eventargs.h"
#include "variant.h"
#include "command.h"
#include "property.h"
#include "baseObject.h"
#include "uuid.h"
#include "objectContainer.h"

class Entity:public ObjectContainer
{
protected:
    ObjectContainer* _location;
    StaticObject* _parent;
    Uuid _uuid;
    std::string _short;
public:
    Entity();
    virtual ~Entity() = default;

    /**
    * Returns the short description of the entity.
    *
    * The short description is what players will see when the entity is not in their inventory.
    * For example, a sword's short description might be:
    * A long sword lies here.
    */
    virtual std::string GetShort() const;
    /**
    * Sets the entities short description.
    *
    * @param [in] s the new short description.
    */
    virtual void SetShort(const std::string &s);

    /**
    * Returns the location of this object.
    *
    * @return the entities location, or nullptr if the object does not have  a parent location.
    */
    ObjectContainer* GetLocation() const;
    /**
    * Sets the location of the object
    *
    * @param [in] location The object to move the entity to.
    */
    void SetLocation(ObjectContainer* location);

    /**
    * Gets the static object associated with this entity.
    */
    StaticObject* GetParent() const;
    /**
    * Sets the static object associated with this object.
    *
    * @param [in] parent the new parent for this object.
    */
    void SetParent(StaticObject* parent);

    /**
    * Attempts to moves this object to the target
    *
    * @param [in] targ the destination where the object needs to go.
    *Return: true on success, false if the object can not move.
    */
    virtual BOOL MoveTo(ObjectContainer* targ);
    /**
    * Removes this object from the room it is in, if possible.
    */
    BOOL FromRoom();

    /**
    * Initializes the entity after loaded.
    */
    virtual void Initialize();
    /**
    * Gets the UUID associated with the entity.
    */
    Uuid& GetUuid();

    /**
    * Returns stats for this object.
    *
    * @param [in] mob the player requesting identification.
    */
    virtual std::string Identify(Player* mob);
    virtual BOOL IsObject() const;

    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
};

/**
* Initializes OLCs attached to this entity.
*/
bool InitializeEntityOlcs();
#endif
