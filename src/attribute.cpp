#include <string>
#include "mud.h"
#include "conf.h"
#include "attribute.h"
#include "world.h"

Attribute::Attribute(int type, int application, int mod, int id):
    _type(type), _apply(application), _mod(mod), _id(id)
{
}

int Attribute::GetType() const
{
    return _type;
}
int Attribute::GetApply() const
{
    return _apply;
}
int Attribute::GetModifier() const
{
    return _mod;
}
int Attribute::GetId() const
{
    return _id;
}
