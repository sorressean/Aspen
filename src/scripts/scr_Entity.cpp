#include <angelscript.h>
#include <scriptarray.h>
#include "../mud.h"
#include "../conf.h"
#include "../entity.h"
#include "script.h"
#include "scr_Entity.h"
#include "scr_BaseObject.h"

static CScriptArray* GetEntityContents(Entity* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    unsigned int size;
    unsigned int i;
    void* ptr = nullptr;

    if (obj == nullptr)
        {
            return nullptr;
        }

    std::list<Entity*>* contents = obj->GetContents();
    size = contents->size();
    asIObjectType* objtype = engine->GetBaseEngine()->GetObjectTypeByDecl("array<Entity@>");
    CScriptArray* ret = CScriptArray::Create(objtype, size);

    for (i = 0; i < size; ++i)
        {
            ptr = &contents[i];
            ret->SetValue(i, ptr);
        }

    return ret;
}

void RegisterEntityMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r = engine->RegisterMethod(obj, "Entity@ GetLocation()", asMETHOD(Entity, GetLocation));
    assert(r);
    r = engine->RegisterMethod(obj, "array<Entity@>@ GetContents() const", asFUNCTION(GetEntityContents), asCALL_CDECL_OBJLAST);
    assert(r);
}

void InitializeEntity()
{
    RegisterBaseObjectMethods("Entity");
    RegisterEntityMethods("Entity");
}
