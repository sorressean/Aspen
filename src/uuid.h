#pragma once
#include <tinyxml2.h>
#include <string>

class Uuid
{
protected:
    unsigned long long int _id;
public:
    Uuid();
    Uuid(const Uuid &u);
    ~Uuid();
    void Initialize();
    std::string ToString() const;
    unsigned long long int GetValue() const;
    void Serialize(tinyxml2::XMLElement* root);
    void Deserialize(tinyxml2::XMLElement* root);
    Uuid& operator =(Uuid& u);
    bool operator ==(Uuid& u);
};
