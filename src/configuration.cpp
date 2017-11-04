#include <string>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "mud.h"
#include "configuration.h"

Configuration* Configuration::_ptr;
Configuration* Configuration::GetPtr()
{
    return Configuration::_ptr;
}

void Configuration::Initialize()
{
    if (!Configuration::_ptr)
        {
            Configuration::_ptr = new Configuration();
        }
}

void Configuration::Release()
{
    if (Configuration::_ptr)
        {
            delete Configuration::_ptr;
            Configuration::_ptr = nullptr;
        }
}

void Configuration::Load()
{
    if (boost::filesystem::exists(SETTINGS_FILE))
        {
            pt::read_xml(SETTINGS_FILE, tree);
        }
}

void Configuration::Save()
{
    pt::write_xml(SETTINGS_FILE, tree);
}
