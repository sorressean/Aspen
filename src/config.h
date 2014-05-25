/*
*Controls configuration data for the mud.
*/
#pragma once
#include "mud.h"
#include "conf.h"
#include "variant.h"
#include <unordered_map>

class Configuration
{
    std::unordered_map<std::string, Variant>