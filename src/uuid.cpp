#include "uuid.h"
#include "utils.h"
#include <string>
#include <tinyxml.h>

Uuid::Uuid()
{
}
Uuid::Uuid(const Uuid& u)
{
    _uuid = u._uuid;
}
Uuid::~Uuid()
{
}
void Uuid::InitializeUuid()
{
    _uuid = GenerateUuid();
}
std::string Uuid::GetUuid() const
{
    return _uuid;
}
Uuid& Uuid::GetRealUuid()
{
    return *this;
}
void Uuid::Serialize(TiXmlElement* root)
{
    root->SetAttribute("uuid", _uuid.c_str());
}
void Uuid::Deserialize(TiXmlElement* root)
{
    _uuid = root->Attribute("uuid");
}

Uuid& Uuid::operator =(Uuid& u)
{
    _uuid = u._uuid;
    return *this;
}
Uuid& Uuid::operator =(const std::string &uuid)
{
    _uuid = uuid;
    return *this;
}
bool Uuid::operator ==(Uuid& u)
{
    return (u._uuid == _uuid);
}
