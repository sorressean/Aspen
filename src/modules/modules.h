#pragma once
#include "../mud.h"
#include "../conf.h"

/**
All of these defines control whether or not any of the modules are compiled in.
They are all commented out by default; you need only to uncomment them and recompile to have the module installed.
*/
//#define MODULE_BOARD

bool InitializeExternalModules();