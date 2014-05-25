#ifndef SCR_WORLD_H
#include "../mud.h"
#include "../conf.h"
#include "../event.h"
#include "scripts.h"

#ifdef MODULE_SCRIPTING
BOOL InitWorldScript(Script* s);

//prototypes
int SCR_Shutdown(lua_State* l);
int SCR_GetRealUptime(lua_State* l);
int SCR_GetCopyoverUptime(lua_State* l);
int SCR_FindPlayer(lua_State* l);
int SCR_GetPlayers(lua_State* l);
int SCR_GetBanner(lua_State* l);
int SCR_GetMotd(lua_State* l);
int SCR_WriteLog(lua_State* l);
int SCR_AddChannel(lua_State* l);
#endif
#endif
