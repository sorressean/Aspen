/*
*An olcGroup is a list of olcs that belong to a specific object.
*/
#pragma once
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"

class IOlcEntry;
class OlcGroup
{
    std::vector<IOlcEntry*> _entries;
    std::string _compname;
public:
    OlcGroup(const std::string&comp="");
    ~OlcGroup();
    void AddEntry(IOlcEntry* entry);
    IOlcEntry* GetEntry(const std::string& name);
    void ListEntries(std::vector<IOlcEntry*>* entries);
    void SetComponentName(const std::string& comp);
    std::string GetComponentName() const;
};
