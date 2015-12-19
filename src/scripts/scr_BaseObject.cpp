#include <angelscript.h>
#include <cassert>
#include "../mud.h"
#include "../baseObject.h"
#include "script.h"
#include "scr_BaseObject.h"

void RegisterBaseObjectMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r=engine->RegisterMethod(obj, "string& GetName()", asMETHOD(BaseObject, GetName));
    assert(r);
    r=engine->RegisterMethod(obj, "void SetName(string name)", asMETHOD(BaseObject, SetName));
    assert(r);
    r = engine->RegisterMethod(obj, "string& GetDescription()", asMETHOD(BaseObject, GetDescription));
    assert(r);
    r = engine->RegisterMethod(obj, "void SetDescription(string description)", asMETHOD(BaseObject, SetDescription));
    assert(r);
//TODO: add get/set Zone
    r = engine->RegisterMethod(obj, "VNUM GetOnum() const", asMETHOD(BaseObject, GetOnum));
    assert(r);
//TODO: add components.
    r = engine->RegisterMethod(obj, "void Attach()", asMETHOD(BaseObject, Attach));
    assert(r);
    r = engine->RegisterMethod(obj, "bool IsPlayer() const", asMETHOD(BaseObject, IsPlayer));
    assert(r);
    r = engine->RegisterMethod(obj, "bool IsLiving() const", asMETHOD(BaseObject, IsLiving));
    assert(r);
    r = engine->RegisterMethod(obj, "bool IsRoom() const", asMETHOD(BaseObject, IsRoom));
    assert(r);
    r = engine->RegisterMethod(obj, "bool IsNpc() const", asMETHOD(BaseObject, IsNpc));
    assert(r);
    r = engine->RegisterMethod(obj, "bool IsObject() const", asMETHOD(BaseObject, IsObject));
    assert(r);

    /*
        r = engine->RegisterMethod(obj, "bool AddAlias(string alias)", asMETHOD(BaseObject, AddAlias));
        assert(r);
        r = engine->RegisterMethod(obj, "bool AliasExists(string& alias) const", asMETHOD(BaseObject, AliasExists));
    */
}

void InitializeBaseObject()
{
//todo: add global properties (property, etc).
    RegisterBaseObjectMethods("BaseObject");
}
