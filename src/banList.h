#ifndef BAN_LIST_H
#define BAN_LIST_H
#include "mud.h"
#include "conf.h"
#include "serializer.h"
#include <vector>

/**
*Holds a collection of all addresses banned from the server.
*
*The BanList keeps track of all addresses that are currently banned from the server.
*When a client connects, this list is referenced to make sure that the address does not exist.
*/
class BanList:public ISerializable
{
    std::vector<unsigned long> _addresses;
public:
///Initializes the ban list.
    BanList();
///cleans up the collection of addresses.
    ~BanList();
    /**
    *Adds the specified address to the collection.
    *
    *\param adds the specified "x.x.x.x" address to the collection.
    *\return true if the address could be added, false otherwise.
    *The address will be converted from it's string representation.
    *This allows for faster searching of the list.
    */
    BOOL AddAddress(const std::string &address);
    /**
    *Removes the specified address from the collection.
    *
    *\param the address to remove.
    *\return True if the address could be removed, false otherwise.
    */
    BOOL RemoveAddress(const std::string &address);
    BOOL AddressExists(const std::string &address);
    BOOL AddressExists(unsigned long address);
    void ListAddresses(std::vector<std::string> *addresses);
    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
};
#endif
