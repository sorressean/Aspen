/*
*This class defines the basic interface for an object that can be serialized.
*/
#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <tinyxml2.h>
#include "mud.h"
#include "conf.h"

class ISerializable
{
public:
    virtual ~ISerializable() { }
    virtual void Serialize(tinyxml2::XMLElement* root) = 0;
    virtual void Deserialize(tinyxml2::XMLElement* root) = 0;
};
#endif
