#include <map>
#include <string>
#include "ComponentFactory.h"
#include "component.h"
#include "componentMeta.hpp"

ComponentFactory::~ComponentFactory()
{

    for (auto it: _components)
        {
            delete it.second;
        }
}

BOOL ComponentFactory::HasComponent(const std::string &name)
{
    return (_components.count(name)==0?false:true);
}

BOOL ComponentFactory::RegisterComponent(const std::string &name, IComponentMeta* meta)
{
    if (HasComponent(name))
        {
            return false;
        }

    _components[name]=meta;
    meta->Initialize();
    return true;
}

Component* ComponentFactory::Create(const std::string &name)
{
    if (HasComponent(name))
        {
            return _components[name]->Create();
        }

    return nullptr;
}

IComponentMeta* ComponentFactory::GetMeta(const std::string &name)
{
    if (HasComponent(name))
        {
            return _components[name];
        }

    return nullptr;
}
