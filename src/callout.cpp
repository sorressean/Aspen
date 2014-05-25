#include <cstdlib>
#include <functional>
#include <sys/time.h>
#include "mud.h"
#include "conf.h"
#include "callout.h"

Callout::Callout(int sec, int msec, const CALLOUT_CB cb, bool oneShot)
{
    _sec = sec;
    _msec = msec;
    _oneShot = oneShot;
    _id = 0;
    _cb = cb;
    Reset();
}
Callout::~Callout()
{
}

int Callout::GetId() const
{
    return _id;
}
void Callout::SetId(unsigned int id)
{
    _id = id;
}
int Callout::GetTime() const
{
    return (_fireTime.tv_sec * 1000) + _fireTime.tv_usec/1000;
}
int Callout::GetDelta() const
{
    timeval curtime;
    int fdelta = (_fireTime.tv_sec * 1000) + (_fireTime.tv_usec/1000);
    int cdelta = 0;

    gettimeofday(&curtime, NULL);
    cdelta = (curtime.tv_sec * 1000) + (curtime.tv_usec/1000);

    return cdelta-fdelta;
}
BOOL Callout::CanFire() const
{
    int extra = 0;
    timeval curtime;

    gettimeofday(&curtime, NULL);
    extra = ((_fireTime.tv_sec - curtime.tv_sec) * 1000);
    extra += ((_fireTime.tv_usec - curtime.tv_usec) / 1000);
    return (extra <= 0? true : false);
}
void Callout::Fire()
{
    _cb(this);
}
bool Callout::IsOneShot() const
{
    return _oneShot;
}
void Callout::Reset()
{
    int extra = 0;
    timeval curtime;

    gettimeofday(&curtime, NULL);
    _fireTime.tv_sec = curtime.tv_sec + _sec;
    extra = curtime.tv_usec + (_msec * 1000);
    while (extra >= 1000000)
        {
            _fireTime.tv_sec +=1;
            extra-=1000000;
        }
    _fireTime.tv_usec = extra;
}
