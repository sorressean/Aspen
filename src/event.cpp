#include <sys/time.h>
#include <string>
#include <vector>
#include <functional>
#include "event.h"
#include "exception.h"
Event::Event():
    _id(0)
{
}
Event::~Event()
{
    for (auto it: _callbacks)
        {
            delete it;
        }
}

BOOL Event::operator +=(const EVENTFUNC cb)
{
    Add(cb);
    return true;
}
UINT Event::Add(const EVENTFUNC cb)
{
    EventContainer* c = new EventContainer();

    _id++;
    c->id = _id;
    c->cb = cb;
    _callbacks.push_back(c);
    return c->id;
}

BOOL Event::operator -=(UINT id)
{
    Remove(id);
    return true;
}
BOOL Event::Remove(UINT id)
{
    std::vector<EventContainer*>::iterator it, itEnd;

    itEnd = _callbacks.end();
    for (it = _callbacks.begin(); it != itEnd; ++it)
        {
            if (id == (*it)->id)
                {
                    _callbacks.erase(it);
                    return true;
                }
        }

    return false;
}

#ifdef MODULE_SCRIPTING
UINT Event::AddScriptCallback(Entity* obj, int func)
{
    /** @todo fix this for scripting. */
    return 0;
}
#endif

void Event::Invoke(EventArgs* args, void* caller)
{
    for (auto it: _callbacks)
        {
            it->cb(args,caller);
        }
}
