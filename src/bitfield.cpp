#include "mud.h"
#include "conf.h"
#include "bitfield.h"
#include "utils.h"

Bitfield::Bitfield(int& flag, int position):_flag(flag), _position(position)
{
}
Bitfield::Bitfield(const Bitfield& bf):_flag(bf._flag), _position(bf._position)
{
}
Bitfield::~Bitfield()
{
}

bool Bitfield::GetField() const
{
    return BitIsSet(_flag, _position);
}

bool Bitfield::operator ==(bool f) const
{
    return GetField() == f;
}
bool Bitfield::operator ==(const Bitfield& bf) const
{
    return ((bf._flag == _flag) && (bf._position == _position));
}

bool Bitfield::operator !=(bool f) const
{
    return (GetField() != f);
}
bool Bitfield::operator !=(const Bitfield& bf) const
{
    return ((bf._flag != _flag) || (bf._position != _position));
}

Bitfield& Bitfield::operator =(bool t)
{
    if (t)
        {
            BitSet(_flag, _position);
        }
    else
        {
            BitClear(_flag, _position);
        }

    return *this;
}
Bitfield& Bitfield::operator =(const Bitfield& bf)
{
    _flag = bf._flag;
    _position = bf._position;

    return *this;
}
