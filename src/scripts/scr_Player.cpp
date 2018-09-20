/**
* Contains registration logic and wrappers for the player object.
*/
#include <cassert>
#include <angelscript.h>

#include "scr_Entity.h"
#include "scr_Player.h"
#include "script.h"

#include "../mud.h"
#include "../player.h"

void RegisterPlayerMethods(const char* obj)
{
    ScriptEngine* engine = ScriptEngine::GetPtr();
    bool r;

    r = engine->RegisterMethod(obj, "bool IsPlayer() const", asMETHOD(Player, IsPlayer));
    assert(r);
    r=engine->RegisterMethod(obj, "string& GetShort()", asMETHOD(Player, GetShort));
    assert(r);
    r=engine->RegisterMethod(obj, "string& GetTitle() const", asMETHOD(Player, GetTitle));
    assert(r);
    r = engine->RegisterMethod(obj, "flag GetRank() const", asMETHOD(Player, GetRank));
    assert(r);
    r = engine->RegisterMethod(obj, "flag GetPflag() const", asMETHOD(Player, GetPflag));
    assert(r);
    r = engine->RegisterMethod(obj, "unsigned int GetOnlineTime() const", asMETHOD(Player, GetOnlineTime));
    assert(r);
    r = engine->RegisterMethod(obj, "unsigned int GetFirstLogin() const", asMETHOD(Player, GetFirstLogin));
    assert(r);
    r = engine->RegisterMethod(obj, "unsigned int GetLastLogin() const", asMETHOD(Player, GetLastLogin));
}

void InitializePlayerScript()
{
    RegisterEntityMethods("Entity");
    RegisterPlayerMethods("Player");
}
