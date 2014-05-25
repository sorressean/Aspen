#include <string>
#include <cstring>
#include "scr_player.h"
#include "scripts.h"
#include "../player.h"
#include "../world.h"
#include "../variant.h"

#ifdef MODULE_SCRIPTING
static const struct luaL_reg player_table [] =
{
//secure functions
#ifndef SCRIPT_SECURE
    {"SetPassword", SCR_SetPassword},
    {"SetRank", SCR_SetRank},
    {"SetOnlineTime", SCR_SetOnlineTime},
    {"SetFirstLogin", SCR_SetFirstLogin},
    {"SetLastLogin", SCR_SetLastLogin},
    {"GetPassword", SCR_GetPassword},
#endif
//normal functions.
    {"GetTitle", SCR_GetTitle},
    {"SetTitle", SCR_SetTitle},
    {"GetLevel", SCR_GetLevel},
    {"SetLevel", SCR_SetLevel},
    {"GetPflag", SCR_GetPflag},
    {"SetPflag", SCR_SetPflag},
    {"GetRank", SCR_GetRank},
    {"GetFirstLogin", SCR_GetFirstLogin},
    {"GetLastLogin", SCR_GetLastLogin},
    {"GetPrompt", SCR_GetPrompt},
    {"SetPrompt", SCR_SetPrompt},
    {"Save", SCR_Save},
    {"Backup", SCR_Backup},
    {"Write", SCR_Write},
    {"Message", SCR_Message},
    {"SetOption", SCR_SetOption},
    {"GetOption", SCR_GetOption},
    {"OptionExists", SCR_OptionExists},
    {"ToggleOption", SCR_ToggleOption},
    {"HasAccess", SCR_HasAccess},
    {NULL, NULL}
};

BOOL InitPlayerScript(Script* s)
{
    lua_State* lstate = s->GetState();
    luaL_newmetatable(lstate, "player");
    lua_pushstring(lstate, "__index");
    lua_pushvalue(lstate, -2);
    lua_settable(lstate, -3);
    luaL_openlib(lstate, "player", player_table, 0);
    MessageType i;
    i = MSG_ERROR;
    lua_pushnumber(lstate, (int)i);
    lua_setglobal(lstate, "MSG_ERROR");
    i = MSG_INFO;
    lua_pushnumber(lstate, (int)i);
    lua_setglobal(lstate, "MSG_INFO");
    i = MSG_CRITICAL;
    lua_pushnumber(lstate, (int)i);
    lua_setglobal(lstate, "MSG_CRITICAL");
    i = MSG_CHANNEL;
    lua_pushnumber(lstate, (int)i);
    lua_setglobal(lstate, "MSG_CHANNEL");
    i = MSG_LIST;
    lua_pushnumber(lstate, (int)i);
    lua_setglobal(lstate, "MSG_LIST");
    return true;
}

#ifndef SCRIPT_SECURE
int SCR_GetPassword(lua_State* l)
{
    std::string password ;
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'GetPassword\'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    password = ((Player*)udata->ptr)->GetPassword();
    lua_pushlstring(l, password.c_str(), password.length());
    return 1;
}
int SCR_SetPassword(lua_State *l)
{
    UserData* udata = NULL;
    const char* password = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetPassword\'.");
            return 0;
        }

    password = lua_tostring(l, -1);
    if (!password)
        {
            SCR_Error(l, "Second argument to \'SetPassword\' should be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->SetPassword(password);
    return 0;
}
#endif

int SCR_GetTitle(lua_State* l)
{
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'GetTitle\'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    lua_pushlstring(l, ((Player*)udata->ptr)->GetTitle().c_str(), ((Player*)udata->ptr)->GetTitle().length());
    return 1;
}
int SCR_SetTitle(lua_State* l)
{
    const char* title = NULL;
    UserData* udata = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetTitle\'.");
            return 0;
        }

    title = lua_tostring(l, -1);
    if (!title)
        {
            SCR_Error(l, "Argument 2 to \'SetTitle\' must be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->SetTitle(title);
    return 0;
}

int SCR_GetLevel(lua_State* l)
{
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'GetLevel\'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    lua_pushinteger(l, ((Player*)udata->ptr)->GetLevel());
    return 1;
}
int SCR_SetLevel(lua_State* l)
{
    int level = 0;
    UserData* udata = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetLevel\'.");
            return 0;
        }

    level = lua_tointeger(l, -1);
    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->SetLevel(level);
    return 0;
}

int SCR_GetPflag(lua_State* l)
{
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'GetFlag\'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    lua_pushinteger(l, ((Player*)udata->ptr)->GetPflag());
    return 1;
}
int SCR_SetPflag(lua_State* l)
{
    FLAG flag = 0;
    UserData* udata = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetPflag\'.");
            return 0;
        }

    flag = lua_tointeger(l, -1);
    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->SetPflag(flag);
    return 0;
}

