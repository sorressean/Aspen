#pragma once
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "player.h"

class Alias
{
    std::vector<std::string> _aliases;
    std::string _name;
public:
    Alias();
    Alias(const std::string& name);
    ~Alias();
    std::string GetName() const;
    void SetName(const std::string& name);
    void ProcessCommands(Player* mobile);
    void AddCommand(const std::string& command);
    void ClearCommands();
    void ListCommands(std::vector<std::string>& commands);
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
};
