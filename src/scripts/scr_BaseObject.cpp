#include <angelscript.h>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
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
    /*  r=engine->RegisterMethod(obj, "string& GetShort()", asMETHOD(BaseObject, GetShort));
      assert(r);
      r = engine->RegisterMethod(obj, "void SetShort(string short)", asMETHOD(BaseObject, SetShort));
      assert(r);*/
    r = engine->RegisterMethod(obj, "string& GetDescription()", asMETHOD(BaseObject, GetDescription));
    assert(r);
    r = engine->RegisterMethod(obj, "void SetDescription(string description)", asMETHOD(BaseObject, SetDescription));
    assert(r);
    /*r = engine->RegisterMethod(obj, "string& GetPlural()", asMETHOD(BaseObject, GetPlural));
    assert(r);
    r = engine->RegisterMethod(obj, "void SetPlural(string plural)", asMETHOD(BaseObject, SetPlural));
    assert(r);*/
    r = engine->RegisterMethod(obj, "bool GetPersistent() const", asMETHOD(BaseObject, GetPersistent));
    assert(r);
    r = engine->RegisterMethod(obj, "void SetPersistent(bool persistent)", asMETHOD(BaseObject, SetPersistent));
    assert(r);
//add get/set Zone
    r = engine->RegisterMethod(obj, "int GetOnum() const", asMETHOD(BaseObject, GetOnum));
    assert(r);
    r = engine->RegisterMethod(obj, "bool AddAlias(string alias)", asMETHOD(BaseObject, AddAlias));
    assert(r);
    r = engine->RegisterMethod(obj, "bool AliasExists(string& alias) const", asMETHOD(BaseObject, AliasExists));

}

void InitializeBaseObject()
{
//todo: add global properties (property, etc).
    RegisterBaseObjectMethods("BaseObject");
}
