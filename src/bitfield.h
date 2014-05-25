/*
*This is a bit field returned by flag's getters and setters.
*/
#pragma once
#include "mud.h"
#include "conf.h"

class Bitfield
{
    int& _flag;
    int _position;
public:
    Bitfield(int& flag, int position);
    Bitfield(const Bitfield& bf);
    ~Bitfield();
    bool GetField() const;
    bool operator ==(bool f) const;
    bool operator ==(const Bitfield& bf) const;
    bool operator !=(bool f) const;
    bool operator !=(const Bitfield& bf) const;
    Bitfield& operator =(bool t);
    Bitfield& operator =(const Bitfield& bf);
};
