#include "npc.h"
#include "olc.h"
#include "olcGroup.h"
#include "world.h"
#include "olcManager.h"

Npc::Npc()
{
}
Npc::~Npc()
{
}

Room* Npc::GetOrigin() const
{
    return _origin;
}
void Npc::SetOrigin(Room* o)
{
    _origin = o;
}

BOOL Npc::IsNpc() const
{
    return true;
}
void Npc::Copy(BaseObject* obj) const
{
    BaseObject::Copy(obj);
}

BOOL InitializeNPCOlcs()
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* ngroup = new OlcGroup();

    ngroup->SetInheritance(omanager->GetGroup(OLCGROUP::Entity));
    omanager->AddGroup(OLCGROUP::NPC, ngroup);
    return true;
}
