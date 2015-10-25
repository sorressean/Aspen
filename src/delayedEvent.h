/**
* Contains: DelayedEvent
*/
#ifndef DELAYED_EVENT_H
#define DELAYED_EVENT_H
#include "conf.h"
#include "mud.h"
#include "event.h"

/**
* These are used to make events go off on a certain amount of time, rather than
* When the invoke method is called once per pulse.
*/
class DelayedEvent:public Event
{
    timeval _fireTime; //how much time before we fire?
    timeval _lastTime; //when was the last time this event was fired?
public:
    /**
    * constructs a DelayedEvent.
    *
    * @param sec [in] The number of seconds between each fire.
    * @param msec [in] The number of milliseconds between each fire.
    */
    DelayedEvent(int sec,int msec);
    DelayedEvent();
    ~DelayedEvent() = default;

    /**
    * Sets the delay for the event.
    *
    * @param [in] sec The number of seconds between each fire.
    * @param [in] msec The number of milliseconds between each fire.
    */
    void SetDelay(int sec,int msec);
    /**
        * Trigger all events in the callback list, passing args.
    *
    * @params [in] args EventArgs specific to this event.
    * @param [in] caller A pointer to the caller object (if any).
    */
    void Invoke(EventArgs *args,void* caller);
};
#endif
