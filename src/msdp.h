#pragma once
#include <string>
#include "mud.h"
#include "conf.h"
struct MSDP_VARIABLE
{
    int var; /*The type of variable*/
    std::string name; /*The name of the variable*/
    bool isString; /*Is this variable a string?*/
    bool isConfigurable; /*Can the client change the value of this variable?*/
    bool isWriteOnce; /*Can only write once?*/
    int minimum; /*The minimum value for the variable.*/
    int maximum; /*The maximum value for this variable.*/
    int numDefault; /*the default integer value.*/
    std::string strDefault; /*Default value for strings.*/
};
