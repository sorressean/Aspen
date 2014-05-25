#include <boost/algorithm/string.hpp>
#include <string>
#include "mud.h"
#include "conf.h"
#include "match.h"

bool FullMatch(const std::string& a, const std::string& b, bool icase)
{
    if (icase)
        {
            return a == b? true : false;
        }
    else
        {
            return boost::iequals(a, b) ? true : false;
        }
}
