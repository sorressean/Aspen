/*
*Component meta data.
*Used to hold global properties for all components.
*Includes OLC, so that OLC entries do not have to be defined on each instance of the component, as well as
*instance data specific to components.
*/
#pragma once
#ifndef COMPONENT_META_H
#define COMPONENT_META_H
#include "mud.h"
#include "conf.h"
#include "olc.h"

class ComponentMeta
{
    std::string _name;
    std::vector <std::string> _dependencies; //component dependencies.
public:
    ComponentMeta(const std::string &name);
    virtual ~ComponentMeta();
    void SetName(const std::string &name);
    std::string GetName() const;
    /*
    *Checks to see if the specified dependency is located in the dependencies list.
    *Param: [in] the name of the dependency.
    *Return: True if the dependency exists in the dependencies list, false otherwise.
    */
    BOOL DependencyExists(const std::string &name) const;
    /*
    *Will add the specified dependency to the list.
    *Param: [in] The name of the component to add as a dependency.
    */
    BOOL AddDependency(const std::string &dependency);
    /*
    *Returns a pointer to a vector of dependency names.
    *Return: a pointer to the vector of strings containing the names of the dependencies associated with the component.
    */
    void GetDependencies(std::vector<std::string>*) const;
    /*
    *Called to create an instance of the component.
    *Param: [in] a pointer to the instance of the ComponentMeta object responsible for the component.
    */
    virtual Component* Create(ComponentMeta* meta) = 0;
    /*
    *Does all the initialization for a component once it's added
    *to the component factory.
    */
    virtual void Initialize() = 0;
    virtual OLCGROUP GetOlcGroup() const;
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
};
#endif
