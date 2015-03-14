#pragma once
#include "mud.h"
#include "conf.h"
#include "serializationHelpers.hpp"

bool SerializeLong(TiXmlElement* parent, unsigned long long int val);
unsigned long long int DeserializeLong(TiXmlElement* parent);
