#pragma once
#include <string>
#include "mud.h"
#include  "conf.h"

class Attribute
{
    int _type;
    int _apply;
    int _mod;
    int _id;
public:
    Attribute(int ttype, int application, int mod, int id=0);
    ~Attribute() = default;
    int GetType() const;
    int GetApply() const;
    int GetModifier() const;
    int GetId() const;
};
