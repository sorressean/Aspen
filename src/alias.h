#pragma once
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"

class Alias
{
    std::vector<std::string> _aliases;
    std::string _name;
public:
    Alias();
    Alias(const std::string& name);
    ~Alias();
    std::string GetName();
    void setName(const std::string& name);
    void processCommands();
    void AddCommand(const std::string& command);
    void ClearCommands();
    void ListCommands(const std::vector<std::string>& commands);
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
};
