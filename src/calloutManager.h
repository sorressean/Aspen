/**
* Contains: CalloutManager
*
* The callout manager is used to manage and fire callouts.
*/
#pragma once
#include <queue>
#include <deque>
#include "mud.h"
#include "conf.h"
#include "callout.h"

/**
* Used to compare for the priority queue.
*/
struct _cm_comp
{
    bool operator ()(Callout* a, Callout* b)
    {
        return (a->GetTime() > b->GetTime() ? true : false);
    }
};

/**
* Manages callouts.
*/
class CalloutManager
{
    unsigned int _curid;
    static CalloutManager* _instance;
    std::priority_queue<Callout*, std::vector<Callout*>, _cm_comp> _callouts;
#ifdef PROFILE_CALLOUTS
    unsigned long long int _fired;
    unsigned long long int _delta;
    unsigned long long int _deltaMin;
    unsigned long long int _deltaMax;
#endif
    CalloutManager();
    virtual ~CalloutManager();
public:
    /**
    * Initializes the callout manager.
    */
    static void Initialize();
    /**
    * Cleans up resources used by the callout manager.
    */
    static void Release();
    /**
    * Returns a pointer to the callout manager.
    */
    static CalloutManager* GetInstance();
    /**
    * Updates all callouts held by the manager.
    */
    void Update();
    /**
    * Registers a callout.
    *
    * @param [in] sec the seconds for the callout to elapse.
    * @param [in] the MS for the callout to elapse.
    * @param [in] cb the callback.
    * @param [in] oneShot whether or not the callout will fire more than once.
    */
    unsigned int RegisterCallout(int sec, int msec, const CALLOUT_CB cb, bool oneShot=true);
    /**
    * Returns profiling results.
    */
    std::string Profile() const;
};
