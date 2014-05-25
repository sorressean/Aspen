#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "alias.h"

Alias::Alias()
{
}
Alias::Alias(const std::string& name)
{
_name = name;
}
Alias::~Alias()
{
}

std::string Alias::GetName() const
{
return _name;
}
void Alias::SetName(const std::string& name)
{
