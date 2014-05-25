#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "olcGroup.h"
#include "olc.h"

OlcGroup::OlcGroup(const std::string& comp)
{
    _compname = comp;
}
OlcGroup::~OlcGroup()
{
    std::vector<IOlcEntry*>::iterator it, itEnd;

    itEnd = _entries.end();
    for (it = _entries.begin(); it != itEnd; ++it)
        {
            delete (*it);
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
