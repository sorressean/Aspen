#ifndef HELP_TABLE_H
#define HELP_TABLE_H
#include <tinyxml2.h>
#include <vector>
#include <string>
#include "../mud.h"
#include "../conf.h"
#include "../player.h"
#include "HelpEntry.h"
#ifdef MODULE_HELP
class HelpTable
{
    std::vector<HelpEntry*> _entries;
public:
    HelpTable();
    ~HelpTable();
    /*
    *Loads all help entries stored in the default help file.
    */
    void Load();
    /*
    *Iterates through all of the help entries stored and serializes them to one file.
    */
    void Save();
    /*
    *Adds the specified entry.
    *Param: [in] the entry to add.
    *Return: True if the entry could be added, false otherwise.
    */
    BOOL AddEntry(HelpEntry* entry);
    /*
    *Locates and removes the entry by name.
    *Param: [in] the entry to find.
    *Return: True if the entry was found and could be removed, false otherwise.
    */
    BOOL RemoveEntry(const std::string &name);
    /*
      *Checks for the existance of an entry by name.
    *Return: True if the entry exists, false otherwise.
    */
    BOOL EntryExists(const std::string &name);
    /*
    *Tries to retrieve a pointer to the specified help entry.
    *Param: [in] the name of the entry to find.
    *Return: A pointer to the help entry if one was found, NULL otherwise.
    */
    HelpEntry* GetEntry(const std::string &name);
    /*
    *Tries to show the specified entry.
    *Param: [in] the name of the entry to show.
    *Param: [in] a pointer to the player to show the entry to.
    *Return: false if the entry could not be shown because it does not exist or the user does not have the proper permissions to read, true otherwise.
    */
    BOOL ShowEntry(const std::string &name, Player* mobile);
};

#endif
#endif
