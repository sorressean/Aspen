#ifndef SCR_EVENT_MANAGER_H
#define SCR_EVENT_MANAGER_H
#include "../mud.h"
#include "../conf.h"
#include "scripts.h"

#ifdef MODULE_SCRIPTING
class EventArgs;

struct EventArgsUserData
{
    EventArgs* args;
};

BOOL InitEventScript(Script* s);
int SCR_AddCallback(lua_State* l);
BOOL SCR_CallEvent(Entity* obj, int func, EventArgs* args, void* caller);
#endif
#endif
