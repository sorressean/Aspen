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
    for (auto it: _entries)
        {
            delete it;
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
//We check if this OLCgroup is inheriting another. If so, we copy the entries from the inherited group.
    if (_inherit)
        {
            _inherit->ListEntries(entries);
        }

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
    _inherit = inherit;
}
