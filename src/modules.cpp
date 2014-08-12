#include "modules.h"
#include "player.h"
#include "socials.h"
#ifdef MODULE_SYSLOG
#include "syslog/syslog.h"
#endif
#ifdef MODULE_HELP
#include "help/help.h"
#endif
#include "scripts/script.h"
#ifdef OLC
#include "olc.h"
#include "olcs.h"
#endif

BOOL InitializeModules()
{
    if (!InitializeScript())
        {
            return false;
        }

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
    if (!InitializeOlc())
        {
            return false;
        }
    if (!InitializeOlcs())
        {
            return false;
        }
#endif

    if (!InitializeSocials())
        {
            return false;
        }

    InitializePlayer();

    return true;
}
