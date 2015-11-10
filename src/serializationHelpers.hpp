#pragma once
#include <tinyxml2.h>
#include <functional>
#include <string>

bool SerializeLong(tinyxml2::XMLElement* parent, unsigned long long int val);
unsigned long long int DeserializeLong(tinyxml2::XMLElement* parent);
void DeserializeCollection(tinyxml2::XMLElement* root, const std::string& name, std::function<void (tinyxml2::XMLElement*)> cb);

template <class O, class C>
bool SerializeList(const std::string& parent, const std::string& nodename, tinyxml2::XMLElement* root, C& container)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* pnode = doc->NewElement(parent.c_str());
    tinyxml2::XMLElement* cur = nullptr;

    for (auto it: container)
        {
            cur = doc->NewElement(nodename.c_str());
            it->Serialize(cur);
            pnode->InsertEndChild(cur);
        }

    root->InsertEndChild(pnode);
    return true;
}
template <class O, class C>
bool SerializeList(const std::string& parent, tinyxml2::XMLElement* root, C& container)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* pnode = doc->NewElement(parent.c_str());

    for (auto it: container)
        {
            it->Serialize(pnode);
        }
    root->InsertEndChild(pnode);
    return true;
}

template <class O, class C>
bool DeserializeList(tinyxml2::XMLElement* root, const std::string &parent, C& objects)
{
    DeserializeCollection(root, parent, [&objects](tinyxml2::XMLElement* visitor) mutable
    {
        O* obj = new O();
        obj->Deserialize(visitor);
        objects.push_back(obj);
    });

    return true;
}

template <class C, class E>
void SerializeCollection(const std::string& colname, const std::string& name, tinyxml2::XMLElement* root, C& container, std::function<void (tinyxml2::XMLElement*, E&)> cb)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* collection = doc->NewElement(colname.c_str());
    tinyxml2::XMLElement* item = nullptr;

    for (auto it: container)
        {
            item = doc->NewElement(name.c_str());
            cb(item, it);
            collection->InsertEndChild(item);
        }

    root->InsertEndChild(collection);
}
