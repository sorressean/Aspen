#include <angelscript.h>
#include <scriptarray.h>
#include "../mud.h"
#include "../conf.h"
#include "../entity.h"
#include "script.h"
#include "scr_Entity.h"
#include "scr_BaseObject.h"
#include "helper.h"

CScriptArray* GetEntityContents(Entity* obj)
{
    if (obj == nullptr)
        {
            return nullptr;
        }

    return ContainerToScriptArray<std::list<Entity*>>("array<Entity@>", *(obj->GetContents()));
}

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
    r = engine->RegisterMethod(obj, "array<Entity@>@ GetContents() const", asFUNCTION(GetEntityContents), asCALL_CDECL_OBJLAST);
    assert(r);
#warning todo: add get and set parent.
    r = engine->RegisterMethod(obj, "bool CanReceive(Entity@ obj) const", asMETHOD(Entity, CanReceive));
    assert(r);
    r = engine->RegisterMethod(obj, "bool MoveTo(Entity@ obj)", asMETHOD(Entity, CanReceive));
    assert(r);
}

void InitializeEntity()
{
    RegisterBaseObjectMethods("Entity");
    RegisterEntityMethods("Entity");
}
