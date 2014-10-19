#include <string>
#include <sstream>
#include <list>
#include <tinyxml.h>
#include "entity.h"
#include "baseObject.h"
#include "world.h"
#include "utils.h"
#include "olc.h"
#include "olcManager.h"
#include "editor.h"
#include "component.h"

BaseObject::BaseObject()
{
    _name="A blank object";
    _desc="You see nothing special.";
    _short = "A generic object lies here.";
    _script="";
    _onum=0;
    _components=new std::vector<Component*>();
    _aliases = new std::vector<std::string>();
    variables.SetObject(this);
    _persistent = true;
    _zone = NULL;
}
BaseObject::~BaseObject()
{
    std::vector<Component*>::iterator it, itEnd;

    itEnd = _components->end();
    for (it=_components->begin(); it != itEnd; ++it)
        {
            delete (*it);
        }

    delete _components;
    delete _aliases;
}

std::string BaseObject::GetName() const
{
    return _name;
}
void BaseObject::SetName(const std::string &s)
{
    _name=s;
}

std::string BaseObject::GetShort() const
{
    return _short;
}
void BaseObject::SetShort(const std::string &s)
{
    _short = s;
}

std::string BaseObject::GetDescription() const
{
    return _desc;
}
void BaseObject::SetDescription(const std::string &s)
{
    _desc=s;
}
std::string BaseObject::GetPlural() const
{
    return _plural;
}
void BaseObject::SetPlural(const std::string &s)
{
    _plural = s;
}

std::string BaseObject::GetScript() const
{
    return _script;
}
void BaseObject::SetScript(const std::string &script)
{
    _script=script;
}
BOOL BaseObject::GetPersistent() const
{
    return _persistent;
}
void BaseObject::SetPersistent(BOOL s)
{
    _persistent = s;
}

Zone* BaseObject::GetZone() const
{
    return _zone;
}
void BaseObject::SetZone(Zone* s)
{
    _zone = s;
}

VNUM BaseObject::GetOnum() const
{
    return _onum;
}
void BaseObject::SetOnum(VNUM num)
{
    _onum=num;
}

BOOL BaseObject::AddComponent(Component* component)
{
    if (component==NULL)
        {
            return false;
        }

    if (HasComponent(component->GetMeta()->GetName()))
        {
            return false;
        }
    _components->push_back(component);
    component->Attach(this);
    return true;
}
BOOL BaseObject::RemoveComponent(Component* component)
{
    std::vector<Component*>::iterator it, itEnd;

    if (!HasComponent(component->GetMeta()->GetName()))
        {
            return false;
        }

    itEnd = _components->end();
    for (it = _components->begin(); it != itEnd; ++it)
        {
            if ((*it) == component)
                {
                    component->Detach();
                    _components->erase(it);
                    delete (*it);
                    return true;
                }
        }

    return false; //should never happen.
}
bool BaseObject::HasComponent(const std::string &name)
{
    std::vector <Component*>::iterator it, itEnd;

    if (_components->size())
        {
            itEnd = _components->end();
            for (it = _components->begin(); it != itEnd; ++it)
                {
                    if ((*it)->GetMeta()->GetName()==name)
                        {
                            return true;
                        }
                }
        }

    return false;
}
Component* BaseObject::GetComponent(const std::string &name)
{
    std::vector <Component*>::iterator it, itEnd;

    if (_components->size())
        {
            itEnd = _components->end();
            for (it = _components->begin(); it != itEnd; ++it)
                {
                    if ((*it)->GetMeta()->GetName()==name)
                        {
                            return (*it);
                        }
                }
        }

    return NULL;
}

void BaseObject::Attach()
{
    Attach(this);
}
void BaseObject::Attach(BaseObject* obj)
{
    std::vector <Component*>::iterator it, itEnd;
    if (_components->size())
        {
            itEnd = _components->end();
            for (it = _components->begin(); it != itEnd; ++it)
                {
                    (*it)->Attach(obj);
                }
        }
}

BOOL BaseObject::AddAlias(const std::string &alias)
{
    if (AliasExists(alias) && alias != "")
        {
            return false;
        }

    _aliases->push_back(alias);
    return true;
}
BOOL BaseObject::AliasExists(const std::string & name)
{
    std::vector<std::string>::iterator it, itEnd;

    if (_aliases->size())
        {
            itEnd = _aliases->end();
            for (it = _aliases->begin(); it != itEnd; ++it)
                {
                    if ((*it) == name)
                        {
                            return true;
                        }
                }
        }

    return false;
}
std::vector<std::string>* BaseObject::GetAliases()
{
    return _aliases;
}

