#include <angelscript.h>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
#include "../baseObject.h"
#include "script.h"

void RegisterBaseObjectMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r=engine->RegisterMethod("BaseObject", "string& GetName()", asMETHOD(BaseObject, GetName));
    assert(r);
    r=engine->RegisterMethod("BaseObject", "void SetName(string name)", asMETHOD(BaseObject, SetName));
    assert(r);
    r=engine->RegisterMethod("BaseObject", "string& GetShort()", asMETHOD(BaseObject, GetShort));
    assert(r);
    r = engine->RegisterMethod("BaseObject", "void SetShort(string short)", asMETHOD(BaseObject, SetShort));
    assert(r);
    r = engine->RegisterMethod("BaseObject", "string& GetDescription()", asMETHOD(BaseObject, GetDescription));
    assert(r);
    r = engine->RegisterMethod("BaseObject", "void SetDescription(string description)", asMETHOD(BaseObject, SetDescription));
    assert(r);
    r = engine->RegisterMethod("BaseObject", "string& GetPlural()", asMETHOD(BaseObject, GetPlural));
    assert(r);
    r = engine->RegisterMethod("BaseObject", "void SetPlural(string plural)", asMETHOD(BaseObject, SetPlural));
    assert(r);
//finish
}

bool InitializeBaseObject()
{
//todo: add global properties (property, etc).
    ScriptEngine* engine = ScriptEngine::GetPtr();
    int r = 0;

    r = engine->GetBaseEngine()->RegisterObjectType("BaseObject", 0, asOBJ_REF|asOBJ_NOCOUNT);
    assert(r >= 0);

    RegisterBaseObjectMethods("BaseObject");

    return true;
}
