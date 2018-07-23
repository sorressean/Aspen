#include <tinyxml2.h>
#include <string>
#include <arpa/inet.h>
#include "mud.h"
#include "conf.h"
#include "banList.h"

bool BanList::AddAddress(const std::string &address)
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
bool BanList::RemoveAddress(const std::string &address)
{
    in_addr addr;

    if (inet_aton(address.c_str(), &addr))
        {
            return false;
        }
    auto end = _addresses.end();
    for (auto it=_addresses.begin(); it != end; ++it)
        {
            if ((*it) == addr.s_addr)
                {
                    _addresses.erase(it);
                    return true;
                }
        }

    return false;
}
bool BanList::AddressExists(const std::string &address) const
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
bool BanList::AddressExists(unsigned long address) const
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
void BanList::ListAddresses(std::vector<std::string>& addresses)
{
    in_addr addr;
    std::string val;

    for (auto it: _addresses)
        {
            addr.s_addr = it;
            val = inet_ntoa(addr);
            addresses.push_back(val);
        }
}

void BanList::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* entries = doc->NewElement("entries");
    in_addr addr;
    std::string printable;

    for (auto it: _addresses)
        {
            tinyxml2::XMLElement* entry = doc->NewElement("entry");
            addr.s_addr = it;
            printable = inet_ntoa(addr);
            entry->SetAttribute("address", printable.c_str());
            entries->InsertEndChild(entry);
        }

    root->InsertEndChild(entries);
}
void BanList::Deserialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLElement* entries = nullptr;
    tinyxml2::XMLElement* entry = nullptr;

    entries = root->FirstChildElement("entries");
    if (!entries)
        {
            return;
        }

    for (entry = entries->FirstChildElement(); entry; entry = entries->NextSiblingElement())
        {
            AddAddress(entry->Attribute("address"));
        }
}
