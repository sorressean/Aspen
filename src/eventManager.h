/**
* Contains: EventManager
*
* The event manager is responsible for managing access to a collection of events.
* Use this any time you might have more than one event associated with a specific class.
*/
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include "mud.h"
#include "event.h"

/**
* This is a container for multiple events.
*
* manages subscriptions to the actual events.
* Also manages access and provides a way to invoke the event.
*/
class EventManager
{
    std::map <std::string, Event*> _events;
public:
    EventManager() = default;
    ~EventManager();
    /**
    * Checks to see if the event is registered.
    *
    * @param [in] name the name of the event.
    * @return True on success, false on failure.
    */
    BOOL IsEventRegistered(const std::string &name);

    /**
    *Retrieves the specified eventevent
    *
    * @param [in] name the name of the event.
    * @return A pointer to the event, nullptr if it was not found.
    */
    Event* GetEvent(const std::string &name);

    /**
    * Registers an event.
    *
    * @param [in] name the name of the event to register.
    * @param [in] event The event object.
    * @return True on success, false on failure.
    */
    BOOL RegisterEvent(const std::string &name, Event* event);
    /**
    * Removes the event from the events table.
    *
    * @param [in] name the name of the event.
    * @return True on success, false on failure.
    */
    BOOL RemoveEvent(const std::string &name);

    /**
    *Calls the event, passing in the caller and event args.
    *
    * @param [in] name the name of the event.
    * @param [in] args The event args.
    * @param [in] caller the event caller.
    * @return True on success, false on failure.
    */
    BOOL CallEvent(const std::string &name, EventArgs* args, void* caller);

    /**
    * Adds a callback to the event.
    *
    * @param [in] name the name of the event.
    * @param [in] cb the callback.
    */
    BOOL AddCallback(const std::string &name, const EVENTFUNC cb);
#ifdef MODULE_SCRIPTING
    /**
    * @todo figure out what this is doing and revamp with scripting.
    *Adds a script event callback.
    *
    * @param [in] name the name of the event.
    *[in] the name of the callback.
    *Return: True on success, false on failure.
    */
    BOOL AddScriptCallback(Entity* obj, const char* event, int func);
#endif
};
#endif
