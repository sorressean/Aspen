/**
* Contains the configuration module.
*/
#ifndef CONFIGURATION_H
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "mud.h"

namespace pt = boost::property_tree;

/**
* The configuration class holds a configuration tree.
*
* The configuration tree is used to allow a mud admin to modify and/or change
* configuration data on the fly.
* This includes anything from password and name length, to listening port.
*/
class Configuration
{
    static Configuration* _ptr;
    pt::ptree tree; //property tree

    Configuration() = default;
    ~Configuration() = default;
public:
    /**
    * Retrieves the pointer for the singleton object.
    */
    static Configuration* GetPtr();
    /**
    * Creates the configuration object for the singleton.
    */
    static void Initialize();
    /**
    * Releases the configuration object.
    */
    static void Release();
    /**
    * Loads the configuration data.
    */
    void Load();
    /**
    * Saves configuration data.
    */
    void Save();
};
#endif
