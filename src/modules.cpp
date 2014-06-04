#include "modules.h"
#include "player.h"
#include "socials.h"
#ifdef MODULE_SYSLOG
#include "syslog/syslog.h"
#endif
#ifdef MODULE_HELP
#include "help/help.h"
#endif
#ifdef MODULE_SCRIPTING
#include "scripts/scripts.h"
#endif
#ifdef OLC
#include "olcs.h"
#endif

BOOL InitializeModules()
{
#ifdef MODULE_SCRIPTING
    if (!InitializeScript())
        {
            return false;
        }
#endif

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

    if (!InitializeSocials())
        {
            return false;
        }

    InitializePlayer();

    return true;
}
