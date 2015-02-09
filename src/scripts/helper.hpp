#pragma once
/*
*Script helpers.
*These are the templated functions, do not include directly.
*/
#include <angelscript.h>
#include <scriptarray.h>

template <class C>
CScriptArray* ContainerToScriptArray(const char* odecl, C& container)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    size_t size; //size of container.
    size_t i; //for index.
    void* ptr = nullptr; //holds the individual element.
    asIObjectType* objtype = nullptr; //holds declaration.
    CScriptArray* ret  = nullptr;

    size = container.size();
    objtype = engine->GetBaseEngine()->GetObjectTypeByDecl(odecl);
    ret = CScriptArray::Create(objtype, size);

//we do the actual copy.
    for (auto it: container)
        {
            ptr = &it;
            ret->SetValue(i, ptr);
            i++;
        }

    return ret;
}