int SCR_GetRank(lua_State* l)
{
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'GetRank\'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    lua_pushinteger(l, ((Player*)udata->ptr)->GetRank());
    return 1;
}
#ifndef SCRIPT_SECURE
int SCR_SetRank(lua_State* l)
{
    int rank = 0;
    UserData* udata = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetRank\'.");
            return 0;
        }

    rank = lua_tointeger(l, -1);
    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->SetRank(rank);
    return 0;
}
#endif

int SCR_GetOnlineTime(lua_State* l)
{
    UserData* udata = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'GetOnlineTime\'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    lua_pushinteger(l, ((Player*)udata->ptr)->GetOnlineTime());
    return 1;
}
#ifndef SCRIPT_SECURE
int SCR_SetOnlineTime(lua_State* l)
{
    UserData* udata = NULL;
    UINT tm = 0;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetOnlineTime\'.");
            return 0;
        }

    tm = lua_tointeger(l, -1);
    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->SetOnlineTime(tm);
    return 0;
}
#endif

//todo: clean up below here
int SCR_GetFirstLogin(lua_State* l)
{
    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to GetFirstLogin.");
            return 0;
        }

    UserData* udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    lua_pushinteger(l, ((Player*)udata->ptr)->GetFirstLogin());
    return 1;
}
#ifndef SCRIPT_SECURE
int SCR_SetFirstLogin(lua_State* l)
{
    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to SetFirstLogin.");
            return 0;
        }

    UINT tm = lua_tointeger(l, -1);
    UserData* udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    ((Player*)udata->ptr)->SetFirstLogin(tm);
    return 0;
}
#endif

int SCR_GetLastLogin(lua_State* l)
{
    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to GetLastLogin.");
            return 0;
        }

    UserData* udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    lua_pushinteger(l, ((Player*)udata->ptr)->GetLastLogin());
    return 1;
}
#ifndef SCRIPT_SECURE
int SCR_SetLastLogin(lua_State* l)
{
    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to SetLastLogin.");
            return 0;
        }

    UINT tm = lua_tointeger(l, -1);
    UserData* udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    ((Player*)udata->ptr)->SetLastLogin(tm);
    return 0;
}
#endif

int SCR_GetPrompt(lua_State* l)
{
    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to GetPrompt.");
            return 0;
        }

    UserData* udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    lua_pushlstring(l, ((Player*)udata->ptr)->GetPrompt().c_str(), ((Player*)udata->ptr)->GetPrompt().length());
    return 1;
}
int SCR_SetPrompt(lua_State* l)
{
    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to SetPrompt.");
            return 0;
        }

    const char* prompt = lua_tostring(l, -1);
    if (!prompt)
        {
            SCR_Error(l, "Argument 2 to \'SetPrompt\' must be a string.");
            return 0;
        }
    UserData* udata = (UserData*)lua_touserdata(l, -2);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    ((Player*)udata->ptr)->SetPrompt(prompt);
    return 0;
}

int SCR_Save(lua_State* l)
{
    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to Save.");
            return 0;
        }

    UserData* udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            return false;
        }

    ((Player*)udata->ptr)->Save();
    return 0;
}
int SCR_Backup(lua_State* l)
{
    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to \'backup\'.");
            return 0;
        }

    UserData* udata = (UserData*)lua_touserdata(l, -1);
    if (!IsPlayer(l, udata))
        {
            SCR_Error(l, "Argument 1 to \'Backup\' must be a player object.");
            return false;
        }

    ((Player*)udata->ptr)->Backup();
    return 0;
}

int SCR_Write(lua_State* l)
{
    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to Write.");
            return 0;
        }

    const char* message = NULL;
    UserData* udata = NULL;

    message = lua_tostring(l, -1);
    if (!message)
        {
            SCR_Error(l, "Argument 2 to \'Write\' should be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'Write\' should be a player object.");
            return 0;
        }

    if (!IsPlayer(l, udata))
        {
            return 0;
        }

    ((Player*)udata->ptr)->Write(message);
    return 0;
}

