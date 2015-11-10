#include <tinyxml2.h>
#include <vector>
#include <string>
#include "../mud.h"
#include "../conf.h"
#include "../world.h"
#include "../player.h"
#include "HelpTable.h"
#include "HelpEntry.h"

#ifdef MODULE_HELP
HelpTable::HelpTable()
{
}
HelpTable::~HelpTable()
{
    for (auto it: _entries)
        {
            delete it;
        }
}

void HelpTable::Load()
{
    World* world = World::GetPtr();
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = nullptr;
    tinyxml2::XMLElement* element = nullptr;
    HelpEntry* entry = nullptr;

    if (doc.LoadFile(HELP_FILE) != tinyxml2::XML_NO_ERROR)
        {
            world->WriteLog("Could not find help files.", ERR);
            return;
        }

    root = doc.FirstChildElement("help");
    if (!root)
        {
            world->WriteLog("The help files were loaded, but the top element could not be located. Help will not load.", ERR);
            return;
        }

    for (element = root->FirstChildElement("entry"); element; element = element->NextSiblingElement())
        {
            entry = new HelpEntry();
            entry->Deserialize(element);
            _entries.push_back(entry);
        }
}
void HelpTable::Save()
{
    tinyxml2::XMLDocument doc;
    doc.LinkEndChild(doc.NewDeclaration());
    tinyxml2::XMLElement* root = doc.NewElement("help");

    for (auto it: _entries)
        {
            it->Serialize(root);
        }

    doc.SaveFile(HELP_FILE);
}

BOOL HelpTable::AddEntry(HelpEntry* entry)
{
    if (!entry)
        {
            return false;
        }

    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == entry->GetName())
                {
                    return false;
                }
        }

    _entries.push_back(entry);
    return true;
}
BOOL HelpTable::RemoveEntry(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd, it2, it2End;

    itEnd = it2End = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    for (it2 = _entries.begin(); it2 != it2End; ++it2)
                        {
                            if ((*it2) == (*it))
                                {
                                    continue;
                                }
                            if ((*it2)->SeeAlsoExists((*it)->GetName()))
                                {
                                    (*it2)->RemoveSeeAlso((*it)->GetName());
                                }
                        }
                    delete (*it);
                    _entries.erase(it);
                    return true;
                }
        }

    return false;
}
BOOL HelpTable::EntryExists(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    return true;
                }
        }

    return false;
}
HelpEntry* HelpTable::GetEntry(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    if (!EntryExists(name))
        {
            return NULL;
        }

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    return (*it);
                }
        }
    return NULL;
}

BOOL HelpTable::ShowEntry(const std::string &name, Player* mobile)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    mobile->Message(MSG_INFO, (*it)->GetData());
                    return true;
                }
        }

    return false;
}
#endif
