#pragma once
#include "mud.h"
#include "conf.h"
#include "exit.h"

class Door:public Exit
{
    VNUM _key; //the object that acts as a key.
    BOOL _open; //is the door open?
    BOOL _autoOpen; //does it automagically open?
    BOOL _autoClose; //does the door autoclosing?
    int _autoCloseDelay; //how long it takes to close the door.
public:
    Door();
    ~Door();
    int GetAutocloseDelay() const;
    void SetAutocloseDelay(int s);
    VNUM GetKey() const;
    void SetKey(VNUM s);
    BOOL GetAutoClose() const;
    void SetAutoClose(BOOL s);
};
