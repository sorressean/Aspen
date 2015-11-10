#include <tinyxml2.h>
#include <string>
#include <vector>
#include "../mud.h"
#include "../conf.h"
#include "../utils.h"
#include "HelpEntry.h"
#ifdef MODULE_HELP
HelpEntry::HelpEntry(const std::string &name, const std::string &data, FLAG access, HelpType type):_name(name), _data(data), _access(access), _type(type)
{
    _lastModified = time(NULL);
    _id = 0;
}
HelpEntry::HelpEntry()
{
    _lastModified = time(NULL);
    _id = 0;
}
HelpEntry::~HelpEntry()
{
}

std::string HelpEntry::GetName() const
{
    return _name;
}
void HelpEntry::SetName(const std::string &name)
{
    _name = name;
}

std::string HelpEntry::GetData() const
{
    return _data;
}
void HelpEntry::SetData(const std::string &data)
{
    _data = data;
}

HelpType HelpEntry::GetType() const
{
    return _type;
}
void HelpEntry::SetType(HelpType type)
{
    _type = type;
}

UINT HelpEntry::GetId() const
{
    return _id;
}
void HelpEntry::SetId(UINT id)
{
    _id = id;
}

FLAG HelpEntry::GetAccess() const
{
    return _access;
}
void HelpEntry::SetAccess(FLAG access)
{
    _access = access;
}

std::string HelpEntry::SeealsoToList()
{
    if (_seeAlso.size() == 0)
        {
            return "none";
        }

    std::vector<std::string> names;

    for (auto it: _seeAlso)
        {
            names.push_back(it->GetName());
        }
    return EnglishList(&names);
}
BOOL HelpEntry::SeeAlsoExists(HelpEntry* entry)
{
    for (auto it: _seeAlso)
        {
            if (it == entry)
                {
                    return true;
                }
        }

    return false;
}
BOOL HelpEntry::SeeAlsoExists(const std::string &name)
{
    for (auto it: _seeAlso)
        {
            if (it->GetName() == name)
                {
                    return true;
                }
        }

    return false;
}
BOOL HelpEntry::AddSeeAlso(HelpEntry* entry)
{
    if (SeeAlsoExists(entry))
        {
            return false;
        }

    _seeAlso.push_back(entry);
    return true;
}
BOOL HelpEntry::RemoveSeeAlso(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _seeAlso.end();
    for (it = _seeAlso.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    _seeAlso.erase(it);
                    return true;
                }
        }
    return false;
}
BOOL HelpEntry::SeeAlsoToList(std::vector<std::string>* entries)
{
    if (!entries)
        {
            return false;
        }

    for (auto it: _seeAlso)
        {
            entries->push_back(it->GetName());
        }
    return true;
}
HelpEntry* HelpEntry::GetSeeAlsoByIndex(unsigned int index)
{
    if (index == 0 || index > _seeAlso.size()-1)
        {
            return NULL;
        }

    return _seeAlso.at(index);
}

void HelpEntry::UpdateAccess(FLAG access)
{
    SetAccess(access);
}

void HelpEntry::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->ToDocument();
    tinyxml2::XMLElement* entry = doc->NewElement("entry");
    entry->SetAttribute("name", _name.c_str());
    entry->SetAttribute("data", _data.c_str());
    entry->SetAttribute("type", (int)_type);
    entry->SetAttribute("access", _access);
    entry->SetAttribute("lastModified", (unsigned int)_lastModified);
    entry->SetAttribute("id", _id);
    root->InsertEndChild(entry);
}
void HelpEntry::Deserialize(tinyxml2::XMLElement* entry)
{
    _name = entry->Attribute("name");
    _data = entry->Attribute("data");
    _access = entry->IntAttribute("access");
    _type = (HelpType)entry->IntAttribute("type");
    _lastModified = (time_t)entry->IntAttribute("lastModified");
    _id = entry->IntAttribute("id");
}
#endif
