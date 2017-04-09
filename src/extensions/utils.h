/**
* extended utils
* overwrite these functions to respond to further requests from the mud.
*/
#ifndef EXT_UTILS_H
#define EXT_UTILS_H
#include <string>

#include "../mud.h"

/**
* callback to retrieve further position information.
* If you add more positions beyond POSITION_STANDING,
* make sure you add their string names in this function.
* See utils.cpp GetPositionString for examples.
*
* \param [in] the position to convert.
* \return the string representing the position.
*/
std::string GetExtendedPositionString(unsigned int position);
#endif
