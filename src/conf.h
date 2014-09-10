/**
*configuration defines for the mud
*Holds configuration and other constants.
*/
#pragma once
#ifndef CONFIG_H
#define CONFIG_H
//Mud configuration constants
#define RDNS
#define DEFAULT_PORT 6666 //our default port: The mud will use this, if the port isn't provided as an argument.
#define LISTEN_BACKLOG 5 //the backlog for the listening socket.
#define PULSES_PER_SECOND 10 //how many game loop itterations will be ran each second
#define MS_PER_PULSE 1000/PULSES_PER_SECOND
#define LOG_CONSOLE //will tell the logger to write to console. Comment out if you do not wish this.
#define LOG_MAXSIZE 1024*1024 //1mb
#define LOG_MAXFILES 10 //maximum number of logs to keep.
#define MIN_LOG_LEVEL (LOG_LEVEL)WARN //the minimum level of logging that will be written to console.
#define MUD_NAME "Aspen Mud" //name of your mud.
#define MAX_CHAN_HISTORY_LENGTH 20 //the maximum number of entries channels will log.
#define GAME_IDLE_TIME (60*60) //the amount of time someone will be able to idle before they are kicked.
#define LOGIN_IDLE_TIME (30) //the amount of time someone will be able to idle at login.
#define LINKDEATH_TIME (60*10)
#define ELAPSED_STARTUP //shows the elapsed time it took the mud to initialize.
/*
*Comment this out if you want all scripting functions enabled by default.
*Leaving it commented will remove functions like SetPassword on player from being exposed to scripting.
*/
#define SCRIPT_SECURE
#define PROFILE_CALLOUTS
//delayed defines
#define LIVING_PULSE 2 //seconds for each living heartbeat to take place.
#define WORLD_PULSE 10 //the world pulse, used for room resets, etc.
#define SAVE_INTERVAL 180 //90 seconds.
#define BACKUP_INTERVAL 300 //2 minutes.
//Defines for locating files:
#define MOTD_FILE "../txt/motd" //message of the day shown after login
#define LOGIN_FILE "../txt/welcome" //banner shown at login
#define COPYOVER_FILE "../copyover.dat" //stores player connection info
#define BIN_FILE "../bin/aspen" //used for copyovers.
#define SETTINGS_FILE "../data/mud.dat"
#define EVENT_FILE "../logs/events.log" //our main log file
#define EVENT_NAME "events" //the actual name of the event log.
#define SCRIPT_FILE "../logs/script.log" //the name of the scripting log file.
#define SCRIPT_NAME "script"
#define HELP_FILE "../data/help.dat"
#define SOCIALS_FILE "../data/socials.dat"
//directories:
#define PLAYER_DIR "../data/players/"
#define STATE_DIR "../data/state/"
#define BACKUP_DIR "../data/backups/"
#define AREA_DIR "../data/areas/"
#define AREA_STARTFILE "../data/areas/start"
//player rankings:
#define RANK_PLAYER 1<<1
#define RANK_PLAYTESTER 1<<2
#define RANK_NEWBIEHELPER 1<<3
#define RANK_NEWBIE 1<<4
#define RANK_BUILDER 1<<5
#define RANK_ADMIN 1<<29
#define RANK_GOD 1<<30
//flags
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

#define VNUMKEEP 100 //how many spare vnums to store.
//logger levels
enum LOG_LEVEL {INFORM,WARN,ERR,CRIT,SCRIPT, PLAYER, CONNECTION};
//Message types:
enum MessageType {MSG_NORMAL, MSG_ERROR,MSG_INFO,MSG_CRITICAL,MSG_CHANNEL,MSG_LIST};

//terrain types:
enum class TERRAIN
{
    OCEAN, UNDERWATER, AIR, PLANE, DESERT, UNDERGROUND, SWAMP, HILLS, MOUNTAIN, FOREST, LIGHTFOREST, CITY, INSIDE, MAX
};
//positions:
enum class POSITION
{
    any, unconcious, sleeping, sitting, standing, flying
};
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
enum ExitDirection {nowhere, north, south, east, west, northeast, northwest, southeast, southwest, up, down};
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

#define MAX_PASSWORD 20
#define MIN_PASSWORD 5
//module defines
//below, define all the modules you want to be compiled in.
#define MODULE_SCRIPTING
#define MODULE_SYSLOG
#define MODULE_HELP
#define OLC
//Leave this commented if you want the code to check for files and create them if they do not exist. see socials.cpp and zone.cpp for examples.
//#define NO_INIT_DEFAULTS
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
#define VERSION_STR "Sapphire Mud v0.1.1"
#define MAX_LEVEL 251
#define MAX_MOB_LEVEL 301
#endif
