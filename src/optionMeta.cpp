#include <string>
#include "mud.h"
#include "conf.h"
#include "optionMeta.h"

OptionMeta::OptionMeta()
{
    _section = OptionSection::Misc;
    _access = 0;
    _toggle = false;
    _expected = VAR_INT;
}
OptionMeta::~OptionMeta()
{
}

std::string OptionMeta::GetName() const
{
    return _name;
}
void OptionMeta::SetName(const std::string& name)
{
    _name = name;
}
std::string OptionMeta::GetHelp() const
{
    return _help;
}
void OptionMeta::SetHelp(const std::string& help)
{
    _help = help;
}
OptionSection OptionMeta::GetSection() const
{
    return _section;
}
void OptionMeta::SetSection(OptionSection section)
{
    _section = section;
}
FLAG OptionMeta::GetAccess() const
{
    return _access;
}
void OptionMeta::SetAccess(FLAG access)
{
    _access = access;
}
Variant& OptionMeta::GetValue()
{
    return _value;
}
void OptionMeta::SetValue(const Variant& value)
{
    _value = value;
}
VARIABLE_TYPE OptionMeta::GetExpected() const
{
    return _expected;
}
void OptionMeta::SetExpected(VARIABLE_TYPE expected)
{
    _expected = expected;
}
bool OptionMeta::CanToggle() const
{
    return _toggle;
}
void OptionMeta::SetToggle(bool toggle)
{
    _toggle = toggle;
}

std::string OptionMeta::GetFullName() const
{
    std::string ret;
    ret = GetName();
    ret += ".";
    ret += OptionSectionToString(GetSection());
    return ret;
}

std::string OptionSectionToString(OptionSection section)
{
    switch(section)
        {
        case OptionSection::Debug:
            return "debug";
        case OptionSection::Misc:
            return "misc";
        case OptionSection::Alog:
            return "alog";
        case OptionSection::Channel:
            return "channel";
        default:
            return "unknown";
        }
}
