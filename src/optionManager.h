/*
*Our option manager holds and keeps track of global options.
*/
#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "option.h"
#include "optionMeta.h"
#include "property.h"

class OptionManager
{
    std::unordered_map<int, std::vector<OptionMeta*>> _sections;
    std::unordered_map<std::string, OptionMeta*> _options;
public:
    OptionManager();
    ~OptionManager();
    bool OptionExists(const std::string& name) const;
    bool ListOptions(OptionSection section, std::vector<OptionMeta*>* opts);
    bool AddOption(OptionMeta* option);
    OptionMeta* GetOption(const std::string &name);
    void ListOptions(std::vector<OptionMeta*>* options) const;
};
