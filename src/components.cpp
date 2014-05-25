/*
*This is a bit of a messy way to set this up, but it works.
*Include the header file for every component here, then register it.
*/
#include "world.h"
#include "component.h"

void CreateComponents()
{
    World* world = World::GetPtr();

    world->WriteLog("Initializing components.");
}
