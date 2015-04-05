#pragma once
#include <string>
#include "mud.h"
#include "conf.h"
#include "variant.h"

enum class OptionSection
{
    Debug,
    Misc,
    Alog,
    Channel
};

class OptionMeta
{
    std::string _name;
    std::string _help;
    OptionSection _section;
    FLAG _access;
    bool _toggle;
    Variant _value;
    VARIABLE_TYPE _expected;
public:
    OptionMeta();
    ~OptionMeta();
    std::string GetName() const;
    void SetName(const std::string& name);
    std::string GetHelp() const;
    void SetHelp(const std::string& help);
    OptionSection GetSection() const;
    void SetSection(OptionSection section);
    FLAG GetAccess() const;
    void SetAccess(FLAG access);
    Variant& GetValue();
    void SetValue(const Variant& value);
    VARIABLE_TYPE GetExpected() const;
    void SetExpected(VARIABLE_TYPE expected);
    bool CanToggle() const;
    void SetToggle(bool toggle);
    std::string GetFullName() const;
};

std::string OptionSectionToString(OptionSection section);