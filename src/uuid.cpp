#include <random>
#include <chrono>
#include "uuid.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <tinyxml.h>
#include "serializationHelpers.h"

Uuid::Uuid()
{
    _id = 0;
}
Uuid::Uuid(const Uuid& u)
{
    _id = u._id;
}
Uuid::~Uuid()
{
}
void Uuid::Initialize()
{
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    _id = generator();
    _id <<= 32;
    _id |= (0xFFFF&time(NULL));
}
std::string Uuid::ToString() const
{
    std::stringstream st;
    st << std::setw(16) << std::setbase(16);
    st << _id;
    return st.str();
}
unsigned long long int Uuid::GetValue() const
{
    return _id;
}

void Uuid::Serialize(TiXmlElement* root)
{
    TiXmlElement* node = new TiXmlElement("uuid");
    SerializeLong(node, _id);
    root->LinkEndChild(node);
}
void Uuid::Deserialize(TiXmlElement* root)
{
    TiXmlNode* node = NULL;
TiXmlElement* element = NULL;

    node = root->FirstChild("uuid");
    if (node)
        {
            element = node->ToElement();
            _id = DeserializeLong(root);
        }
}

Uuid& Uuid::operator =(Uuid& u)
{
    _id = u._id;
    return *this;
}
bool Uuid::operator ==(Uuid& u)
{
    return (u._id == _id);
}
