#include <tinyxml2.h>
#include <string>
#include <sstream>
#include <list>
#include "entity.h"
#include "baseObject.h"
#include "world.h"
#include "utils.h"
#include "olc.h"
#include "olcManager.h"
#include "editor.h"
#include "component.h"
#include "eventargs.h"
#include "serializationHelpers.h"

BaseObject::BaseObject()
{
    events.RegisterEvent("PreLook", new Event());
    events.RegisterEvent("PostLook", new Event());
    events.RegisterEvent("OnEnter", new Event());
    events.RegisterEvent("OnExit", new Event());
    events.RegisterEvent("OnLook", new Event());

    _name="A blank object";
    _desc="You see nothing special.";
    _script="";
    _onum=0;
    _zone = NULL;

//    variables.SetObject(this);
}
BaseObject::~BaseObject()
{
    for (auto it: _components)
        {
            delete it;
        }
}

std::string BaseObject::GetName() const
{
    return _name;
}
void BaseObject::SetName(const std::string &s)
{
    _name=s;
}

std::string BaseObject::GetDescription() const
{
    return _desc;
}
void BaseObject::SetDescription(const std::string &s)
{
    _desc=s;
}

std::string BaseObject::GetScript() const
{
    return _script;
}
void BaseObject::SetScript(const std::string &script)
{
    _script=script;
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

bool BaseObject::AddComponent(Component* component)
{
    if (component == nullptr)
        {
            return false;
        }
    if (HasComponent(component->GetMeta()->GetName()))
        {
            return false;
        }

    _components.push_back(component);
    component->Attach(this);
    return true;
}
bool BaseObject::RemoveComponent(Component* component)
{
    std::vector<Component*>::iterator it, itEnd;

    if (!HasComponent(component->GetMeta()->GetName()))
        {
            return false;
        }

    itEnd = _components.end();
    for (it = _components.begin(); it != itEnd; ++it)
        {
            if ((*it) == component)
                {
                    component->Detach();
                    _components.erase(it);
                    delete (*it);
                    return true;
                }
        }

    return false;
}
bool BaseObject::HasComponent(const std::string &name)
{
    for (auto it: _components)
        {
            if (it->GetMeta()->GetName()==name)
                {
                    return true;
                }
        }

    return false;
}
Component* BaseObject::GetComponent(const std::string &name)
{
    for (auto it: _components)
        {
            if (it->GetMeta()->GetName()==name)
                {
                    return it;
                }
        }

    return nullptr;
}

void BaseObject::Attach()
{
    for (auto it: _components)
        {
            it->Attach(this);
        }
}

void BaseObject::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement*node = doc->NewElement("BaseObject");
    tinyxml2::XMLElement* properties = doc->NewElement("properties");

    SerializeCollection<std::vector<Component*>, Component*>("components", "component", node, _components, [](tinyxml2::XMLElement* compelement, Component* compobj)
    {
        compelement->SetAttribute("name", compobj->GetMeta()->GetName().c_str());
        compobj->Serialize(compelement);
    });

//    variables.Serialize(properties);
    node->InsertEndChild(properties);

    node->SetAttribute("name", _name.c_str());
    node->SetAttribute("desc", _desc.c_str());
    node ->SetAttribute("script", _script.c_str());
    node->SetAttribute("onum", _onum);
    root->InsertEndChild(node);
}
void BaseObject::Deserialize(tinyxml2::XMLElement* root)
{
    World* world = World::GetPtr();
    tinyxml2::XMLElement* properties = NULL;

    DeserializeCollection(root, "components", [this, world](tinyxml2::XMLElement* visitor)
    {
        Component* com = world->CreateComponent(visitor->Attribute("name"));
        com->Deserialize(visitor);
        AddComponent(com);
    });

    properties = root->FirstChildElement("properties");
//    variables.Deserialize(properties);

    _name = root->Attribute("name");
    _desc = root->Attribute("desc");
    _script = root->Attribute("script");
    _onum = (unsigned int)root->IntAttribute("onum");
}

void BaseObject::Copy(BaseObject* obj) const
{
    /**
    * @todo move this to entity.
    */
    /*
        for (auto it: _aliases)
            {
                obj->AddAlias(it);
            }
    */

    obj->SetName(_name);
    obj->SetOnum(_onum);
    obj->SetDescription(_desc);
    obj->SetScript(_script);
}

std::string BaseObject::Identify(Player*mobile)
{
    std::stringstream st;
    st << GetName() << std::endl;
    st << "Originating zone: " << GetZone()->GetName() << std::endl;
    return st.str();
}

std::string BaseObject::DoLook(Player* mobile)
{
    std::string str;

    LookArgs* args=new LookArgs(mobile,this,str);
    events.CallEvent("PreLook", args, (void*)mobile);
    str+=Capitalize(this->GetName())+"\n";
    str+=this->GetDescription()+"\n";
    events.CallEvent("PostLook", args, (void*)mobile);
    delete args;
    return str;
}

bool BaseObject::IsNpc() const
{
    return false;
}
bool BaseObject::IsPlayer() const
{
    return false;
}
bool BaseObject::IsLiving() const
{
    if (IsPlayer() || IsNpc())
        {
            return true;
        }

    return false;
}
bool BaseObject::IsRoom() const
{
    return false;
}
bool BaseObject::IsObject() const
{
    return false;
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
    omanager->AddGroup(OLCGROUP::BaseObject, group);
    return true;
}
