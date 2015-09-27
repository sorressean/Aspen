/**
* Contains: Attribute
*
* The attribute class holds information about changes and bonuses to a player or NPC.
*/
#pragma once
#include <string>
#include "mud.h"
#include  "conf.h"
#include "event.h"
#include "tableManager.hpp"

/**
* Attributes are used for adding or removing bonuses or stats from a player.
*/
class Attribute
{
    int _type;
    int _apply;
    int _mod;
    int _id;
public:
    /**
    * Constructs an attribute object.
    *
    * @param [in] type The type of the attribute. This is an index from the attribute table.
    * @param [in] application Attribute specific information, is this applied from a piece of equipment, etc.
    * @param [in] mod The modifier for the attribute of type.
    * @param [in] id The iD of the specific attribute, used for removal purposes. Should only be provided by the attribute framework.
    */
    Attribute(int ttype, int application, int mod, int id=0);
    ~Attribute() = default;
    /**
    * Returns the type of the attribute.
    */
    int GetType() const;
    /**
    * Returns information on how this attribute is applied.
    *
    * This is attribute specific, see documentation related to individual attributes.
    */
    int GetApply() const;
    /**
    * Returns the modifier for the attribute.
    */
    int GetModifier() const;
    /**
    * Returns the ID of the attribute.
    */
    int GetId() const;
};

/**
* The table of attribute types.
*/
typedef TableManager<int> AttributeTable;

/**
* The event callback, used to free up attribute resources.
*/
EVENT(CleanupAttributes);
/**
* Initializes all attributes, including the attribute table.
*/
void InitializeAttributes();
/**
* A helper function, which allows you to retrieve a pointer to the attribute lookup table.
*/
AttributeTable* GetAttributeTable();
