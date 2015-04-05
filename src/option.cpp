#include "mud.h"
#include "conf.h"
#include "variant.h"
#include "world.h"
#include "option.h"
#include "optionMeta.h"
#include "optionManager.h"
Option::Option(OptionMeta* meta, const Variant& value)
{
    _meta = meta;
    _value = value;
}
Option::~Option()
{
}

OptionMeta* Option::GetMeta() const
{
    return _meta;
}
void Option::SetMeta(OptionMeta* meta)
{
    _meta = meta;
}
Variant& Option::GetValue()
{
    return _value;
}
void Option::SetValue(const Variant& value)
{
    _value = value;
}

Variant GetOptionValue(const std::string &name, const Player* mobile)
{
    World* world = World::GetPtr();
    OptionManager* omanager = world->GetOptionManager();

    if (mobile->OptionExists(name))
        {
            return mobile->GetOption(name)->GetValue();
        }
    else if (omanager->OptionExists(name))
        {
            return omanager->GetOption(name)->GetValue();
        }

    return Variant();
}