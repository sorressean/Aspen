#include <string>
#include <tinyxml.h>
#include <arpa/inet.h>
#include "mud.h"
#include "conf.h"
#include "serializer.h"
#include "banList.h"

BOOL BanList::AddAddress(const std::string &address)
{
    in_addr addr;

    if (!inet_aton(address.c_str(), &addr))
        {
            return false;
        }
    if (AddressExists(addr.s_addr))
        {
            return false;
        }

    _addresses.push_back(addr.s_addr);
    return true;
}
BOOL BanList::RemoveAddress(const std::string &address)
{
    std::vector<unsigned long>::iterator it, itEnd;
    in_addr addr;

    if (inet_aton(address.c_str(), &addr))
        {
            return false;
        }

    itEnd = _addresses.end();
    for (it = _addresses.begin(); it != itEnd; ++it)
        {
            if ((*it) == addr.s_addr)
                {
                    it = _addresses.erase(it);
                    return true;
                }
        }

    return false;
}
BOOL BanList::AddressExists(const std::string &address)
{
    in_addr addr;

    if (inet_aton(address.c_str(), &addr))
        {
            return false;
        }

    for (auto it: _addresses)
        {
            if (it == addr.s_addr)
                {
                    return true;
                }
        }

    return false;
}
BOOL BanList::AddressExists(unsigned long address)
{
    for (auto it: _addresses)
        {
            if (it == address)
                {
                    return true;
                }
        }

    return false;
}
void BanList::ListAddresses(std::vector<std::string>* addresses)
{
    in_addr addr;
    std::string val;

    for (auto it: _addresses)
        {
            addr.s_addr = it;
            val = inet_ntoa(addr);
            addresses->push_back(val);
        }
}

void BanList::Serialize(TiXmlElement* root)
{
    TiXmlElement *entries = new TiXmlElement("entries");
    in_addr addr;
    std::string printable;

    for (auto it: _addresses)
        {
            TiXmlElement* entry = new TiXmlElement("entry");
            addr.s_addr = it;
            printable = inet_ntoa(addr);
            entry->SetAttribute("address", printable.c_str());
            entries->LinkEndChild(entry);
        }

    root->LinkEndChild(entries);
}
void BanList::Deserialize(TiXmlElement* root)
{
    TiXmlElement* entries = NULL;
    TiXmlNode* node = NULL;

    node = root->FirstChild("entries");
    if (!node)
        {
            return;
        }

    entries = node->ToElement();
    for (node = entries->FirstChild(); node; node = node->NextSibling())
        {
            TiXmlElement* entry = node->ToElement();
            AddAddress(entry->Attribute("address"));
        }
}
