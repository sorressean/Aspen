#include "../mud.h"
#include "../conf.h"
#include "../utils.h"
#include "HelpEntry.h"
#include <string>
#include <vector>

#ifdef MODULE_HELP
HelpEntry::HelpEntry(const std::string &name, const std::string &data, FLAG access, HELP_ENTRY_TYPE type):_name(name), _data(data), _access(access), _type(type)
{
    _lastModified = time(NULL);
    _id = 0;
    _seeAlso = new std::vector<HelpEntry*>();
}
HelpEntry::HelpEntry()
{
    _lastModified = time(NULL);
    _id = 0;
    _seeAlso = new std::vector<HelpEntry*>();
}
HelpEntry::~HelpEntry()
{
    delete _seeAlso;
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

HELP_ENTRY_TYPE HelpEntry::GetType() const
{
    return _type;
}
void HelpEntry::SetType(HELP_ENTRY_TYPE type)
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
    if (_seeAlso->size() == 0)
        {
            return "none";
        }

    std::vector<std::string> names;
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _seeAlso->end();
    for (it = _seeAlso->begin(); it != itEnd; ++it)
        {
            names.push_back((*it)->GetName());
        }
    return EnglishList(&names);
}
BOOL HelpEntry::SeeAlsoExists(HelpEntry* entry)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _seeAlso->end();
    for (it = _seeAlso->begin(); it != itEnd; ++it)
        {
            if ((*it) == entry)
                {
                    return true;
                }
        }

    return false;
}
BOOL HelpEntry::SeeAlsoExists(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _seeAlso->end();
    for (it = _seeAlso->begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
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

    _seeAlso->push_back(entry);
    return true;
}
BOOL HelpEntry::RemoveSeeAlso(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _seeAlso->end();
    for (it = _seeAlso->begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    _seeAlso->erase(it);
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

    std::vector<HelpEntry*>::iterator it, itEnd;
    itEnd = _seeAlso->end();
    for (it = _seeAlso->begin(); it != itEnd; ++it)
        {
            entries->push_back((*it)->GetName());
        }
    return true;
}
HelpEntry* HelpEntry::GetSeeAlsoByIndex(unsigned int index)
{
    if (index == 0 || index > _seeAlso->size()-1)
        {
            return NULL;
        }

    return _seeAlso->at(index);
}

void HelpEntry::UpdateAccess(FLAG access)
{
    _lastModified = time(NULL);
    SetAccess(access);
}

void HelpEntry::Serialize(TiXmlElement* root)
{
    TiXmlElement* entry = new TiXmlElement("entry");
    entry->SetAttribute("name", _name.c_str());
    entry->SetAttribute("data", _data.c_str());
    entry->SetAttribute("type", _type);
    entry->SetAttribute("access", _access);
    entry->SetAttribute("lastModified", _lastModified);
    entry->SetAttribute("id", _id);
    root->LinkEndChild(entry);
}
void HelpEntry::Deserialize(TiXmlElement* entry)
{
    int tmp = 0;

    _name = entry->Attribute("name");
    _data = entry->Attribute("data");
    entry->Attribute("access", &_access);
    tmp = (int)_type;
    entry->Attribute("type", &tmp);
    tmp = (int)_lastModified;
    entry->Attribute("lastModified", &tmp);
    entry->Attribute("id", (int*)&_id);
}
#endif
