#include <cassert>
#include <angelscript.h>
#include "../mud.h"
#include "../exit.h"
#include "script.h"

void RegisterExitMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r = engine->RegisterMethod(obj, "vnum GetTo() const", asMETHOD(Exit, GetTo));
    assert(r);
    r = engine->RegisterMethod(obj, "void SetTo(vnum)", asMETHOD(Exit, SetTo));
    assert(r);
}
void InitializeExit()
{
    RegisterExitMethods("Exit");
}
