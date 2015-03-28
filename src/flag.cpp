#include <unordered_map>
#include <string>
#include <sstream>
#include "flag.h"
#include "mud.h"
#include "conf.h"
#include "utils.h"
#include "bitfield.h"

Flag::Flag(const std::unordered_map<FLAG, std::string>& ffields)
{
    _flagfields = ffields;
    for (auto it: ffields)
        {
            _stringfields[it.second] = it.first;
        }
    _val = 0;
}
Flag::~Flag()
{
}

void Flag::Set(int field)
{
    _val = BitSet(_val, field);
}
bool Flag::IsSet(int field) const
{
    return BitIsSet(_val, field);
}
void Flag::Clear(int field)
{
    _val = BitClear(_val, field);
}
void Flag::Toggle(int field)
{
    if (BitIsSet(_val, field))
        {
            _val = BitClear(_val, field);
        }
    else
        {
            _val = BitSet(_val, field);
        }
}

bool Flag::Set(const std::string& field)
{
    int fnum = 0;
    if (!_stringfields.count(field))
        {
            return false;
        }

    fnum = _stringfields[field];
    _val = BitSet(_val, fnum);
    return true;
}
std::string Flag::Print() const
{
    std::stringstream st;
    st << "[";

    for (auto it: _stringfields)
        {
            if (BitIsSet(_val, it.second))
                {
                    st << "|" << it.first;
                }
        }
    st << "]";
    return st.str();
}

Bitfield Flag::operator [](int pos)
{
    return Bitfield(_val, pos);
}
