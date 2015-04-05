#include <vector>
#include <unordered_map>
#include <algorithm>
#include "mud.h"
#include "conf.h"
#include "option.h"
#include "optionMeta.h"
#include "optionManager.h"

OptionManager::OptionManager()
{
}
OptionManager::~OptionManager()
{
    for (auto it: _options)
        {
            delete it.second;
        }
}

bool OptionManager::OptionExists(const std::string& name) const
{
    return (_options.count(name) ? true : false);
}
bool OptionManager::ListOptions(OptionSection section, std::vector<OptionMeta*>* opts)
{
    std::vector<OptionMeta*> source = _sections[(int)section];
    if (!_sections.count((int)section))
        {
            return false;
        }

    for (auto it:source)
        {
            opts->push_back(it);
        }

    return true;
}
bool OptionManager::AddOption(OptionMeta* option)
{
    // std::string fullname;
    if (!option)
        {
            return false;
        }

    //fullname = OptionSectionToString(option->GetSection()) + "." + option->GetName();
    if (OptionExists(option->GetName()))
        {
            return false;
        }

    _options[option->GetName()] = option;
    _sections[(int)option->GetSection()].push_back(option);

    return true;
}
OptionMeta* OptionManager::GetOption(const std::string& name)
{
    if (!OptionExists(name))
        {
            return nullptr;
        }

    return _options[name];
}
void OptionManager::ListOptions(std::vector<OptionMeta*>* options) const
{
    for (auto it:_options)
        {
            options->push_back(it.second);
        }
}
