#include <string>
#include "mud.h"
#include "conf.h"
#include "attribute.h"
#include "world.h"

Attribute::Attribute(AttributeType type, AttributeApplication application, int mod, int id):
    _type(type), _application(application), _mod(mod), _id(id)
{
}

AttributeType Attribute::GetType() const
{
    return _type;
}
AttributeApplication Attribute::GetApply() const
{
    return _application;
}
int Attribute::GetModifier() const
{
    return _mod;
}
int Attribute::GetId() const
{
    return _id;
}

int Attribute::GetAttributePoints(AttributeType  t)
{
    World* world = World::GetPtr();

    switch(t)
        {
        default:
            world->WriteLog("Tried to convert invalid attribute type.");
            return 0;
        case AttributeType::Health:
        case AttributeType::Mana:
        case AttributeType::MaxHealth:
        case AttributeType::MaxMana:
            return 1;
        case AttributeType::Dex:
        case AttributeType::Con:
        case AttributeType::Str:
        case AttributeType::Int:
        case AttributeType::Wis:
            return 5;
        case AttributeType::MagicalArmor:
        case AttributeType::PhysicalArmor:
            return 4;
        }
}

std::string Attribute::ToString(AttributeType t)
{
    switch(t)
        {
        default:
            return "unknown";
        case AttributeType::Health:
            return "health";
        case AttributeType::Mana:
            return "mana";
        case AttributeType::MaxHealth:
            return "max health";
        case AttributeType::MaxMana:
            return "max mana";
        case AttributeType::Dex:
            return "dexterity";
        case AttributeType::Con:
            return "constitution";
        case AttributeType::Str:
            return "strength";
        case AttributeType::Int:
            return "intelligence";
        case AttributeType::Wis:
            return "wisdom";
        case AttributeType::MagicalArmor:
            return "magical armor";
        case AttributeType::PhysicalArmor:
            return "physical armor";
        }
}
