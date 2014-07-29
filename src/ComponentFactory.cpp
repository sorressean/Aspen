#include <map>
#include "ComponentFactory.h"
#include "component.h"
#include "componentMeta.hpp"

ComponentFactory::ComponentFactory()
{
    _components = new std::map <std::string, IComponentMeta*>();
}
ComponentFactory::~ComponentFactory()
{
    std::map <std::string, IComponentMeta*>::iterator it, itEnd;

    itEnd = _components->end();
    for (it = _components->begin(); it != itEnd; ++it)
        {
            delete (*it).second;
        }

    delete _components;
}

BOOL ComponentFactory::HasComponent(const std::string &name)
{
    return (_components->count(name)==0?false:true);
}

BOOL ComponentFactory::RegisterComponent(const std::string &name, IComponentMeta* meta)
{
    if (HasComponent(name))
        {
            return false;
        }

    (*_components)[name]=meta;
    meta->Initialize();
    return true;
}

Component* ComponentFactory::Create(const std::string &name)
{
    if (HasComponent(name))
        {
            return (*_components)[name]->Create();
        }

    return NULL;
}
IComponentMeta* ComponentFactory::GetMeta(const std::string &name)
{
    if (HasComponent(name))
        {
            return (*_components)[name];
        }

    return NULL;
}
