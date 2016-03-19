#pragma once
/**
* These macros are used for defining event IDS and retrieving information from them.
* Unless you are defining your own IDs, you only need to use the constants provided.
*/
//Global events
#define GEVT(v)(((v<<3)&0xFFFFFFFC)|0)
//Room events
#define REVT(v)(((v<<3)&0xFFFFFFFC)|1)
//Static object events
#define SEVT(v)(((v<<3)&0xFFFFFFFC)|2)
//NPC events
#define NEVT(v)(((v<<3)&0xFFFFFFFC)|3)
//UUID based events
#define UEVT(v)(((v<<3)&0xFFFFFFFC)|4)

/**
* Requests the event type.
* returns the type of the value provided.
*/
#define EVTTYPE(v)((v)&0x7)\
/**
* Requests the event value.
* returns the value of the value provided.
*/
#define EVTVAL(v)(v>>3)

//global events
#define GEVENT_LIVING_PULSE GEVT(1)
#define GEVENT_WORLD_PULSE GEVT(2)
#define GEVENT_PLAYER_CONNECT GEVT(3)
#define GEVENT_PLAYER_DISCONNECT GEVT(4)
#define GEVENT_PLAYER_CREATED GEVT(5)
#define GEVENT_PLAYEr_DELETED GEVT(6)
#define GEVENT_SHUTDOWN GEVT(7)
#define GEVENT_COPYOVER GEVT(8)
#define GEVENT_OBJECT_LOADED GEVT(9)
#define GEVENT_OBJECT_DESTROYED GEVT(10)
#define GEVENT_MAX 1000

//room events
#define REVENT_ON_ENTER REVT(1)
#define REVENT_ON_EXIT REVT(2)
#define REVENT_ON_LOOK REVT(3)
#define REVENT_MAX 1000

//telnet events
#define UEVENT_TP_ON_NEGOTIATION UEVT(1)
#define UEVENT_TP_ON_OPTION UEVT(2)
//editor events
#define UEVENT_EDITOR_SAVE UEVT(1001)
#define UEVENT_EDITOR_LOAD UEVT(1002)
#define UEVENT_EDITOR_ABORT UEVT(1003)
#define UEVENT_EDITOR_ATEXIT UEVT(1004)


#define UEVENT_MAX 35000
