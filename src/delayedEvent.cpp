#include "conf.h"
#include "mud.h"
#include "event.h"
#include "delayedEvent.h"

DelayedEvent::DelayedEvent(int sec,int msec)
{
    SetDelay(sec,msec);
}

DelayedEvent::DelayedEvent()
{
    SetDelay(0,0);
}

void DelayedEvent::SetDelay(int sec,int msec)
{
    msec*=1000;
    _fireTime.tv_sec=sec;
    _fireTime.tv_usec=msec;
    gettimeofday(&_lastTime,NULL);
}

void DelayedEvent::Invoke(EventArgs *args,void* caller)
{
    timeval curtime;
    std::vector <EventContainer*>::iterator it, itEnd;
    unsigned int secs = 0; //seconds between last time and now
    unsigned int msecs = 0; //microseconds between last time and now
    unsigned int felapse; //the full elapsed time that _fireTime requires.
    unsigned int celapse; //the time elapsed between now and last call

    gettimeofday(&curtime,NULL);
    secs=(int)(curtime.tv_sec-_lastTime.tv_sec)*1000;
    msecs=(int)(curtime.tv_usec-_lastTime.tv_usec)/1000;
    felapse=((_fireTime.tv_sec*1000)+(_fireTime.tv_usec/1000));
    celapse=msecs+secs;

    if (celapse>=felapse)   //the time between current time and last time elapsed is greater or equal to fire time
        {
            itEnd = _callbacks->end();
            for (it = _callbacks->begin(); it != itEnd; ++it)
                {
                    (*it)->cb(args,caller);
                }
//update the last time the event was fired:
            gettimeofday(&_lastTime,NULL);
        }
}
