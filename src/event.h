/**
* Contains: Event
*
* Events are used to notify systems of actions taking place.
* For example, if a player levels, you might have a leveled event.
* Other systems could subscribe to this event,
* which would allow them to notify channels.
* in short, use events when other systems might need to know about actions taking place.
* Make these systems subscribe to the event to receive the action.
*/
#pragma once
#ifndef EVENT_H
#define EVENT_H
#include <sys/time.h>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "mud.h"

/**
* Event arguments are passed to individual events.
*
* Inherit this object to create your own.
* For examples, see eventargs.h.
*/
class EventArgs
{
public:
    virtual ~EventArgs() {}
};

/**
* Event callback
*/
typedef void (*EVENTCB)(EventArgs*,void*);
/**
* The function object callbacks will be bound to.
*/
typedef std::function<void (EventArgs*, void*)> EVENTFUNC;

/**
*This holds our event function object, as well as an ID associated with each event.
*/
struct EventContainer
{
    EVENTFUNC cb;
    unsigned int id;
};

/**
* Use this macro to create event functions.
*/
#define EVENT(name)\
void name(EventArgs* args, void* caller)
/**
* Use this macro to create class events.
*/
#define CEVENT(c, name)\
void c::name(EventArgs* args,void* caller)

/**
* Use events when you wish to notify systems of actions.
*
* Subscribe to these actions through this class.
*/
class Event
{
protected:
    unsigned int _id;
    std::vector<EventContainer*> _callbacks;
public:
    Event();
    virtual ~Event();

    /**
    * Adds a callback to the event list.
    *
    * @param [in] cb the callback to add.
    */
    virtual bool operator +=(const EVENTFUNC cb);
    /**
    * Removes a callback from the list by ID.
    *
    * @param [in] id the ID of the callback.
    */
    virtual bool operator -=(unsigned int id);

    /**
    * Adds a callback to the event list.
    *
    * @param [in] cb the callback to add.
    * @return the new ID of the callback.
    */
    virtual unsigned int Add(const EVENTFUNC cb);
    /**
    * Removes a callback from the list by ID.
    *
    * @param [in] id the ID of the callback.
    * @return true if the callback can be removed, false otherwise.
    */
    virtual bool Remove(unsigned int id);

#ifdef MODULE_SCRIPTING
    /**
    * @todo figure out what this is supposed to do.
    */
    virtual unsigned int AddScriptCallback(Entity* obj, int func);
#endif
    /**
    * Trigger all events in the callback list, passing args.
    *
    * @param [in] args EventArgs specific to this event.
    * @param [in] caller The caller object (if any).
    */
    virtual void Invoke(EventArgs *args,void* caller);
};
#endif
