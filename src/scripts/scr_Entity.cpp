#include <angelscript.h>
#include "../mud.h"
#include "../conf.h"
#include "../entity.h"
#include "script.h"
#include "scr_BaseObject.h"

void RegisterEntityMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r = engine->RegisterMethod(obj, "Entity@ GetLocation()", asMETHOD(Entity, GetName));
    assert(r);
}

void InitializeEntity()
{
    RegisterBaseObjectMethods("Entity");
    RegisterEntityMethods("Entity");
}
