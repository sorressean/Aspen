#ifndef HELP_ENTRY_H
#define HELP_ENTRY_H
#include "../mud.h"
#include "../conf.h"
#include <string>
#include <vector>
#include <tinyxml.h>

#ifdef MODULE_HELP
enum class HelpType
{
    General, Builder, Administration
};

//the basic help node.
class HelpEntry
{
    time_t _lastModified;
    std::string _name;
    std::string _data;
    FLAG _access;
    HelpType _type;
    UINT _id;
    std::vector<HelpEntry*> _seeAlso;
public:
    HelpEntry(const std::string &name, const std::string &data, FLAG access, HelpType type);
    HelpEntry();
    ~HelpEntry();
    /*
    *return:  the name of the entry.
    */
    std::string GetName() const;
    /*
    *Sets the name on the help entry.
    *Param: [in] the name of the entry.
    */
    void SetName(const std::string &name);
    /*
    *Return: the text associated with the entry.
    */
    std::string GetData() const;
    /*
    *Sets the data associated with the entry.
    *Param: [in] the data for the entry.
    */
    void SetData(const std::string &data);
    /*
    *Return: The type of the help entry.
    */
    HelpType GetType() const;
    /*
    *Sets the type of the help entry.
    *Param: [in] the type for the entry.
    */
    void SetType(HelpType type);
    /*
    *Returns the ID of the help file.
    */
    UINT GetId() const;
    /*
    *Sets the ID associated with this help file.
    *Param: [in] the id.
    */
    void SetId(UINT id);
    /*
    *Gets the minimum access required to view the entry.
    *Return: the minimum access.
    */
    FLAG GetAccess() const;
    /*
    *Sets the access required to view the help entry.
    *Param: [in] the minimum access.
    */
    void SetAccess(FLAG access);
    /*
    *Takes a list of the SeeAlso entries and converts them to an english list.
    *Return: The std::string with the SeeAlso list expanded to an actual English list.
    */
    std::string SeealsoToList();
    /*
    *Checks to see if the specified help entry is in the SeeAlso list.
    *Param: [in] a pointer to the entry.
    *Return: True if the entry was found, false otherwise.
    */
    BOOL SeeAlsoExists(HelpEntry* entry);
    /*
    *Checks to see if the specified help entry is in the SeeAlso list.
    *Param: [in] The name of the entry.
    *Return: True if the entry was found, false otherwise.
    */
    BOOL SeeAlsoExists(const std::string &name);
    /*
    *Adds a SeeAlso entry.
    *Param: [in] a pointer to the entry.
    *Return: True if the add was successful, false otherwise.
    */
    BOOL AddSeeAlso(HelpEntry* entry);
    /*
    *Removes the SeeAlso entry from the list.
    *Param: [in] the name of the entry to remove.
    *Return: True if the entry could be removed, false otherwise.
    */
    BOOL RemoveSeeAlso(const std::string &name);
    /*
    *Populates a vector with all the names of the SeeAlso entries.
    *Param: [in] a pointer to a vector of strings, which will contain the names of the see also entries.
    *Return: True on success, false on failure.
    */
    BOOL SeeAlsoToList(std::vector<std::string>* entries);
    /*
    *Returns a seeAlso entry based on the given index, NULL if none was found.
    *Param: [in] the index.
    *Return: A pointer to the help entry this index references, NULL if none was found.
    */
    HelpEntry* GetSeeAlsoByIndex(unsigned int index);
    /*
    *Updates the time since the help entry was last modified, and sets the required rank.
    *Param: [in] the rank to set.
    */
    void UpdateAccess(FLAG access);
    /*
    *Serializes the entry into the xml element.
    *Param: [in] the root xml element to append entries to.
    */
    void Serialize(TiXmlElement* root);
    /*
    *Retrieves help entry data from the specified XML entry.
    *Param: [in] the element where the data is to be retrieved from.
    */
    void Deserialize(TiXmlElement* entry);
};

#endif
#endif
