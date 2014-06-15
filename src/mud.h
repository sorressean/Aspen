//mud constants and values:
#pragma once
#ifndef MUD_H
#define MUD_H
#include "conf.h"
//used to show where an exception was thrown:
#define WHERE() std::string(" In ")+std::string(__FUNCTION__)+std::string(" at ")+std::string(__FILE__)+std::string(".")
//typedefs:
typedef int FLAG; //bitfield flags
typedef int VNUM;
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef unsigned long long int OBJID;
struct range
{
    int min;
    int max;
};
//forward declarations
//I wanted to avoid these, but it seems I have no choice
class World;
class Uuid;
class Entity;
class Socket;
class Player;
class Component;
class Variant;
class BaseObject;
class StaticObject;
class Zone;
class Room;
class ObjectManager;

struct OLC_DATA;

//olc forward declarations
struct OLC_DATA;
//enum OLC_INPUT_TYPE {NUM, STRING, BOOLEAN, NOINPUT, EDITOR};
//typedef void (*FP_INPUT)(Entity*, Player*, const Variant*);
#endif
