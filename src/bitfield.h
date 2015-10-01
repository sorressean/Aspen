/**
* Contains: Bitfield
* This is a bit field returned by flag's getters and setters.
*/
#pragma once
#include "mud.h"
#include "conf.h"

/**
* A bitfield is used to manage flags and their positions.
*/
class Bitfield
{
    int& _flag;
    int _position;
public:
    /**
    * Constructs a bitfield with a specific position set.
    *
    * @param [in] flag the flag.
    * @param [in] position the position used.
    */
    Bitfield(int& flag, int position);
    Bitfield(const Bitfield& bf);
    ~Bitfield();
    /**
    * Determines whether the field is set.
    *
    * @return True if the flag is set, false otherwise.
    */
    bool GetField() const;

    bool operator ==(bool f) const;
    bool operator ==(const Bitfield& bf) const;
    bool operator !=(bool f) const;
    bool operator !=(const Bitfield& bf) const;
    Bitfield& operator =(bool t);
    Bitfield& operator =(const Bitfield& bf);
};
