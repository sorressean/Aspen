#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "alias.h"
#include "player.h"
#include "world.h"

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

void Alias::Serialize(TiXmlElement* root)
{
    TiXmlElement* alias = new TiXmlElement("alias");
    TiXmlElement* commands = new TiXmlElement("commands");
    alias->SetAttribute("name", _name.c_str());

    for (auto it: _aliases)
        {
            TiXmlElement* command = new TiXmlElement("command");
            command->SetAttribute("value", it.c_str());
            commands->LinkEndChild(command);
        }
    alias->LinkEndChild(commands);
    root->LinkEndChild(alias);
}
void Alias::Deserialize(TiXmlElement* root)
{
    TiXmlElement* commands = nullptr;
    TiXmlNode* node = NULL;
    std::string val;
    _name = root->Attribute("name");

    commands = root->FirstChild("commands")->ToElement();
    for (node = commands->FirstChild(); node; node = node->NextSibling())
        {
            TiXmlElement* command = node->ToElement();
            val = command->Attribute("value");
            _aliases.push_back(val);
        }
}
