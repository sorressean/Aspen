#include "scr_events.h"
#include "../mud.h"
#include "../conf.h"
#include "../entity.h"

#ifdef MODULE_SCRIPTING
static const struct luaL_reg event_table [] =
{
    {"AddCallback", SCR_AddCallback},
//{"CallEvent", SCR_CallEvent},
    {NULL, NULL}
};

BOOL InitEventScript(Script* s)
{
    lua_State* lstate = s->GetState();
    luaL_newmetatable(lstate, "events");
    lua_pushstring(lstate, "__index");
    lua_pushvalue(lstate, -2);
    lua_settable(lstate, -3);
    luaL_openlib(lstate, "events", event_table, 0);
    return true;
}

int SCR_AddCallback(lua_State* l)
{
    UserData* udata = NULL;
    int func = 0;
    const char* event = NULL;

    if (lua_gettop(l) != 3)
        {
            SCR_Error(l, "Invalid number of arguments to 'AddCallback'");
            return 0;
        }

    if (!lua_isfunction(l, -1))
        {
            SCR_Error(l, "Argument 3 to 'AddCallback' must be the function to call.");
            return 0;
        }
//we don't want to have to unref if we don't end up doing anything, so we'll save the ref creation.

    event = lua_tostring(l, -2);
    if (!event)
        {
            SCR_Error(l, "Argument 2 to 'AddCallback' must be the name of the event to add a callback to.");
            return  0;
        }

    udata = (UserData*)lua_touserdata(l, -3);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'AddCallback' must be the object to add the callback to.");
            return 0;
        }

    lua_rawgeti(l, LUA_REGISTRYINDEX, ((Entity*)udata->ptr)->GetOnum());
    lua_insert(l, -2);
    func = luaL_ref(l, -2);
    (((Entity*)udata->ptr)->events).AddScriptCallback(udata->ptr, event, func);
    return 0;
}
BOOL SCR_CallEvent(Entity* obj, int func, EventArgs* args, void* caller)
{
    EventArgsUserData* udata = NULL;
    lua_State* l = Script::GetState();
    lua_rawgeti(l, LUA_REGISTRYINDEX, obj->GetOnum());
//we get the function we need to call.
    lua_rawgeti(l, -1, func);
//we create the event args user data and push it.
    udata = (EventArgsUserData*)lua_newuserdata(l, sizeof(EventArgsUserData));
    udata->args = args;
    lua_call(l, 1, 0);
    return true;
}
#endif
