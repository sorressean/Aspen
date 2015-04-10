#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "olcGroup.h"
#include "olc.h"

OlcGroup::OlcGroup(const std::string& comp)
{
    _compname = comp;
    _inherit = nullptr;
}
OlcGroup::~OlcGroup()
{
    std::vector<IOlcEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            delete (*it);
            (*it) = nullptr;
        }
}

void OlcGroup::AddEntry(IOlcEntry* entry)
{
    _entries.push_back(entry);
}
IOlcEntry* OlcGroup::GetEntry(const std::string& name)
{
    std::vector<IOlcEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            if (name == (*it)->GetName())
                {
                    return (*it);
                }
        }

    return NULL;
}
void OlcGroup::ListEntries(std::vector<IOlcEntry*>* entries)
{
    /* if (_inherit)                  //We check if this OLCgroup is inheriting another. If so, we copy the entries from the inherited group.
         {
             _inherit->ListEntries(entries);
         }*/

    std::vector<IOlcEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            entries->push_back((*it));

        }
}


void OlcGroup::SetComponentName(const std::string& comp)
{
    _compname = comp;
}
std::string OlcGroup::GetComponentName() const
{
    return _compname;
}

OlcGroup* OlcGroup::GetInheritance()
{
    return _inherit;
}
void OlcGroup::SetInheritance(OlcGroup* inherit)
{
    std::vector<IOlcEntry*> entries;              // This recursively copies entries from the base OLCGroup (cont'd)
    _inherit = inherit;
    _inherit->ListEntries(&_entries);                                       // and from base OLCGroup's parent and so on until it (cont'd)
    // reaches an OLCGroup with no inheritance.
}
