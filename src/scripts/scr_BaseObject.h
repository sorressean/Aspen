/**
Scripting for the BaseObject class.
*/
#pragma once
#include <angelscript.h>
#include "../mud.h"
#include "../conf.h"

void RegisterBaseObjectMethods(asIScriptEngine* engine, const char* obj);
bool InitializeBaseObject(asIScriptEngine* engine);
