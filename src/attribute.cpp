#include <string>
#include "mud.h"
#include "attribute.h"
#include "world.h"
#include "tableManager.hpp"
#include "eventTable.h"

Attribute::Attribute(int type, int application, int mod, int id):
    _type(type), _apply(application), _mod(mod), _id(id)
{
}

int Attribute::GetType() const
{
    return _type;
}
int Attribute::GetApply() const
{
    return _apply;
}
int Attribute::GetModifier() const
{
    return _mod;
}
int Attribute::GetId() const
{
    return _id;
}

EVENT(CleanupAttributes)
{
    World* world = World::GetPtr();
    AttributeTable* attributes = (AttributeTable*)world->GetProperty("attributes");

    if (attributes)
        {
            delete attributes;
        }
}

void InitializeAttributes()
{
    World* world = World::GetPtr();
    AttributeTable* attributes = new AttributeTable();
EventTable* events = EventTable::GetPtr();;

    world->AddProperty("attributes", (void*)attributes);
events->AddCallback(GEVENT_SHUTDOWN, CleanupAttributes);
}

AttributeTable* GetAttributeTable()
{
    World* world = World::GetPtr();

    return (AttributeTable*)world->GetProperty("attributes");
}
