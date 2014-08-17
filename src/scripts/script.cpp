#include <angelscript.h>
#include <scriptstdstring.h>
#include <sstream>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
#include "../world.h"
#include "../event.h"
#include "script.h"
#include "scr_BaseObject.h"

static void MessageCallback(const asSMessageInfo *msg, void *param)
{
    World* world = World::GetPtr();
    std::stringstream st;

//error type:
    switch(msg->type)
        {
        case asMSGTYPE_ERROR:
            st << "[ERROR]";
            break;
        case asMSGTYPE_WARNING:
            st << "[WARNING]";
            break;
        case asMSGTYPE_INFORMATION:
            st << "[INFORMATION]";
        }

    st << " @" << msg->section << ":";
    st << "LN " << msg->row << ", COL " << msg->col << ":";
    st << msg->message;
    world->WriteLog(st.str(), SCRIPT);
}

ScriptEngine* ScriptEngine::_ptr;
ScriptEngine::ScriptEngine()
{
    _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    RegisterStdString(_engine);
    _engine->SetMessageCallback(asFUNCTION(MessageCallback), NULL, asCALL_CDECL);
}
ScriptEngine::~ScriptEngine()
{
    _engine->Release();
}

void ScriptEngine::Initialize()
{
    _ptr = new ScriptEngine();
}
void ScriptEngine::Release()
{
    if (_ptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
}
ScriptEngine* ScriptEngine::GetPtr()
{
    return _ptr;
}

asIScriptEngine* ScriptEngine::GetBaseEngine()
{
    return _engine;
}
bool ScriptEngine::RegisterMethod(const char* obj, const char* decl, asSFuncPtr ptr)
{
    int ret = 0;
    ret = _engine->RegisterObjectMethod(obj, decl, ptr, asCALL_THISCALL);
    return (ret >= 0? true : false);
}


CEVENT(ScriptEngine, Shutdown)
{
    World* world = World::GetPtr();
    world->WriteLog("Cleaning up scripting.");
    ScriptEngine::Release();
}

bool InitializeScript()
{
    bool ret = false;

    World* world = World::GetPtr();
    ScriptEngine* engine = nullptr;

    world->WriteLog("Initializing scripting.");

    ScriptEngine::Initialize();
    engine = ScriptEngine::GetPtr();
    world->events.AddCallback("Shutdown", std::bind(&ScriptEngine::Shutdown, engine, std::placeholders::_1, std::placeholders::_2));

    ret = InitializeBaseObject();
    assert(ret==true);
    return true;
}