int SCR_Message(lua_State* l)
{
    if (lua_gettop(l) != 3)
        {
            SCR_Error(l, "Invalid number of arguments to Message.");
            return 0;
        }

    const char* message = lua_tostring(l, -1);
    if (!message)
        {
            SCR_Error(l, "Argument 3 to \'Message\' should be a string.");
            return 0;
        }

    MessageType type = (MessageType)lua_tointeger(l, -2);

    UserData* udata = (UserData*)lua_touserdata(l, -3);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'Message\' should be a player object.");
            return 0;
        }

    ((Player*)udata->ptr)->Message(type, message);
    return 0;
}

int SCR_SetOption(lua_State* l)
{
    Variant var;
    const char* str = NULL;
    UserData* udata = NULL;

    if (lua_gettop(l) != 3)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetOption\'.");
            return 0;
        }

    var = IndexToVariant(l, -1);
    if (var.Typeof() == VAR_EMPTY)
        {
            SCR_Error(l, "Argument 3 to \'SetOption\' must be a float, integer, or string.");
            return 0;
        }

    str = lua_tostring(l, -2);
    if (!str)
        {
            SCR_Error(l, "Argument 2 to \'SetOption\' must be the option name.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -3);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'SetOption\' must be a player object.");
            return 0;
        }
    if (!IsPlayer(l, udata))
        {
            SCR_Error(l, "Argument 1 to \'SetOption\' must be a player object.");
            return 0;
        }

    ((Player*)udata->ptr)->SetOption(str, var);
    return 0;
}
int SCR_GetOption(lua_State* l)
{
    UserData* udata = NULL;
    const char* str = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalud number of arguments to \'GetOption\'.");
            return 0;
        }

    str = lua_tostring(l, -1);
    if (!str)
        {
            SCR_Error(l, "Argument 2 to \'GetOption\' must be the option to retrieve.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'GetOption\' must be an object.");
            return 0;
        }
    if (!IsPlayer(l, udata))
        {
            SCR_Error(l, "Argument 1 to \'GetOption\' must be a player object.");
            return 0;
        }

    if (VariantToStack(l, ((Player*)udata->ptr)->GetOption(str)->GetValue()))
        {
            return 1;
        }

    return 0;
}
int SCR_OptionExists(lua_State* l)
{
    UserData* udata = NULL;
    const char* str = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalud number of arguments to \'OptionExists\'.");
            return 0;
        }

    str = lua_tostring(l, -1);
    if (!str)
        {
            SCR_Error(l, "Argument 2 to \'OptionExists\' must be the option to retrieve.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'OptionExists\' must be an object.");
            return 0;
        }
    if (!IsPlayer(l, udata))
        {
            SCR_Error(l, "Argument 1 to \'OptionExists\' must be a player object.");
            return 0;
        }

    lua_pushboolean(l, ((Player*)udata->ptr)->OptionExists(str));
    return 1;
}
int SCR_ToggleOption(lua_State* l)
{
    const char* str = NULL;
    UserData* udata = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalud number of arguments to \'ToggleOption\'.");
            return 0;
        }

    str = lua_tostring(l, -1);
    if (!str)
        {
            SCR_Error(l, "Argument 2 to \'ToggleOption\' must be the option to retrieve.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'ToggleOption\' must be an object.");
            return 0;
        }
    if (!IsPlayer(l, udata))
        {
            SCR_Error(l, "Argument 1 to \'ToggleOption\' must be a player object.");
            return 0;
        }

    ((Player*)udata->ptr)->ToggleOption(str);
    return 0;
}

int SCR_HasAccess(lua_State* l)
{
    UserData* udata = NULL;
    int access = 0;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalud number of arguments to \'HasAccess\'.");
            return 0;
        }

    access = lua_tointeger(l, -1);
    if (!access)
        {
            SCR_Error(l, "Argument 2 to \'HasAccess\' must be the option to retrieve.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!udata)
        {
            SCR_Error(l, "Argument 1 to \'HasAccess\' must be an object.");
            return 0;
        }
    if (!IsPlayer(l, udata))
        {
            SCR_Error(l, "Argument 1 to \'HasAccess\' must be a player object.");
            return 0;
        }

    lua_pushboolean(l, ((Player*)udata->ptr)->HasAccess(access));
    return 1;
}
#endif
