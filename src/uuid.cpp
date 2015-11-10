#include <tinyxml2.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include "uuid.h"
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

void Uuid::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->ToDocument();
    tinyxml2::XMLElement* node = doc->NewElement("uuid");
    SerializeLong(node, _id);
    root->InsertEndChild(node);
}
void Uuid::Deserialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLElement* element = nullptr;

    element = root->FirstChildElement("uuid");
    if (element)
        {
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
