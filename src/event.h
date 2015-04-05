#pragma once
#ifndef EVENT_H
#define EVENT_H
#include <sys/time.h>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "mud.h"
#include "conf.h"

/*
*EventArgs
*Any event passing args to it's caller will have a class that inherits this class
*/

class EventArgs
{
public:
    virtual ~EventArgs() {}
};

/*
*Our main event callback
*/
typedef void (*EVENTCB)(EventArgs*,void*);
typedef std::function<void (EventArgs*, void*)> EVENTFUNC;
/*
*This holds our event function object, as well as an ID associated with each event.
*/
struct EventContainer
{
    EVENTFUNC cb;
    UINT id;
#ifdef MODULE_SCRIPTING
    BOOL script;
    int func;
    Entity* obj;
#endif
};

/*
*Event function declarations
*/
#define EVENT(name)\
void name(EventArgs* args, void* caller)
#define CEVENT(c, name)\
void c::name(EventArgs* args,void* caller)

/*
*Main event class.
*Events will be declared with this; any other types of events should inherit this class.
*/
class Event
{
protected:
    std::vector<EventContainer*> *_callbacks;
    UINT _id;
public:
    Event();
    virtual ~Event();
    /*
    *Add and remove a callback from the event list.
    */
    virtual BOOL operator +=(const EVENTFUNC cb);
    virtual BOOL operator -=(UINT id);
    virtual UINT Add(const EVENTFUNC cb);
    virtual BOOL Remove(UINT id);
#ifdef MODULE_SCRIPTING
    virtual UINT AddScriptCallback(Entity* obj, int func);
#endif
    /*
    *Trigger all events in the callback list, passing args.
    *Params: [in] EventArgs specific to this event.
    *[in] A pointer to the caller object (if any).
    */
    virtual void Invoke(EventArgs *args,void* caller);
};
#endif
