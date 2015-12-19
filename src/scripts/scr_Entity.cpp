#include <cassert>
#include <angelscript.h>
#include "../mud.h"
#include "../conf.h"
#include "../entity.h"
#include "script.h"
#include "scr_Entity.h"
#include "scr_ObjectContainer.h"
#include "scr_BaseObject.h"
#include "helper.h"

void RegisterEntityMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r=engine->RegisterMethod(obj, "string& GetShort()", asMETHOD(Entity, GetShort));
    assert(r);
    r = engine->RegisterMethod(obj, "void SetShort(string short)", asMETHOD(Entity, SetShort));
    assert(r);
    r = engine->RegisterMethod(obj, "Entity@ GetLocation()", asMETHOD(Entity, GetLocation));
    assert(r);
    r = engine->RegisterMethod(obj, "bool MoveTo(Entity@ obj)", asMETHOD(Entity, CanReceive));
    assert(r);
}

void InitializeEntity()
{
    RegisterBaseObjectMethods("Entity");
    RegisterObjectContainerMethods("Entity");
    RegisterEntityMethods("Entity");
}
