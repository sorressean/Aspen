#include <unordered_map>
#include <tuple>
#include <functional>
#include "mud.h"
#include "event.h"

EventTable* EventTable::_ptr;

Event* EventCreator()
{
return new Event();
}

EventTable::~EventTable()
{
    for (auto it: _events)
        {
            delete it.second;
        }
}
void EventTable::initialize()
{
    _ptr = new EventTable();
}
void EventTable::Release()
{
    if (_ptr)
        {
            delete *_ptr;
            _ptr = nullptr;
        }
}
EventTable* EventTable::GetPtr()
{
    return _ptr;
}

bool EventTable::IsEventRegistered(VNUM onum, EVENTNUM evt)
{
    EventKey key = std::make_tuple(onum, evt);
    return (_events.count(key)? true : false);
}
Event* EventTable::GetEvent(VNUM onum, EVENTNUM evt)
{
    EventKey key;

    if (IsEventRegistered(onum, evt))
        {
            key = std::make_tuple(onum, evt);
            return _events[key];
        }

    return nullptr;
}
unsigned int EventTable::AddCallback(VNUM onum, EVENTNUM evt, const EVENTFUNC cb)
{
    Event* event = nullptr;
    EventKey key;

    event = GetEvent(onum, evt);
    if (!event)
        {
if (MetadataExists(evt))
{
            event = (_eventMeta[evt])();
}
else
{
event = new Event();
}
key = std::make_tupal(onum, evt);
_events[key] = event;
        }

    return event->Add(cb);
}
void EventTable::Invoke(VNUM onum, EVENTNUM evt, EventArgs *args=nullptr, void* caller = nullptr)
{
    Event* event = nullptr;

    event = GetEvent(onum, evt);
    if (event)
        {
            event->Invoke(args, caller);
        }
}

bool EventTable::MetadataExists(EVENTNUM evt)
{
return (_eventMeta.count(evt) ? true : false);
}
void EventTable::RegisterMetadata(EVENTNUM evt, EventCreate func)
{
    _eventMeta[evt] = func;
}
