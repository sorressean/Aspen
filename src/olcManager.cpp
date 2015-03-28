#include <unordered_map>
#include "mud.h"
#include "conf.h"
#include "olc.h"
#include "olcGroup.h"
#include "olcManager.h"

OlcManager::OlcManager()
{
}
OlcManager::~OlcManager()
{
    std::unordered_map<int, OlcGroup*>::iterator it, itEnd;

    itEnd = _olcs.end();

    for (it = _olcs.begin(); it != itEnd; ++it)
        {
            delete it->second;
        }
}

BOOL OlcManager::HasGroup(OLCGROUP group)
{
    return (_olcs.count((int)group) ? true : false);
}
BOOL OlcManager::AddGroup(OLCGROUP g, OlcGroup* group)
{
    if (HasGroup(g))
        {
            return false;
        }

    _olcs[(int)g] = group;
    return true;
}
OlcGroup* OlcManager::GetGroup(OLCGROUP group)
{
    if (!HasGroup(group))
        {
            return NULL;
        }

    return _olcs[(int)group];
}
