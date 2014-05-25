#ifndef SCR_ENTITY_H
#define SCR_ENTITY_H
#include "../mud.h"
#include "../conf.h"
#include "../player.h"
#include "../entity.h"
#include "../world.h"
#include "../variant.h"
#include "../event.h"
#include "scripts.h"
#include <string>
#include <cstring>

#ifdef MODULE_SCRIPTING
BOOL InitEntityScript(Script* s);
int SCR_GetName(lua_State* l);
int SCR_SetName(lua_State* l);
int SCR_GetDescription(lua_State* l);
int SCR_SetDescription(lua_State* l);
int SCR_GetPlural(lua_State* l);
int SCR_SetPlural(lua_State* l);
int SCR_GetScript(lua_State* l);
int SCR_SetScript(lua_State* l);
int SCR_GetLocation(lua_State* l);
int SCR_SetLocation(lua_State* l);
int SCR_GetContents(lua_State* l);
int SCR_GetOnum(lua_State* l);
int SCR_CanReceive(lua_State* l);
int SCR_MoveTo(lua_State* l);
#endif
#endif
