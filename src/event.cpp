#include <sys/time.h>
#include <string>
#include <vector>
#include <functional>
#include "event.h"
#include "exception.h"

Event::Event()
{
    _id = 0;
    _callbacks = new std::vector<EventContainer*>();
}
Event::~Event()
{
    std::vector<EventContainer*>::iterator it, itEnd;

    itEnd = _callbacks->end();
    for (it = _callbacks->begin(); it != itEnd; ++it)
        {
            delete (*it);
        }
    delete _callbacks;
    _callbacks = NULL;
}

UINT Event::Add(const EVENTFUNC cb)
{
    EventContainer* c = new EventContainer();
    if (!c)
        {
            return 0;
        }

    _id++;
    c->id = _id;
    c->cb = cb;
#ifdef MODULE_SCRIPTING
    c->script = false;
    c->obj = NULL;
#endif
    _callbacks->push_back(c);
    return c->id;
}
BOOL Event::Remove(UINT id)
{
    std::vector<EventContainer*>::iterator it, itEnd;

    itEnd = _callbacks->end();
    for (it = _callbacks->begin(); it != itEnd; ++it)
        {
            if (id == (*it)->id)
                {
                    _callbacks->erase(it);
                    return true;
                }
        }

    return false;
}
#ifdef MODULE_SCRIPTING
UINT Event::AddScriptCallback(Entity* obj, int func)
{
    EventContainer* c = new EventContainer();
    if (!c)
        {
            return 0;
        }

    _id++;
    c->id = _id;
    c->script = true;
    c->obj = obj;
    c->func = func;
    _callbacks->push_back(c);
    return c->id;
}
#endif

BOOL Event::operator +=(const EVENTFUNC cb)
{
    Add(cb);
    return true;
}
BOOL Event::operator -=(UINT id)
{
    Remove(id);
    return true;
}

void Event::Invoke(EventArgs* args, void* caller)
{
    std::vector <EventContainer*>::iterator it, itEnd;

    itEnd = _callbacks->end();
    for (it = _callbacks->begin(); it != itEnd; ++it)
        {
#ifdef MODULE_SCRIPTING
            if ((*it)->script)
                {
//                    SCR_CallEvent((*it)->obj, (*it)->func, args, caller);
                }
            else
                {
#endif
                    (*it)->cb(args,caller);
#ifdef MODULE_SCRIPTING
                }
#endif
        }
}
