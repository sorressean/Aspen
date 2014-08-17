/**
Scripting for the BaseObject class.
*/
#pragma once
#include <angelscript.h>
#include "../mud.h"
#include "../conf.h"

void RegisterBaseObjectMethods(const char* obj);
bool InitializeBaseObject();
