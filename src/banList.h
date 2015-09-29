/**
* Contains: BanList
*
* The ban list is responsible for managing a list of addresses currently banned from the game.
*/
#ifndef BAN_LIST_H
#define BAN_LIST_H
#include <vector>
#include "mud.h"
#include "conf.h"
#include "serializer.h"

/**
* Holds a collection of all addresses banned from the server.
*
* The BanList keeps track of all addresses that are currently banned from the server.
* When a client connects, this list is referenced to make sure that the address does not exist.
* @todo Add a way to ban large networks by CIDR.
*/
class BanList:public ISerializable
{
    std::vector<unsigned long> _addresses;
public:
    BanList() = default;
    ~BanList() = default;
    /**
    * Adds the specified address to the collection.
    *
    * @param [in] address adds the specified IPV4 address to the collection.
    * The address will be converted from it's string representation.
    * @return true if the address could be added, false otherwise.
    */
    BOOL AddAddress(const std::string &address);
    /**
    * Removes the specified address from the collection.
    *
    * @param [in] address the address to remove.
    * @return True if the address could be removed, false otherwise.
    */
    BOOL RemoveAddress(const std::string &address);
    /**
    * Determine if the specified address is in the ban list.
    *
    * @param [in] address The address to check.
    * @return True if the address is in the list, false otherwise.
    */
    BOOL AddressExists(const std::string &address);
    BOOL AddressExists(unsigned long address);
    /**
    * Gets a list of addresses that have been banned.
    *
    * @param [out] A vector that can take a list of all addresses, in readable form.
    */
    void ListAddresses(std::vector<std::string> *addresses);

    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
};
#endif
