#include <tinyxml2.h>
#include <functional>
#include <stdexcept>
#include <string>
#include "mud.h"
#include "conf.h"
#include "serializationHelpers.h"

bool SerializeLong(tinyxml2::XMLElement* parent, unsigned long long int val)
{
    if (parent == nullptr)
        {
            return false;
        }
    if (val == 0)
        {
            parent->SetAttribute("a", 0);
            parent->SetAttribute("b", 0);
            return true;
        }

    unsigned int a = 0;
    unsigned int b = 0;
    b = val;
    a = val >> 32;
    parent->SetAttribute("a", a);
    parent->SetAttribute("b", b);
    return true;
}
unsigned long long int DeserializeLong(tinyxml2::XMLElement* parent)
{
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned long long int ret = 0;

    a = parent->IntAttribute("a");
    b = parent->IntAttribute("b");
    ret = a;
    ret <<=32;
    ret |= (0xFFFFFFFF&b);
    return ret;
}

void DeserializeCollection(tinyxml2::XMLElement* root, const std::string& name, std::function<void (tinyxml2::XMLElement*)> callback)
{
    tinyxml2::XMLElement* ent = nullptr;
    tinyxml2::XMLElement* visit = nullptr;

    ent = root->FirstChildElement(name.c_str());
    if (!ent)
        {
            throw(std::runtime_error("Could not deserialize "+std::string(name)+": no such element found."));
        }

    for (visit = ent->FirstChildElement(); visit; visit = visit->NextSiblingElement())
        {
            callback(visit);
        }
}
