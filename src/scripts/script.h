#pragma once
#include "../mud.h"
#include "../conf.h"
#include "../event.h"

class ScriptEngine
{
    ScriptEngine();
    ~ScriptEngine();
    static ScriptEngine* _ptr;
    asIScriptEngine* _engine;
public:
    static void Initialize();
    static void Release();
    static ScriptEngine* GetPtr();
    asIScriptEngine* GetBaseEngine();
    bool RegisterMethod(const char* obj, const char* decl, asSFuncPtr ptr);
    bool RegisterObject(const char* obj);
    EVENT(Shutdown);
};

bool InitializeScript();