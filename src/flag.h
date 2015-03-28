#pragma once
#include <unordered_map>
#include <string>
#include "mud.h"
#include "conf.h"
#include "bitfield.h"

class Flag
{
    FLAG _val;
    std::unordered_map<FLAG, std::string> _flagfields;
    std::unordered_map<std::string, FLAG> _stringfields;
public:
    Flag(const std::unordered_map<FLAG, std::string>& ffields);
    ~Flag();
    void Set(int field);
    bool IsSet(int field) const;
    void Clear(int field);
    void Toggle(int field);
    bool Set(const std::string& field);
    std::string Print() const;
    Bitfield operator[](int pos);
};
