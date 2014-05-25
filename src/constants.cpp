#include "conf.h"
#include "mud.h"

static int level_cost[MAX_LEVEL];
static int level_cap[MAX_LEVEL];

void InitializeConstants()
{
    int i = 0;

    for (i = 1; i <= MAX_LEVEL; i++)
        {
            level_cost[i - 1] = 16000 + 2500*i + 1000*i*i;
            level_cap[i-1] = (level_cost[i-1]/15);
        }
}

int GetLevelCost(int level)
{
    if (level > MAX_LEVEL)
        {
            return 0;
        }

    return level_cost[level-1];
}
int GetLevelCap(int level)
{
    return level_cap[level-1];
}

int GetExperienceGain(int m, int y, int d)
{
    return (150*d*m*(d + m))/y;
}
