#include <angelscript.h>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
#include "../baseObject.h"
#include "script.h"

void RegisterBaseObjectMethods(asIScriptEngine* engine, const char* obj)
{
    int r = 0;

    r=engine->RegisterObjectMethod("BaseObject", "string& GetName()", asMETHOD(BaseObject, GetName), asCALL_THISCALL);
    assert(r >= 0);
    r=engine->RegisterObjectMethod("BaseObject", "void SetName(string name)", asMETHOD(BaseObject, SetName), asCALL_THISCALL);
    assert(r >= 0);
    r=engine->RegisterObjectMethod("BaseObject", "string& GetShort()", asMETHOD(BaseObject, GetShort), asCALL_THISCALL);
    assert(r >= 0);
    r = engine->RegisterObjectMethod("BaseObject", "void SetShort(string short)", asMETHOD(BaseObject, SetShort), asCALL_THISCALL);
    assert(r >= 0);
    r = engine->RegisterObjectMethod("BaseObject", "string& GetDescription()", asMETHOD(BaseObject, GetDescription), asCALL_THISCALL);
    assert(r >= 0);
    r = engine->RegisterObjectMethod("BaseObject", "void SetDescription(string description)", asMETHOD(BaseObject, SetDescription), asCALL_THISCALL);
    assert(r >= 0);
    r = engine->RegisterObjectMethod("BaseObject", "string& GetPlural()", asMETHOD(BaseObject, GetPlural), asCALL_THISCALL);
    assert(r >= 0);
    r = engine->RegisterObjectMethod("BaseObject", "void SetPlural(string plural)", asMETHOD(BaseObject, SetPlural), asCALL_THISCALL);
    assert(r >= 0);
//finish
}

bool InitializeBaseObject(asIScriptEngine* engine)
{
//todo: add global properties (property, etc).
    int r = 0;

    r = engine->RegisterObjectType("BaseObject", 0, asOBJ_REF|asOBJ_NOCOUNT);
    assert(r >= 0);

    RegisterBaseObjectMethods(engine, "BaseObject");

    return true;
}
