#include <tinyxml2.h>
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "alias.h"
#include "player.h"
#include "world.h"
#include "serializationHelpers.h"

Alias::Alias(const std::string& name):
    _name(name)
{
}

std::string Alias::GetName() const
{
    return _name;
}
void Alias::SetName(const std::string& name)
{
    _name = name;
}

void Alias::ProcessCommands(Player* mobile)
{
    World* world = World::GetPtr();

    for (auto it: _aliases)
        {
            world->DoCommand(mobile, it);
        }
}

void Alias::AddCommand(const std::string& command)
{
    if (!command.empty())
        {
            _aliases.push_back(command);
        }
}
void Alias::ClearCommands()
{
    _aliases.clear();
}
void Alias::ListCommands(std::vector<std::string>& commands)
{
    for (auto it: _aliases)
        {
            commands.push_back(it);
        }
}

void Alias::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->ToDocument();
    tinyxml2::XMLElement* alias = doc->NewElement("alias");

    alias->SetAttribute("name", _name.c_str());

    SerializeCollection<std::vector<std::string>, std::string>("commands", "command", root, _aliases, [this](tinyxml2::XMLElement* visitor, std::string command)
    {
        visitor->SetAttribute("value", command.c_str());
    });
    root->LinkEndChild(alias);
}
void Alias::Deserialize(tinyxml2::XMLElement* root)
{
    _name = root->Attribute("name");

    DeserializeCollection(root, "commands", [this](tinyxml2::XMLElement* visitor)
    {
        _aliases.push_back(visitor->Attribute("value"));
    });
}
