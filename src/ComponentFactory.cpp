#include <map>
#include "ComponentFactory.h"
#include "component.h"
#include "componentMeta.h"

ComponentFactory::ComponentFactory()
{
    _components = new std::map <std::string, ComponentMeta*>();
}
ComponentFactory::~ComponentFactory()
{
    std::map <std::string, ComponentMeta*>::iterator it, itEnd;

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

BOOL ComponentFactory::RegisterComponent(const std::string &name, ComponentMeta* meta)
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
            return (*_components)[name]->Create((*_components)[name]);
        }

    return NULL;
}
ComponentMeta* ComponentFactory::GetMeta(const std::string &name)
{
    if (HasComponent(name))
        {
            return (*_components)[name];
        }

    return NULL;
}
