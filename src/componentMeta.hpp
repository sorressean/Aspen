/*
*Component meta data.
*Used to hold global properties for all components.
*Includes OLC, so that OLC entries do not have to be defined on each instance of the component, as well as
*instance data specific to components.
*/
#pragma once
#ifndef COMPONENT_META_H
#define COMPONENT_META_H
#include <tinyxml2.h>
#include <algorithm>
#include <string>
#include <vector>
#include "mud.h"
#include "conf.h"
#include "olcGroup.h"
#include "olc.h"
#include "serializer.h"
#include "serializationHelpers.h"

class IComponentMeta:public ISerializable
{
public:
    virtual ~IComponentMeta() { }
    virtual Component* Create() = 0;
    virtual void Initialize() = 0;
    virtual std::string GetName() const = 0;
    virtual     BOOL AddDependency(const std::string &dependency) = 0;
    virtual     void GetDependencies(std::vector<std::string>* out) const = 0;
    virtual OLCGROUP GetOlcGroup() const = 0;
};

template <class T>
class ComponentMeta:public IComponentMeta
{
    std::string _name;
    std::vector <std::string> _dependencies; //component dependencies.
public:
    ComponentMeta(const std::string &name)
    {
        _name = name;
    }
    ~ComponentMeta()
    {
    }
    void SetName(const std::string &name)
    {
        _name = name;
    }
    std::string GetName() const
    {
        return _name;
    }
    /*
    *Checks to see if the specified dependency is located in the dependencies list.
    *Param: [in] the name of the dependency.
    *Return: True if the dependency exists in the dependencies list, false otherwise.
    */
    BOOL DependencyExists(const std::string &name) const
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

    /*
    *Will add the specified dependency to the list.
    *Param: [in] The name of the component to add as a dependency.
    */
    BOOL AddDependency(const std::string &dependency)
    {
        if (DependencyExists(dependency))
            {
                return false;
            }

        _dependencies.push_back(dependency);
        return true;
    }

    /*
    *Returns a pointer to a vector of dependency names.
    *Return: a pointer to the vector of strings containing the names of the dependencies associated with the component.
    */
    void GetDependencies(std::vector<std::string>* out) const
    {
        std::copy(_dependencies.begin(), _dependencies.end(), out->end());
    }

    /*
    *Called to create an instance of the component.
    *Param: [in] a pointer to the instance of the ComponentMeta object responsible for the component.
    */
    virtual T* Create()
    {
        T* ret = new T(this);
        ret->Initialize();
        return ret;
    }

    /*
    *Does all the initialization for a component once it's added
    *to the component factory.
    */
    virtual void Initialize()
    {
    }

    virtual OLCGROUP GetOlcGroup() const
    {
        return OLCGROUP::NONE;
    }
    virtual void Serialize(tinyxml2::XMLElement* root)
    {
        tinyxml2::XMLDocument* doc = root->GetDocument();
        tinyxml2::XMLElement* ent = doc->NewElement("cmeta");
        ent->SetAttribute("name", _name.c_str());

        SerializeList<std::string, std::vector<std::string>>("dependencies", "dependency", ent, _dependencies);
        root->InsertEndChild(ent);
    }
    virtual void Deserialize(tinyxml2::XMLElement* root)
    {
        _name = root->Attribute("name");

        DeserializeCollection(root, "dependencies", [this](tinyxml2::XMLElement* visitor)
        {
            AddDependency(visitor->Attribute("name"));
        });
    }
};
#endif
