#include "modules.h"
#include "olcs.h"
#include "player.h"
#ifdef MODULE_SYSLOG
#include "syslog/syslog.h"
#endif
#ifdef MODULE_HELP
#include "help/help.h"
#endif

BOOL InitializeModules()
{
#ifdef MODULE_SYSLOG
    if(!InitializeSyslog())
        {
            return false;
        }
#endif
#ifdef MODULE_HELP
    if(!InitializeHelp())
        {
            return false;
        }
#endif
#ifdef OLC
    if (!InitializeOlcs())
        {
            return false;
        }
#endif
    InitializePlayer();

    return true;
}
