#pragma once
#include "mud.h"
#include "conf.h"
#include "attribute.h"

/**
This class is used to apply affects to any living object.
It can hold a duration (number of game ticks) as well as the attributes to apply.
\todo Allow affects to store multiple attributes.
\todo allow an ID or some pointer to be set so that affects and their timers can be destroyed when a living leaves the game.
\todo add affect removal code on game leave.
*/
class Affect
{
    Attribute* _attribute;
    int _duration;
public:
/**
Creates an affect, providing an attribute and the duration (in game ticks).
*/
    Affect(int duration, Attribute* attr);
    Affect();
    ~Affect();
    int GetDuration() const;
    void SetDuration(int duration);
    Attribute* GetAttribute() const;
    void SetAttribute(Attribute* attr);
};
