/**
*Contains: Callout
*
* Ccallouts are used to handle one-shot delayed tasks.
*Callouts are somewhat like Delayed events, but used to fire at a specific delay for
*Things like a door automagically closing, an affect on a player/etc.
*/
#pragma once
#include <sys/time.h>
#include <functional>
#include "mud.h"
#include "conf.h"

class Callout;

/**
* Callout callback.
*/
typedef std::function<void (Callout*)> CALLOUT_CB;

/**
* Callouts are timed events.
*/
class Callout
{
    int _sec;
    int _msec;
    CALLOUT_CB _cb;
    bool _oneShot;
    timeval _fireTime; //how much time before we fire?
    unsigned int _id;
public:
    /**
    * Constructs a callout provided a specific time.
    *
    * @param [in] sec The seconds to elapse before firing.
    * @param [in] msec The MS that should elapse before event is fired.
    * @param [in] cb the callout's callback.
    * @param [in] oneShot determines whether or not the callout should be requeued after it is fired, or if it should only fire once.
    */
    Callout(int sec, int msec, const CALLOUT_CB cb, bool oneShot=true);
    ~Callout() = default;

    /**
    * Gets the ID of the callout.
    *
    * An ID is provided to a callout once it is queued.
    * Until that point, the ID will be 0.
    */
    int GetId() const;
    /**
    * Sets the ID of the callout.
    *
    * @param [in] id the new ID of the callout.
    */
    void SetId(unsigned int id);
    /**
    * Returns the time the callout will elapse.
    *
    * @return The total time, in MS for the duration of the callout.
    */
    int GetTime() const;
    /**
    * Gets the remaining time for the callout.
    *
    * @return The delta, the time remaining before the callout fires.
    */
    int GetDelta() const;
    /**
    * Determines if the callout can fire.
    *
    * @return True if the callout's elapse time is complete, false otherwise.
    */
    BOOL CanFire() const;
    /**
    * Fires the callout.
    * This means the time specified has elapsed and the callback will be called.
    */
    void Fire();
    /**
    * Determines if the callback is a one shot.
    *
    * @return True if the callout is a oneshot (meaning it fires only once), false otherwise).
    */
    bool IsOneShot() const;
    /**
    * Resets the time until the callout will be called.
    * Mostly useful for callouts that are not oneshots.
    */
    void Reset();
};
