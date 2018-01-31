/**
* Contains: mud constants and values.
*/
#pragma once
#ifndef MUD_H
#define MUD_H
#include "conf.h"
/**
* @todo move to utils.h
*/
//used to show where an exception was thrown:
#define WHERE() std::string(" In ")+std::string(__FUNCTION__)+std::string(" at ")+std::string(__FILE__)+std::string(".")

//typedefs:
typedef int FLAG; //bitfield flags
typedef int VNUM;
typedef int BOOL;
/**
* @todo clean some of these up.
*/
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef unsigned long long int OBJID;

struct range
{
    int min;
    int max;
};

/**
* @todo remove these or as many as possible, put forward declarations in the files that require them.
*/
//forward declarations
class Entity;
class Socket;
class Player;
class Component;
class BaseObject;
class StaticObject;
class Zone;

//olc forward declarations
struct OLC_DATA;

//player rankings:
#define RANK_PLAYER 1<<1
#define RANK_PLAYTESTER 1<<2
#define RANK_NEWBIEHELPER 1<<3
#define RANK_NEWBIE 1<<4
#define RANK_BUILDER 1<<5
#define RANK_ADMIN 1<<29
#define RANK_GOD 1<<30

//player flags
#define PF_SILENCE 1<<1 //player can't broadcast
#define FKILLER 1<<2 //player is PK
#define PF_FROZEN 1<<3 //player can't do anything.
#define PF_NOTITLE 1<<4 //player can't set a title
//room flags
#define RF_NOSAY 1<<1
#define RF_DARK 1<<2
#define RF_DEATH 1<<3
#define RF_NOMOB 1<<4
#define RF_INDOORS 1<<5
#define RF_PEACE 1<<6
#define RF_PRIVATE 1<<7
#define RF_HOUSE 1<<8
//NPC flags
#define MF_STATIC 1<<1 //mob does not move
#define MF_SCAVENGER 1<<2 //mob picks up eq on the ground.
#define M_AGGRESSIVE 1<<3 //mob is aggro
#define MF_ZONELOCK 1<<4 //mob doesn't leave zone
#define MF_WIMPY 1<<5 //mob flees when it gets hurt.
#define MF_MEMORY 1<<6 //remember attackers.
#define MF_ASSIST 1<<7 //assist other mobs fighting.
#define MF_NOKILL 1<<8 //mob can't be killed.

//logger levels
enum LOG_LEVEL {INFORM,WARN,ERR,CRIT,SCRIPT, PLAYER, CONNECTION};
//Message types:
enum MessageType {MSG_NORMAL, MSG_ERROR,MSG_INFO,MSG_CRITICAL,MSG_CHANNEL,MSG_LIST};

//terrain types:
enum class TERRAIN
{
    OCEAN, UNDERWATER, AIR, PLANE, DESERT, UNDERGROUND, SWAMP, HILLS, MOUNTAIN, FOREST, LIGHTFOREST, CITY, INSIDE, MAX
};
#define POSITION_ANY 0
#define POSITION_UNCONCIOUS 1<<1
#define POSITION_SLEEPING 1<<2
#define POSITION_SITTING 1<<3
#define POSITION_STANDING 1<<4

//exit/room constants:
#define EXIT_NOWHERE -1
#define ROOM_NOWHERE -1
#define ROOM_START 1

//Direction defines for subcmd.
#define DIR_NORTH 1
#define DIR_SOUTH 2
#define DIR_EAST 3
#define DIR_WEST 4
#define DIR_NORTHEAST 5
#define DIR_NORTHWEST 6
#define DIR_SOUTHEAST 7
#define DIR_SOUTHWEST 8
#define DIR_UP 9
#define DIR_DOWN 10

enum ExitDirection
{
    nowhere, north, south, east, west, northeast, northwest, southeast, southwest, up, down
};
//colors:
#define C_NORMAL "\x1B[0m"
#define C_RED "\x1B[31m"
#define C_GREEN "\x1B[32m"
#define C_YELLOW "\x1B[33m"
#define C_BLUE "\x1B[34m"
#define C_MAGENTA "\x1B[35m"
#define C_CYAN "\x1B[36m"
#define C_WHITE "\x1B[37m"
#define C_RESET "\033[0m"
//bold
#define CB_RED "\x1B[1;31m"
#define CB_GREEN "\x1B[1;32m"
#define CB_YELLOW "\x1B[1;33m"
#define CB_BLUE "\x1B[1;34m"
#define CB_MAGENTA "\x1B[1;35m"
#define CB_CYAN "\x1B[1;36m"
#define CB_WHITE "\x1B[1;37m"

//msdp defines.
#define MSDP_VAR                       1
#define MSDP_VAL                       2
#define MSDP_TABLE_OPEN                3
#define MSDP_TABLE_CLOSE               4
#define MSDP_ARRAY_OPEN                5
#define MSDP_ARRAY_CLOSE               6
#define MAX_MSDP_SIZE                  100
//MSDP indexes

#define MSDP_CHARNAME 0
#define MSDP_SERVERTIME 1
#define MSDP_AFFECTS 2
#define MSDP_EXPERIENCE 3
#define MSDP_MAXEXPERIENCE 4
#define MSDP_TNL 5
#define MSDP_HEALTH 6
#define MSDP_MAXHEALTH 7
#define MSDP_LEVEL 8
#define MSDP_MONEY 9
#define MSDP_STR 10
#define MSDP_DEX 11
#define MSDP_CON 12
#define MSDP_INT 13
#define MSDP_PER 14
#define MSDP_BASESTR 15
#define MSDP_BASEDEX 16
#define MSDP_BASECON 17
#define MSDP_BASEINT 18
#define MSDP_BASEPER 19
#define MSDP_OPPONENTHEALTH 20
#define MSDP_MAXOPPONENTHEALTH 21
#define MSDP_OPPONENTNAME 22
#define MSDP_AREA 23
#define MSDP_EXITS 24
#define MSDP_ROOM 25
#define MSDP_RVNUM 29
#define MSDP_MAX 30
#endif
