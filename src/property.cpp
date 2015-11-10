#include <tinyxml2.h>
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "property.h"
#include "variant.h"
#include "serializationHelpers.h"


Property::Property(const std::string &name, const Variant& value, Property* parent):
    _name(name), _value(value), _parent(parent)
{
}
Property::Property():
    _name("root"), _parent(NULL)
{
    _value = Variant();
    _owner = NULL;
}
Property::~Property()
{
    RemoveSelf();
}

Variant Property::GetValue()
{
    return _value;
}
Variant& Property::GetPropertyRef(const std::string &name)
{
    Property *prop = FindProperty(name);
    if (! prop)
        {
            throw std::runtime_error("Property not found: Searched for "+name+".");
        }
    else
        {
            return prop->_value;
        }
}

void Property::SetValue(const Variant& value)
{
    _value = value;
}
void Property::SetObject(BaseObject* obj)
{
    _owner = obj;
}

BaseObject* Property::GetOwner() const
{
    return _owner;
}

Property* Property::GetParent() const
{
    return _parent;
}
void Property::SetParent(Property* parent)
{
    _parent = parent;
}

void Property::AddProperty(Property* prop)
{
    prop->SetParent(this);
    _children.push_back(prop);
}
Property* Property::AddProperty(const std::string &name, const Variant& value)
{
    size_t dotInd = name.find_first_of(".");
    Property *prop = NULL;

    if (dotInd != std::string::npos)
        {
            std::string childPart = name.substr(0, dotInd);
            std::string newName = name.substr(dotInd + 1);

            // first we need to make sure that childPart exists.
            prop = FindProperty(childPart);
            if (! prop)
                {
                    prop = new Property(childPart, Variant(), this);
                    _children.push_back(prop);
                    return prop->AddProperty(newName, value);
                }
        }
    else
        {
            // we're at the last dot, we can create the node
            prop = new Property(name, value, this);
            _children.push_back(prop);
            return prop;
        }

    return prop;
}

void Property::RemoveSelf()
{
    if (! _parent)
        return;

    _parent->RemoveProperty(this);
    _parent = NULL;
}
void Property::RemoveProperty(const std::string &name)
{
    Property *prop = FindProperty(name);
    if (! prop)
        return;
    std::vector<Property*>::iterator it, itEnd;

    itEnd = prop->_children.end();
    for (it = prop->_children.begin(); it != itEnd; ++it)
        {
            RemoveProperty(*it);
        }
}
void Property::RemoveProperty(Property* prop)
{
    if (! prop)
        return;

    // first we have to remove all of its children first
    std::vector<Property*>::iterator it, itEnd;

    itEnd = prop->_children.end();
    for (it = prop->_children.end(); it != itEnd; ++it)
        {
            Property *tmp = *it;
            prop->RemoveProperty(tmp);
            it = _children.erase(it);
            delete prop;
        }
}

Property* Property::FindProperty(const std::string &name)
{
    std::vector<Property*>::iterator it, itEnd;
    // first check cache
    /*
      if (name == _lastSearchName)
        return _lastSearchRes;
    */

    size_t dotInd = name.find_first_of(".");
    if (dotInd != std::string::npos)
        {
            std::string childName = name.substr(0, dotInd);
            std::string newName = name.substr(dotInd + 1);

            itEnd = _children.end();
            for (it = _children.begin(); it != itEnd; ++it)
                {
                    if ( (*it)->_name == childName )
                        {
                            return ( (*it)->FindProperty(newName) );
                        }
                }
        }

    itEnd = _children.end();
    for (it = _children.begin(); it != itEnd; ++it)
        {
            if ( (*it)->_name == name )
                {
                    return *it;
                }
        }

    return NULL;
}

void Property::Serialize(tinyxml2::XMLElement *root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement *prop = doc->NewElement("property");

    if (_children.size())
        {
            SerializeList<Property, std::vector<Property*>>("property", root, _children);
        }

    prop->SetAttribute("name", _name.c_str());
    _value.Serialize(prop);
}
void Property::Deserialize(tinyxml2::XMLElement* root, Property* parent)
{
    tinyxml2::XMLElement* property = nullptr;
    tinyxml2::XMLElement* child = nullptr;
    Property prop;
    const char* val = nullptr;

    val = root->Attribute("name");
    if (val)
        {
            _name = val;
        }

    property = root->FirstChildElement("property");
    if (property)
        {
            _value.Deserialize(property);
        }

    for (child = root->FirstChildElement("variable"); child; child = child->NextSiblingElement())
        {
            prop = Property();
            prop.Deserialize(child, this);
        }

    SetParent(parent);
}
