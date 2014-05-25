#include "mud.h"
#include "conf.h"
#include "door.h"
#include "exit.h"
Door::Door()
{
    _key = -1;
    _open = false;
    _autoOpen = false;
    _autoClose = false;
    _autoCloseDelay = 0;
}
Door::~Door()
{
}


int Door::GetAutocloseDelay() const
{
    return _autoCloseDelay;
}
void Door::SetAutocloseDelay(int s)
{
    _autoCloseDelay = s;
}
VNUM Door::GetKey() const
{
    return _key;
}
void Door::SetKey(VNUM s)
{
    _key = s;
}
BOOL Door::GetAutoClose() const
{
    return _autoClose;
}
void Door::SetAutoClose(BOOL s)
{
    _autoClose = s;
}
