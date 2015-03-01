#pragma once
#include <tinyxml.h>
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
    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
    Uuid& operator =(Uuid& u);
    bool operator ==(Uuid& u);
};
