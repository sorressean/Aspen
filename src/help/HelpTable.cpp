#include "../mud.h"
#include "../conf.h"
#include "../world.h"
#include "../player.h"
#include "HelpTable.h"
#include "HelpEntry.h"
#include <vector>
#include <string>
#include <tinyxml.h>

#ifdef MODULE_HELP
HelpTable::HelpTable()
{
    _entries = new std::vector<HelpEntry*>();
}
HelpTable::~HelpTable()
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
        {
            delete (*it);
        }

    delete _entries;
}

void HelpTable::Load()
{
    World* world = World::GetPtr();
    TiXmlDocument doc(HELP_FILE);
    TiXmlElement* root = NULL;
    TiXmlElement* element = NULL;
    TiXmlNode* node = NULL;
    HelpEntry* entry = NULL;
    if (!doc.LoadFile())
        {
            world->WriteLog("Could not find help files.", ERR);
            return;
        }

    node = doc.FirstChild("help");
    if (!node)
        {
            world->WriteLog("The help files were loaded, but the top element could not be located. Help will not load.", ERR);
            return;
        }
    root = node->ToElement();

    node=root->FirstChild("entry");
    if (!node)
        {
            return;
        }
    element = node->ToElement();
    for (node = element->FirstChild(); node; node = node->NextSibling())
        {
            entry = new HelpEntry();
            entry->Deserialize(element);
            _entries->push_back(entry);
        }
}

void HelpTable::Save()
{
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement* root = new TiXmlElement("help");
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
        {
            (*it)->Serialize(root);
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

    itEnd = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == entry->GetName())
                {
                    return false;
                }
        }

    _entries->push_back(entry);
    return true;
}
BOOL HelpTable::RemoveEntry(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd, it2, it2End;;

    itEnd = it2End = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
        {
            if ((*it)->GetName() == name)
                {
                    for (it2 = _entries->begin(); it2 != it2End; ++it2)
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
                    _entries->erase(it);
                    delete (*it);
                    return true;
                }
        }

    return false;
}
BOOL HelpTable::EntryExists(const std::string &name)
{
    std::vector<HelpEntry*>::iterator it, itEnd;

    itEnd = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
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

    itEnd = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
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

    itEnd = _entries->end();
    for (it = _entries->begin(); it != itEnd; ++it)
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
