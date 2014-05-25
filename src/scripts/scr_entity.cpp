#include "scr_entity.h"

#ifdef MODULE_SCRIPTING
static const struct luaL_reg entity_table [] =
{
    {"GetName", SCR_GetName},
    {"SetName", SCR_SetName},
    {"GetDescription", SCR_GetDescription},
    {"SetDescription", SCR_SetDescription},
    {"GetPlural", SCR_GetPlural},
    {"SetPlural", SCR_SetPlural},
    {"GetScript", SCR_GetScript},
    {"SetScript", SCR_SetScript},
    {"GetLocation", SCR_GetLocation},
    {"SetLocation", SCR_SetLocation},
    {"GetContents", SCR_GetContents},
    {"GetOnum", SCR_GetOnum},
    {"CanReceive", SCR_CanReceive},
    {"MoveTo", SCR_MoveTo},
    {NULL, NULL}
};

BOOL InitEntityScript(Script* s)
{
    lua_State* lstate = s->GetState();
    luaL_newmetatable(lstate, "entity");
    lua_pushstring(lstate, "__index");
    lua_pushvalue(lstate, -2);
    lua_settable(lstate, -3);
    luaL_openlib(lstate, "entity", entity_table, 0);
    return true;
}

//our script functions:
int SCR_GetName(lua_State* l)
{
    UserData* udata = NULL;
    std::string name;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetName'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetName' must be an object.");
            return 0;
        }

    name = ((Entity*)udata->ptr)->GetName();
    lua_pushlstring(l, name.c_str(), name.length());
    return 1;
}
int SCR_SetName(lua_State* l)
{
    UserData* udata = NULL;
    const char* name = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to 'SetName'.");
            return 0;
        }

    name = lua_tostring(l, -1);
    if (!name)
        {
            SCR_Error(l, "Argument 2 to 'SetName' must be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'SetName' must be an object.");
            return 0;
        }

    ((Entity*)udata->ptr)->SetName(name);
    return 0;
}

int SCR_GetDescription(lua_State *l)
{
    UserData* udata = NULL;
    std::string description;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetDescription'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetDescription' must be an object.");
            return 0;
        }

    description = ((Entity*)udata->ptr)->GetDescription();
    lua_pushlstring(l, description.c_str(), description.length());
    return 1;
}
int SCR_SetDescription(lua_State* l)
{
    UserData* udata = NULL;
    const char* description = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to 'SetDescription'.");
            return 0;
        }

    description = lua_tostring(l, -1);
    if (!description)
        {
            SCR_Error(l, "Argument 2 to 'SetDescription' must be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'SetDescription' must be an object.");
            return 0;
        }

    ((Entity*)udata->ptr)->SetDescription(description);
    return 0;
}

int SCR_GetPlural(lua_State* l)
{
    UserData* udata = NULL;
    std::string plural;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetPlural'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetPlural' must be an object.");
            return 0;
        }

    plural = ((Entity*)udata->ptr)->GetPlural();
    lua_pushlstring(l, plural.c_str(), plural.length());
    return 1;
}
int SCR_SetPlural(lua_State* l)
{
    UserData* udata = NULL;
    const char* plural = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to 'SetPlural'.");
            return 0;
        }

    plural = lua_tostring(l, -1);
    if (!plural)
        {
            SCR_Error(l, "Argument 2 to 'SetPlural' must be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'SetPlural' must be an object.");
            return 0;
        }

    ((Entity*)udata->ptr)->SetPlural(plural);
    return 0;
}

int SCR_GetScript(lua_State* l)
{
    UserData* udata = NULL;
    std::string script;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetScript'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetScript' must be an object.");
            return 0;
        }

    script = ((Entity*)udata->ptr)->GetScript();
    lua_pushlstring(l, script.c_str(), script.length());
    return 1;
}
int SCR_SetScript(lua_State* l)
{
    UserData* udata = NULL;
    const char* script = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to \'SetScript\'.");
            return 0;
        }

    script = lua_tostring(l, -1);
    if (!script)
        {
            SCR_Error(l, "Argument 2 to 'SetScript' must be a string.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'SetScript' must be an object.");
            return 0;
        }

    ((Entity*)udata->ptr)->SetScript(script);
    return 0;
}

int SCR_GetLocation(lua_State* l)
{
    UserData* udata = NULL;
    Entity* location = NULL;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetLocation'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetLocation' must be an object.");
            return 0;
        }

    location = ((Entity*)udata->ptr)->GetLocation();
    if (!location)
        {
            return 0;
        }

    ObjectToStack(l, location);
    return 1;
}
int SCR_SetLocation(lua_State* l)
{
    UserData* udata = NULL;
    UserData* location = NULL;

    if (lua_gettop(l) != 2)
        {
            SCR_Error(l, "Invalid number of arguments to 'SetLocation'.");
            return 0;
        }

    location = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, location))
        {
            SCR_Error(l, "Argument 2 to 'SetLocation' must be an object.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'SetLocation' must be an object.");
            return 0;
        }

    ((Entity*)udata->ptr)->SetLocation((Entity*)location->ptr);
    return 0;
}

int SCR_GetContents(lua_State* l)
{
    UserData* udata = NULL;
    std::list<Entity*> *contents = NULL;
    std::list<Entity*>::iterator it, itEnd;
    int i;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetContents'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetContents' must be an object.");
            return 0;
        }

    contents = ((Entity*)udata->ptr)->GetContents();
    lua_newtable(l);
    itEnd = contents->end();
    for (i = 0, it = contents->begin(); it != itEnd; ++it, ++i)
        {
            ObjectToStack(l, (*it));
            lua_rawseti(l, i, -2);
        }
    return 1;
}

int SCR_GetOnum(lua_State* l)
{
    UserData* udata = NULL;
    VNUM onum;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'GetOnum'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'GetOnum' must be an object.");
            return 0;
        }

    onum = ((Entity*)udata->ptr)->GetOnum();
    lua_pushinteger(l, onum);
    return 1;
}

int SCR_CanReceive(lua_State* l)
{
    UserData* udata = NULL;
    UserData* obj = NULL;
    BOOL ret;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'CanReceive'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'CanReceive' must be an object.");
            return 0;
        }

    obj = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 2 to 'CanReceive' must be an object.");
            return 0;
        }

    ret = ((Entity*)udata->ptr)->CanReceive((Entity*)obj->ptr);
    lua_pushboolean(l, ret);
    return 1;
}

int SCR_MoveTo(lua_State* l)
{
    UserData* udata = NULL;
    UserData* obj = NULL;
    BOOL ret;

    if (lua_gettop(l) != 1)
        {
            SCR_Error(l, "Invalid number of arguments to 'MoveTo'.");
            return 0;
        }

    udata = (UserData*)lua_touserdata(l, -2);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 1 to 'MoveTo' must be an object.");
            return 0;
        }

    obj = (UserData*)lua_touserdata(l, -1);
    if (!IsObject(l, udata))
        {
            SCR_Error(l, "Argument 2 to 'MoveTo' must be an object.");
            return 0;
        }

    ret = ((Entity*)udata->ptr)->MoveTo((Entity*)obj->ptr);
    lua_pushboolean(l, ret);
    return 1;
}
#endif