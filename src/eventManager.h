#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#include "conf.h"
#include "mud.h"
#include "event.h"

class EventManager
{
    std::map <std::string, Event*> _events;
public:
    EventManager();
    ~EventManager();
    /*
    *Checks to see if the event is registered.
    *Param: [in] the name of the event.
    *Return: True on success, false on failure.
    */
    BOOL IsEventRegistered(const std::string &name);
    /*
    *Retrieves an event
    *Param: [in] the name of the event.
    *Return: A pointer to the event, NULL if no event was found.
    */
    Event* GetEvent(const std::string &name);
    /*
    *Registers an event with the object.
    *Param: [in] the name of the event to register.
    *Param: [in] a pointer to the event object.
    *Return: True on success, false on failure.
    */
    BOOL RegisterEvent(const std::string &name, Event* event);
    /*
    *Removes the event from the events table.
    *Param: [in] the name of the event.
    *Return: True on success, false on failure.
    */
    BOOL RemoveEvent(const std::string &name);
    /*
    *Calls the event, passing in the caller and event args.
    *Param: [in] the name of the event.
    *[in] The event args.
    *[in] the event caller.
    *Return: True on success, false on failure.
    */
    BOOL CallEvent(const std::string &name, EventArgs* args, void* caller);
    /*
    *Adds a callback to the event.
    *Param: [in] the name of the event.
    *[in] the callback.
    */
    BOOL AddCallback(const std::string &name, const EVENTFUNC cb);
#ifdef MODULE_SCRIPTING
    /*
    *Adds a script event callback.
    *Param: [in] a pointer to the lua state.
    *[in] the name of the event.
    *[in] the name of the callback.
    *Return: True on success, false on failure.
    */
    BOOL AddScriptCallback(Entity* obj, const char* event, int func);
#endif
};
#endif
