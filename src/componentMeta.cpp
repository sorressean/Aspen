#include "mud.h"
#include "conf.h"
#include "componentMeta.h"

ComponentMeta::ComponentMeta(const std::string &name)
{
    _name=name;
}
ComponentMeta::~ComponentMeta()
{
}

void ComponentMeta::SetName(const std::string &name)
{
    _name=name;
}
std::string ComponentMeta::GetName() const
{
    return _name;
}

BOOL ComponentMeta::DependencyExists(const std::string &name) const
{
    std::vector<std::string>::const_iterator it, itEnd;

    itEnd = _dependencies.end();
    for (it = _dependencies.begin(); it != itEnd; ++it)
        {
            if ((*it) == name)
                {
                    return true;
                }
        }

    return false;
}
BOOL ComponentMeta::AddDependency(const std::string &name)
{
    if (DependencyExists(name))
        {
            return false;
        }

    _dependencies.push_back(name);
    return true;
}
void ComponentMeta::GetDependencies(std::vector<std::string>* out) const
{
    std::copy(_dependencies.begin(), _dependencies.end(), out->end());
}

OLCGROUP ComponentMeta::GetOlcGroup() const
{
    return OLCGROUP::NONE;
}

void ComponentMeta::Serialize(TiXmlElement* root)
{
    TiXmlElement* ent = new TiXmlElement("cmeta");
    TiXmlElement* dependencies = new TiXmlElement("dependencies");
    TiXmlElement* dependency = NULL;
    std::vector<std::string>::iterator it, itEnd;

    ent->SetAttribute("name", _name.c_str());

    itEnd = _dependencies.end();
    for (it = _dependencies.begin(); it != itEnd; ++it)
        {
            dependency = new TiXmlElement("dependency");
            dependency->SetAttribute("name", (*it).c_str());
            dependencies->LinkEndChild(dependency);
        }
    ent->LinkEndChild(dependencies);

    root->LinkEndChild(ent);
}
void ComponentMeta::Deserialize(TiXmlElement* root)
{
    TiXmlNode* node = NULL;
    TiXmlElement* dependency = NULL;
    TiXmlElement *dependencies = NULL;
    _name = root->Attribute("name");

    node = root->FirstChild("dependencies");
    if (node)
        {
            dependencies = node->ToElement();
            for (node = dependencies->FirstChild(); node; node = node->NextSibling())
                {
                    dependency = node->ToElement();
                    AddDependency(dependency->Attribute("name"));
                }
        }
}