void BaseObject::Serialize(TiXmlElement* root)
{
    TiXmlElement*node = new TiXmlElement("BaseObject");
    TiXmlElement *components = new TiXmlElement("components");
    TiXmlElement* aliases = new TiXmlElement("aliases");
    TiXmlElement* alias = NULL;
    TiXmlElement* component = NULL;
    TiXmlElement* properties = new TiXmlElement("properties");
    std::vector<std::string>::iterator ait, aitEnd;
    std::vector <Component*>::iterator it, itEnd;
    itEnd = _components->end();

    if (_components->size())
        {
            for (it=_components->begin(); it != itEnd; ++it)
                {
                    component = new TiXmlElement("component");
                    component->SetAttribute("name", (*it)->GetMeta()->GetName().c_str());
                    (*it)->Serialize(component);
                    components->LinkEndChild(component);
                }
        }
    node->LinkEndChild(components);

    if (_aliases->size())
        {
            aitEnd = _aliases->end();
            for (ait = _aliases->begin(); ait != aitEnd; ++ait)
                {
                    alias = new TiXmlElement("alias");
                    alias->SetAttribute("name", (*ait).c_str());
                    aliases->LinkEndChild(alias);
                }
        }
    node->LinkEndChild(aliases);

    variables.Serialize(properties);
    node->LinkEndChild(properties);

    node->SetAttribute("name", _name.c_str());
    node->SetAttribute("desc", _desc.c_str());
    node ->SetAttribute("script", _script.c_str());
    node->SetAttribute("plural", _plural.c_str());
    node->SetAttribute("onum", _onum);
    root->LinkEndChild(node);
}
void BaseObject::Deserialize(TiXmlElement* root)
{
    World* world = World::GetPtr();

    TiXmlElement* components = NULL;
    TiXmlElement* component = NULL;
    TiXmlElement* alias = NULL;
    TiXmlElement*aliases = NULL;
    TiXmlElement* properties = NULL;
    TiXmlNode* node = NULL;
    Component* com = NULL;

    node = root->FirstChild("components");
    if (node)
        {
            components = node->ToElement();
            for (node = components->FirstChild(); node; node = node->NextSibling())
                {
                    component = node->ToElement();
                    com = world->CreateComponent(component->Attribute("name"));
                    com->Deserialize(component);
                    AddComponent(com);
                }
        }

    node = root->FirstChild("aliases");
    if (node)
        {
            aliases = node->ToElement();
            for (node = aliases->FirstChild(); node; node = node->NextSibling())
                {
                    alias=node->ToElement();
                    AddAlias(alias->Attribute("name"));
                }
        }

    node = root->FirstChild("properties");
    if (node)
        {
            properties = node->ToElement();
            variables.Deserialize(properties);
        }

    _name = root->Attribute("name");
    _desc = root->Attribute("desc");
    _script = root->Attribute("script");
    _plural = root->Attribute("plural");
    root->Attribute("onum", (unsigned int*)&_onum);
}

void BaseObject::Copy(BaseObject* obj) const
{
    std::vector<std::string>::iterator it, itEnd;
    it = _aliases->end();

    if (_aliases->size())
        {
            for (it = _aliases->begin(); it != itEnd; ++it)
                {
                    obj->AddAlias((*it));
                }
        }

    obj->SetName(_name);
    obj->SetOnum(_onum);
    obj->SetShort(_short);
    obj->SetDescription(_desc);
    obj->SetScript(_script);
    obj->SetPlural(_plural);
    obj->SetPersistent(_persistent);
}

std::string BaseObject::Identify(Player*mobile)
{
    std::stringstream st;
    st << GetName() << std::endl;
    st << "Short description: " << GetShort() << std::endl;
    st << "Persistent: " << (GetPersistent()? "yes." : "no.") << std::endl;
    st << "Origenating zone: " << GetZone()->GetName() << "." << std::endl;
    return st.str();
}

bool InitializeBaseObjectOlcs()
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = new OlcGroup();

    group->AddEntry(new OlcStringEntry<BaseObject>("name", "the name of the object", OF_NORMAL, OLCDT::STRING,
                    std::bind(&BaseObject::GetName, std::placeholders::_1),
                    std::bind(&BaseObject::SetName, std::placeholders::_1, std::placeholders::_2)));
    group->AddEntry(new OlcEditorEntry<BaseObject>("description", "the description of the object", OF_NORMAL, OLCDT::EDITOR,
                    std::bind(&BaseObject::GetDescription, std::placeholders::_1),
                    std::bind(&BaseObject::SetDescription, std::placeholders::_1, std::placeholders::_2)));
    group->AddEntry(new OlcStringEntry<BaseObject>("short", "the title of the object seen in rooms", OF_NORMAL, OLCDT::STRING,
                    std::bind(&BaseObject::GetShort, std::placeholders::_1),
                    std::bind(&BaseObject::SetShort, std::placeholders::_1, std::placeholders::_2)));
    group->AddEntry(new OlcStringEntry<BaseObject>("plural", "the plural of multiple objects", OF_NORMAL, OLCDT::STRING,
                    std::bind(&BaseObject::GetPlural, std::placeholders::_1),
                    std::bind(&BaseObject::SetPlural, std::placeholders::_1, std::placeholders::_2)));

    omanager->AddGroup(OLCGROUP::BaseObject, group);
    return true;
}
