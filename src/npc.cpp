#include "npc.h"

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
