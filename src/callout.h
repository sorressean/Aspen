/*callouts are used to handle one-shot delayed tasks.
*Callouts are somewhat like Delayed events, but used to fire at a specific delay for
*Things like a door automagically closing, an affect on a player/etc.
*/
#pragma once
#include <sys/time.h>
#include <functional>
#include "mud.h"
#include "conf.h"

class Callout;
typedef std::function<void (Callout*)> CALLOUT_CB;

class Callout
{
    timeval _fireTime; //how much time before we fire?
    unsigned int _id;
    CALLOUT_CB _cb;
    bool _oneShot;
    int _sec;
    int _msec;
public:
    Callout(int sec, int msec, const CALLOUT_CB cb, bool oneShot=true);
    ~Callout();
    int GetId() const;
    void SetId(unsigned int id);
    int GetTime() const;
    int GetDelta() const;
    BOOL CanFire() const;
    void Fire();
    bool IsOneShot() const;
    void Reset();
};
