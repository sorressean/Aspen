#include <angelscript.h>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
#include "../baseObject.h"
#include "script.h"

bool InitializeBaseObject(asIScriptEngine* engine)
{
//todo: add global properties (property, etc).
    int r = 0;

    r = engine->RegisterObjectType("BaseObject", 0, asOBJ_REF|asOBJ_NOCOUNT);
    assert(r >= 0);

//methods
    r=engine->RegisterObjectMethod("BaseObject", "string& GetName()", asMETHOD(BaseObject, GetName), asCALL_THISCALL);
    assert(r >= 0);
    return true;
}
