#include "mud.h"
#include "conf.h"
#include "olcs.h"
#include "staticObject.h"
#include "room.h"
#include "entity.h"
#include "baseObject.h"
#include "npc.h"

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
    if (!InitializeNPCOlcs())
        {
            return false;
        }
    return true;
}
