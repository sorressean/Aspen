#ifndef DELAYED_EVENT_H
#define DELAYED_EVENT_H
#include "conf.h"
#include "mud.h"
#include "event.h"

/*
*Delayed events
*These are used to make events go off on a certain amount of time, rather than
*When the invoke method is called one per pulse.
*/
class DelayedEvent:public Event
{
    timeval _fireTime; //how much time before we fire?
    timeval _lastTime; //when was the last time this event was fired?
public:
    /*
    *Constructor:
    *Params: [in] The number of seconds between each fire.
    *[in] The number of milliseconds between each fire.
    */
    DelayedEvent(int sec,int msec);
    DelayedEvent();
    /*
    *Sets the delay for the event.
    *Params: [in] The number of seconds between each fire.
    *[in] The number of milliseconds between each fire.
    */
    void SetDelay(int sec,int msec);
    /*
        *Trigger all events in the callback list, passing args.
    *Params: [in] EventArgs specific to this event.
    *[in] A pointer to the caller object (if any).
    */
    void Invoke(EventArgs *args,void* caller);
};
#endif
