/**
* Contains: Event table.
*/
#pragma once
#include <unordered_map>
#include <tuple>
#include <functional>
#include <map>
#include "mud.h"
#include "event.h"

typedef unsigned int EVENTNUM;
typedef std::tuple<unsigned long long int, EVENTNUM> EventKey;
typedef std::function<Event* ()> EventCreate;
Event* EventCreator();

/**
* The event table holds a set of all events in the mud.
*
* Different event types include instanced events, which belong to objects with UUIDs
* Also numbered events, which are attached to objects with vnums.
*
* The event table provides a transparent memory-friendly mechenism to manage events and their callbacks.
* Event metadata is used to determine which type of event to create.
* When event callbacks are registered, we can check to see if the event object exists.
* If the object does not exist, we populate the hashtable with the event and add the callbacks.
* Invokes also work similarly in that they check for the existance of an entry
* prior to calling the event.
* If no entry was found, we can still return as if we called the event on the object.
* This is incredibly useful; this enables us to only allocate memory for those specific objects which require event callbacks.
*/
class EventTable
{
    static EventTable* _ptr;
    std::map<EventKey, Event*> _events;
    std::unordered_map<EVENTNUM, EventCreate> _eventMeta;
    EventTable() = default;
    ~EventTable();
public:
    /**
    * Initializes the event table and the singleton instance.
    */
    static void Initialize();
    /**
    * Frees the event table and all instance data.
    */
    static void Release();
    /**
    * Gets the pointer to the event table instance.
    */
    static EventTable* GetPtr();

    /**
    * Checks to see if the specified event is registered.
    *
    * @param [in] onum the object number.
    * @param [in] evt the event id.
    * @return True if the event is registered (there exists one or more callbacks), false otherwise.
    */
    bool IsEventRegistered(VNUM onum, EVENTNUM evt);
    /**
    * Retrieves the pointer to the event handler.
    *
    * The event handler is used to add callbacks, etc.
    * @param [in] onum the vnum of the object to retrieve data for.
    * @param [in] evt the ID of the event.
    * @return a pointer to the event handler, nullptr if it does not exist.
    */
    Event* GetEvent(VNUM onum, EVENTNUM evt);
    /**
    * Adds a callback to the specified event.
    *
    * @param [in] onum the object vnum.
    * @param [in] evt the event ID.
    * @param [in] cb the Callback.
    * @return the ID of the callback being added.
    */
    unsigned int AddCallback(VNUM onum, EVENTNUM evt, const EVENTFUNC cb);
    /**
    * Invokes the specified event.
    *
    * @param [in] onum the object vnum.
    * @param [in] evt the event ID.
    * @param [in] args the event arguments.
    * @param [in] caller a pointer to the calling object.
    */
    void Invoke(VNUM onum, EVENTNUM evt, EventArgs *args=nullptr, void* caller = nullptr);
/**
* Determines if the specified metadata exists.
*
* @param [in] evt the ID of the event.
* @return true on success, false if the metadata does not exist.
*/
bool MetadataExists(EVENTNUM evt);
    /**
    * Registers event metadata.
    *
    * The event metadata tells the event table which type of event to create.
    * This is important to enable the event table to manage events in the background.
* If metadata does not exist for a specific event, we create a generic Event object.
* This is useful to prevent registering basic metadata for every event,
* this also helps us save a bit of memory.
    * @param [in] evt the ID of the event.
    * @param [in] func the function to call when creating the event.
    */
    void RegisterMetadata(EVENTNUM evt, EventCreate func=EventCreator);
};
