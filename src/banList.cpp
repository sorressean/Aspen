#include <string>
#include <tinyxml.h>
#include <arpa/inet.h>
#include "mud.h"
#include "conf.h"
#include "serializer.h"
#include "banList.h"

BanList::BanList()
{
}
BanList::~BanList()
{
}
BOOL BanList::AddAddress(const std::string &address)
{
    if (AddressExists(address))
        {
            return false;
        }

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
                    return true;
                }
        }

    return false;
}
BOOL BanList::AddressExists(unsigned long address)
{
    std::vector<unsigned long>::iterator it, itEnd;

    itEnd = _addresses.end();
    for (it = _addresses.begin(); it != itEnd; ++it)
        {
            if ((*it) == address)
                {
                    return true;
                }
        }

    return false;
}
void BanList::ListAddresses(std::vector<std::string>* addresses)
{
    in_addr addr;
    std::vector<unsigned long>::iterator it, itEnd;
    std::string val;

    itEnd = _addresses.end();
    for (it = _addresses.begin(); it != itEnd; ++it)
        {
            addr.s_addr = (*it);
            val=inet_ntoa(addr);
            printf("%s\n", val.c_str());
            addresses->push_back(val);
        }
}

void BanList::Serialize(TiXmlElement* root)
{
    TiXmlElement *entries = new TiXmlElement("entries");
    std::vector<std::string>* addresses = new std::vector<std::string>();
    std::vector<std::string>::iterator it, itEnd;
    TiXmlElement *entry = NULL;

    ListAddresses(addresses);
    itEnd = addresses->end();
    for (it = addresses->begin(); it != itEnd; ++it)
        {
            entry = new TiXmlElement("entry");
            entry->SetAttribute("address", (*it).c_str());
            entries->LinkEndChild(entry);
        }

    root->LinkEndChild(entries);
    delete addresses;
}
void BanList::Deserialize(TiXmlElement* root)
{
    TiXmlElement* entries = NULL;
    TiXmlNode* node = NULL;
    TiXmlElement* entry = NULL;

    node = root->FirstChild("entries");
    if (!node)
        {
            return;
        }

    entries = node->ToElement();
    for (node = entries->FirstChild(); node; node = node->NextSibling())
        {
            entry = node->ToElement();
            AddAddress(entry->Attribute("address"));
        }
}
