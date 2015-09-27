#include "mud.h"
#include "conf.h"
#include "attribute.h"
#include "affect.h"

Affect::Affect(int duration, Attribute* attr):
    _duration(duration), _attribute(attr)
{
}
Affect::Affect():
    _duration(0), _attribute(nullptr)
{
}
Affect::~Affect()
{
    if (_attribute)
        {
            delete _attribute;
        }
}

int Affect::GetDuration() const
{
    return _duration;
}
void Affect::SetDuration(int duration)
{
    _duration = duration;
}

Attribute* Affect::GetAttribute() const
{
    return _attribute;
}
void Affect::SetAttribute(Attribute* attr)
{
    _attribute = attr;
}
