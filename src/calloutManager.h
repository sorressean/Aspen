#pragma once
#include <queue>
#include <deque>
#include "mud.h"
#include "conf.h"
#include "callout.h"

struct _cm_comp
{
    bool operator ()(Callout* a, Callout* b)
    {
        return (a->GetTime() > b->GetTime() ? true : false);
    }
};

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
    CalloutManager(CalloutManager& m);
    virtual ~CalloutManager();
public:
    static void Initialize();
    static void Release();
    static CalloutManager* GetInstance();
    void Update();
    unsigned int RegisterCallout(int sec, int msec, const CALLOUT_CB cb, bool oneShot=true);
    std::string Profile() const;
};
