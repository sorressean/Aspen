#include "mud.h"
#include "event.h"
#include "eventManager.h"
#include "exception.h"

EventManager::~EventManager()
{
    for (auto it:_events)
        {
            if (it.second)
                {
                    delete it.second;
                }
        }
}

bool EventManager::IsEventRegistered(const std::string &name)
{
    return (_events.count(name)==0?false:true);
}

Event* EventManager::GetEvent(const std::string &name)
{
    if (!IsEventRegistered(name))
        {
            return nullptr;
        }

    return _events[name];
}

bool EventManager::RegisterEvent(const std::string &name)
{
    if (IsEventRegistered(name))
        {
            return false;
        }

    _events[name] = nullptr;
    return true;
}
bool EventManager::RemoveEvent(const std::string &name)
{
    if (IsEventRegistered(name))
        {
            auto event = _events[name];
            if (event)
                {
                    delete event;
                }
            _events.erase(name);
            return true;
        }

    return false;
}

bool EventManager::CallEvent(const std::string &name, EventArgs* args, void* caller)
{
    if (!IsEventRegistered(name))
        {
            throw(EventNotFoundException("Tried to call "+name+"."));
            return false;
        }

//this will be true if there are no events attached.
    if (_events[name]==nullptr)
        {
            return true;
        }

    (_events[name])->Invoke(args,caller);
    return true;
}

bool EventManager::AddCallback(const std::string &name, const EVENTFUNC cb)
{
    if (!IsEventRegistered(name))
        {
            throw(EventNotFoundException("Tried to call "+name+"."));
            return false;
        }

    if (!_events[name])
        {
            _events[name] = new Event();
        }
    (_events[name])->Add(cb);
    return true;
}
#ifdef MODULE_SCRIPTING
bool EventManager::AddScriptCallback(Entity* obj, const char* event, int func)
{
    if (!IsEventRegistered(event))
        {
            throw(EventNotFoundException("Tried to call "+std::string(event)+"."));
            return false;
        }

    if (!_events[event])
        {
            _events[event] = new Event();
        }
    (_events[event])->AddScriptCallback(obj, func);
    return true;
}
#endif
