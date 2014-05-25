#ifndef SCR_ROOM_H
#define SCR_ROOM_H
#include "../mud.h"
#include "../conf.h"
#include "../player.h"
#include "../entity.h"
#include "../world.h"
#include "../event.h"
#include "../room.h"
#include "scripts.h"

#ifdef MODULE_SCRIPTING
BOOL InitRoomScript(Script* s);

int SCR_SetRoomFlag(lua_State* l);
int SCR_GetRoomFlag(lua_State* l);
int SCR_TellAll(lua_State* l);
#endif
#endif
