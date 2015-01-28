#pragma once
#include <tinyxml.h>

template <class O, class C, class I>
BOOL SerializeList(const std::string& parent, const std::string& nodename, TiXmlElement* root, C& container)
{
    I it, itEnd;
    TiXmlElement* pnode = new TiXmlElement(parent.c_str());

    itEnd = container.end();
    for (it = container.begin(); it != itEnd; ++it)
        {
            (*it)->Serialize(pnode);
        }
    root->LinkEndChild(pnode);
    return true;
}

template <class O, class C>
BOOL DeserializeList(TiXmlElement* root, const std::string &parent, C& objects)
{
    TiXmlNode* node = NULL;
    TiXmlElement* element = NULL;
    TiXmlElement* pnode = NULL;
    O* obj = NULL;

    node = root->FirstChild(parent.c_str());
    if (node)
        {
            pnode = node->ToElement();
            for (node = pnode->FirstChild(); node; node=node->NextSibling())
                {
                    element = node->ToElement();
                    obj = new O();
                    obj->Deserialize(element);
                    objects.push_back(obj);
                }
        }
    return true;
}
