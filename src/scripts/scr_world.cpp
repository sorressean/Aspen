#include <cstring>
#include "scr_world.h"
#include "scripts.h"
#include "../world.h"
#include "../event.h"

#ifdef MODULE_SCRIPTING
//our library tables:
//world:
static const struct luaL_reg world_table [] =
{
    {"Shutdown", SCR_Shutdown},
    {"GetPlayers", SCR_GetPlayers},
    {"FindPlayer", SCR_FindPlayer},
//{"AddCommand", SCR_AddCommand},
//{"RemoveCommand", SCR_RemoveCommand},
    {"AddChannel", SCR_AddChannel},
//{"FindChannel", SCR_FindChannel},
//{"RegisterComponent", SCR_RegisterComponent},
//{"CreateComponent", SCR_CreateComponent},
//{"GetRoom", SCR_GetRoom},
//{"GetObject", SCR_GetObject},
    {"GetRealUptime", SCR_GetRealUptime},
    {"GetCopyoverUptime", SCR_GetCopyoverUptime},
    {"GetBanner", SCR_GetBanner},
    {"GetMotd", SCR_GetMotd},
//{"DoCommand", SCR_DoCommand},
//{"MatchObject", SCR_MatchObject},
//{"GetZone", SCR_GetZone},
    {"WriteLog", SCR_WriteLog},
    {NULL, NULL}
};

BOOL InitWorldScript(Script* s)
{
    lua_State* lstate = s->GetState();
    luaL_newmetatable(lstate, "world");
    lua_pushstring(lstate, "__index");
    lua_pushvalue(lstate, -2);
    lua_settable(lstate, -3);
    luaL_openlib(lstate, "world", world_table, 0);
    return true;
}

int SCR_Shutdown(lua_State* l)
{
    World* world = World::GetPtr();
    world->Shutdown();
    lua_pushboolean(l, true);
    return 1;
}
int SCR_GetRealUptime(lua_State* l)
{
    World* world = World::GetPtr();
    lua_pushnumber(l, world->GetRealUptime());
    return 1;
}
int SCR_GetCopyoverUptime(lua_State* l)
{
    World* world = World::GetPtr();
    lua_pushnumber(l, world->GetCopyoverUptime());
    return 1;
}
int SCR_FindPlayer(lua_State* l)
{
    World* world = World::GetPtr();
    const char* search = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "'FindPlayer' takes 1 argument.");
            return 0;
        }
    search = lua_tostring(l, -1);
    if (!search)
        {
            SCR_Error(l, "Argument 1 to 'FindPlayher' must be the name of the player to find.");
            return 0;
        }

    UserData* data = (UserData*)lua_newuserdata(l, sizeof(UserData));
    data->ptr = (Entity*)world->FindPlayer(search);
    data->type = OBJECT;
    return 1;
}
int SCR_GetPlayers(lua_State* l)
{
    World* world = World::GetPtr();

//our list of players
    std::list <Player*> *players = world->GetPlayers();
//iterators for the list.
    std::list<Player*>::iterator it, itEnd;
//counter for index
    int i;

    lua_newtable(l);
    itEnd = players->end();
    for (i = 0, it = players->begin(); it != itEnd; ++i, ++it)
        {
            ObjectToStack(l, (*it));
            lua_rawseti(l, i, -2);
        }
    return 1;
}
int SCR_GetBanner(lua_State* l)
{
    World* world = World::GetPtr();

    const char*banner = world->GetBanner();
    lua_pushlstring(l, banner, strlen(banner));
    return 1;
}
int SCR_GetMotd(lua_State* l)
{
    World* world = World::GetPtr();

    const char* motd = world->GetMotd();
    lua_pushlstring(l, motd, strlen(motd));
    return 1;
}
int SCR_WriteLog(lua_State* l)
{
    World* world = World::GetPtr();

    world->WriteLog(lua_tostring(l, -1), SCRIPT);
    return 0;
}

int SCR_AddChannel(lua_State* l)
{
    return 0;
}
#endif
