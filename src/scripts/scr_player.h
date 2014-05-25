#ifndef SCR_PLAYER_H
#define SCR_PLAYER_H
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
BOOL InitPlayerScript(Script* s);

#ifndef SCRIPT_SECURE
int SCR_GetPassword(lua_State* l);
int SCR_SetPassword(lua_State *l);
int SCR_SetRank(lua_State* l);
int SCR_SetOnlineTime(lua_State* l);
int SCR_SetFirstLogin(lua_State* l);
int SCR_SetLastLogin(lua_State* l);
#endif
int SCR_GetTitle(lua_State* l);
int SCR_SetTitle(lua_State*l);
int SCR_GetLevel(lua_State* l);
int SCR_SetLevel(lua_State* l);
int SCR_GetPflag(lua_State* l);
int SCR_SetPflag(lua_State* l);
int SCR_GetRank(lua_State* l);
int SCR_GetOnlineTime(lua_State* l);
int SCR_GetFirstLogin(lua_State* l);
int SCR_GetLastLogin(lua_State* l);
int SCR_GetPrompt(lua_State* l);
int SCR_SetPrompt(lua_State* l);
int SCR_Save(lua_State* l);
int SCR_Backup(lua_State* l);
int SCR_Write(lua_State* l);
int SCR_Message(lua_State* l);
int SCR_SetOption(lua_State* l);
int SCR_GetOption(lua_State* l);
int SCR_OptionExists(lua_State* l);
int SCR_ToggleOption(lua_State* l);
int SCR_HasAccess(lua_State* l);
#endif
#endif
