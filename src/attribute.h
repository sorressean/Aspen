#pragma once
#include <string>
#include "mud.h"
#include  "conf.h"

enum class AttributeType
{
    Health, Mana,
    MaxHealth, MaxMana,
    Dex, Con, Str, Int, Wis,
    MagicalArmor, PhysicalArmor
};
enum class AttributeApplication
{
    Talent,
    Perk,
    Affect,
    Item
};
class Attribute
{
    AttributeType _type;
    AttributeApplication _application;
    int _mod;
    int _id;
public:
    Attribute(AttributeType type, AttributeApplication application, int mod, int id=0);
    ~Attribute() = default;
    AttributeType GetType() const;
    AttributeApplication GetApply() const;
    int GetModifier() const;
    int GetId() const;
    static int GetAttributePoints(AttributeType t);
    static std::string ToString(AttributeType t);
};
