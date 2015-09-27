/**
*Contains: Affect
*
*The affect class is used to add affects to players and npcs.
*This is done via the Attribute class.
*/

#pragma once
#include "mud.h"
#include "conf.h"
#include "attribute.h"

/**
* Affect is used to apply affects to any living object.
*
*It can hold a duration (number of game ticks) as well as the attributes to apply.
* @todo Allow affects to store multiple attributes.
* @todo allow an ID or some pointer to be set so that affects and their timers can be destroyed when a living leaves the game.
* @todo add affect removal code on game leave.
*/
class Affect
{
    int _duration;
    Attribute* _attribute;
public:
    /**
    * Creates an affect, providing an attribute and the duration (in game ticks).
    * @param [in] duration the duration of time, in seconds for the affect to apply.
    * @param [in] attr A pointer to the attribute object to add.
    */
    Affect(int duration, Attribute* attr);
    /**
    * Creates a blank attribute.
    */
    Affect();
    ~Affect();
    /**
    * Returns the total duration of the affect.
    */
    int GetDuration() const;
    /**
    * Sets the total duration of the affect.
    * @param [in] duration The duration the affect will last.
    */
    void SetDuration(int duration);
    /**
    * Returns a pointer to the attribute associated with this affect.
    */
    Attribute* GetAttribute() const;
    /**
    * Sets the attribute to be associated with this affect.
    * @param [in] attr the attribute for the object.
    */
    void SetAttribute(Attribute* attr);
};
