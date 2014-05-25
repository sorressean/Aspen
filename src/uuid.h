#pragma once
#include <string>
#include <tinyxml.h>

class Uuid
{
protected:
    std::string _uuid;
public:
    Uuid();
    Uuid(const Uuid &u);
    ~Uuid();
    void InitializeUuid();
    std::string GetUuid() const;
    Uuid& GetRealUuid();
    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
    Uuid& operator =(Uuid& u);
    Uuid& operator =(const std::string &uuid);
    bool operator ==(Uuid& u);
};
