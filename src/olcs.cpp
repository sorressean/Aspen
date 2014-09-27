#include "mud.h"
#include "conf.h"
#include "olcs.h"
#include "staticObject.h"
#include "room.h"
#include "entity.h"
#include "baseObject.h"

BOOL InitializeOlcs()
{
    if (!InitializeBaseObjectOlcs())
        {
            return false;
        }
    if (!InitializeEntityOlcs())
        {
            return false;
        }
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
