/*
*This class defines the basic interface for an object that can be serialized.
*/
#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "mud.h"
#include "conf.h"
#include <tinyxml.h>

class ISerializable
{
public:
    virtual ~ISerializable() { }
    virtual void Serialize(TiXmlElement* root) = 0;
    virtual void Deserialize(TiXmlElement* root) = 0;
};
#endif
