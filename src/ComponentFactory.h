/**
* Contains: Component Factory
*
* The factory is responsible for creating components upon request.
*/
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H
#include <string>
#include <map>
#include "mud.h"
#include "conf.h"
#include "component.h"
#include "componentMeta.hpp"

/**
* The factory holds, manages and creates components.
*/
class ComponentFactory
{
    std::map <std::string, IComponentMeta*> _components;
public:
    ComponentFactory() = default;
    ~ComponentFactory();
    /**
    * Checks for the existance of the specified component.
    *
    * @param [in] the name of the component.
    * @return true if the component was found, false otherwise.
    */
    BOOL HasComponent(const std::string &name);
    /**
    * Registers the component with the factory if it isn't already registered by the same name.
    *
    * @param [in] the name of the component.
    *@param [in] the component meta.
    * @return True on success, false on failure.
    */
    BOOL RegisterComponent(const std::string &name, IComponentMeta* meta);
    /**
    * Creates the specified component if it's in the registered components.
    *
    * @param [in] the name of the component.
    * @returns A newly created component if exists, nullptr otherwise.
    */
    Component* Create(const std::string &name);
    /**
    * Returns a component meta given the name of a component.
    *
    * @param [in] The name of the component meta.
    * @return the meta object if exists, nullptr otherwise.
    */
    IComponentMeta* GetMeta(const std::string &name);
};
#endif
