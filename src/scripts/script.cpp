#include <angelscript.h>
#include <scriptstdstring.h>
#include <sstream>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
#include "../world.h"
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

bool InitializeScript()
{
    bool ret = false;

    World* world = World::GetPtr();
    world->WriteLog("Initializing scripting.");
    asIScriptEngine *engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    if (engine == NULL)
        {
            world->WriteLog("Error initializing scripting, could not create script engine.", CRIT);
            return false;
        }

    RegisterStdString(engine);
    engine->SetMessageCallback(asFUNCTION(MessageCallback), NULL, asCALL_CDECL);

    ret = InitializeBaseObject(engine);
    assert(ret==true);
    return true;
}
