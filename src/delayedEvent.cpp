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
    unsigned int secs = 0; //seconds between last time and now
    unsigned int msecs = 0; //microseconds between last time and now
    unsigned int felapse = 0; //the full elapsed time that _fireTime requires.
    unsigned int celapse = 0; //the time elapsed between now and last call

    gettimeofday(&curtime,NULL);
    secs=(int)(curtime.tv_sec-_lastTime.tv_sec)*1000;
    msecs=(int)(curtime.tv_usec-_lastTime.tv_usec)/1000;
    felapse=((_fireTime.tv_sec*1000)+(_fireTime.tv_usec/1000));
    celapse=msecs+secs;

//the time between current time and last time elapsed is greater or equal to fire time
    if (celapse>=felapse)
        {
            for (auto it: _callbacks)
                {
                    it->cb(args,caller);
                }

//update the last time the event was fired:
            gettimeofday(&_lastTime,NULL);
        }
}
