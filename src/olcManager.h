/*
*This class is responsible for managing olc groups.
*/
#pragma once
#include <unordered_map>
#include "mud.h"
#include "conf.h"
#include "olc.h"
#include "olcGroup.h"

class OlcManager
{
    std::unordered_map<int, OlcGroup*> _olcs;
public:
    OlcManager();
    ~OlcManager();
    bool HasGroup(OLCGROUP group);
    bool AddGroup(OLCGROUP g, OlcGroup* group);
    OlcGroup* GetGroup(OLCGROUP group);
};
