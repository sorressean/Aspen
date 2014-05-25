#include "mud.h"
#include "conf.h"
#include "olcs.h"
#include "staticObject.h"
#include "room.h"

BOOL InitializeOlcs()
{
    if (!InitializeStaticObjectOlcs())
        {
            return  false;
        }
    if (!InitializeRoomOlcs())
        {
            return false;
        }

    return true;
}
