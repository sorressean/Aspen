#include <tinyxml2.h>
#include <vector>
#include "component.h"
#include "exception.h"
#include "eventargs.h"
#include "world.h"

void Component::Initialize()
{
    _attached = false;

    events.RegisterEvent("OnCreate");
    events.RegisterEvent("OnDestroy");
    events.RegisterEvent("OnAttach");
    events.RegisterEvent("OnDetach");
    events.CallEvent("OnCreate", nullptr, (void*)this);
}

Component::Component(IComponentMeta* parent):_parent(parent)
{
}
Component::Component()
{
    _parent = nullptr;
}
Component::~Component()
{
    events.CallEvent("OnDestroy", nullptr, (void*)this);
}

void Component::Serialize(tinyxml2::XMLElement* root)
{
    root->SetAttribute("name", _parent->GetName().c_str());
    variables.Serialize(root);
}
void Component::Deserialize(tinyxml2::XMLElement* root)
{
//we need to pull the name in entity when we deserialize, so we don't need that here.
    variables.Deserialize(root);
}

void Component::SetObject(BaseObject* obj)
{
    _object=obj;
}
BaseObject* Component::GetObject() const
{
    return _object;
}

IComponentMeta* Component::GetMeta()
{
    return _parent;
}

void Component::Attach(BaseObject* obj)
{
    World* world = World::GetPtr();
    std::vector<std::string> dependencies;

    if (_attached)
        {
            return;
        }

    _parent->GetDependencies(&dependencies);
    for (auto it: dependencies)
        {
            obj->AddComponent(world->CreateComponent(it));
        }

    ComponentAttachedArgs arg(obj);
    events.CallEvent("OnAttach", &arg, (void*)this);
    _attached = true;
    SetObject(obj);
}
void Component::Detach()
{
    if (!_attached)
        {
            return;
        }

    SetObject(nullptr);
    _attached = false;
    events.CallEvent("OnDetach", nullptr, (void*) this);
}
