#pragma once
#include "mud.h"
#include "conf.h"
#include "attribute.h"

class Affect
{
    Attribute* _attribute;
    int _duration;
public:
    Affect(int duration, Attribute* attr);
    Affect();
    ~Affect();
    int GetDuration() const;
    void SetDuration(int duration);
    Attribute* GetAttribute() const;
    void SetAttribute(Attribute* attr);
};
