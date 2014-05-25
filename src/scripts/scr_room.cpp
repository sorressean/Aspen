#include "scr_room.h"

#ifdef MODULE_SCRIPTING
static const struct luaL_reg  room_table [] =
{
//{"AddExit", SCR_AddExit},
//{"ExitExists", SCR_ExitExists},
//{"GetExit", SCR_GetExit},
//{"GetExits", SCR_GetExits},
    {"SetRoomFlag", SCR_SetRoomFlag},
    {"GetRoomFlag", SCR_GetRoomFlag},
    {"TellAll", SCR_TellAll},
//{"TellAllBut", SCR_TellAllBut},
//{"GetCoord", SCR_GetCoord},
//{"SetCoord", SCR_SetCoord},
    {NULL, NULL}
};

BOOL InitRoomScript(Script* s)
{
    lua_State* lstate = s->GetState();
    luaL_newmetatable(lstate, "room");
    lua_pushstring(lstate, "__index");
    lua_pushvalue(lstate, -2);
    lua_settable(lstate, -3);
    luaL_openlib(lstate, "room", room_table, 0);
    return true;
}

int SCR_SetRoomFlag(lua_State* l)
{
    UserData* udata = NULL;
    FLAG flag = 0;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetRoomFlag'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsRoom(l, udata))
        {
            SCR_Error(l, "Argument 2 to 'SetRoomFlag' must be the room whose flag you wish to set.");
            return 0;
        }

    flag = (FLAG)lua_tointeger(l, -2);
    ((Room*)(udata->ptr))->SetRoomFlag(flag);
    return 0;
}
int SCR_GetRoomFlag(lua_State* l)
{
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetRoomFlag'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsRoom(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetRoomFlag' must be a room.");
            return 0;
        }

    lua_pushinteger(l, ((Room*)(udata->ptr))->GetRoomFlag());
    return 1;
}

int SCR_TellAll(lua_State* l)
{
    UserData* udata = NULL;
    const char* message = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to 'TellAll'.");
            return 0;
        }

    message = lua_tostring(l, -1);
    if (!message)
        {
            SCR_Error(l, "Argument 2 to 'TellAll' must be the message to send.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsRoom(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'TellAll' must be the room to send the message to.");
            return 0;
        }

    ((Room*)udata->ptr)->TellAll(message);
    return 0;
}
int SCR_TellAllBut(lua_State* l)
{
    UserData* udata = NULL;
    UserData* exclude = NULL;
    const char* message = NULL;

    if (lua_gettop(l) != 3)
        {
            SCR_Error(l, "Invalud number of arguments to 'TellAllBut'.");
            return 0;
        }

    message = lua_tostring(l, -1);
    if (!message)
        {
            SCR_Error(l, "Argument 3 to 'TellAllBut' must be the message to send to the room.");
            return 0;
        }

    exclude = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, exclude))
        {
            SCR_Error(l, "Argument 2 to 'TellAllBut' must be the player you wish to exclude.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -3);
    if (!IsRoom(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'TellAllBut' must be the room you wish to send the message to.");
            return 0;
        }

    ((Room*)udata->ptr)->TellAllBut(message, (Player*)exclude->ptr);
    return 0;
}
#endif
