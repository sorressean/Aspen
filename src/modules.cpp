#include "modules.h"
#include "player.h"
#include "socials.h"
#include "syslog/syslog.h"
#include "scripts/script.h"
#include "olc.h"
#include "olcs.h"
#include "modules/modules.h"
#include "extensions/extensions.h"

bool InitializeModules()
{
    if (!InitializeScript())
        {
            return false;
        }

    if(!InitializeSyslog())
        {
            return false;
        }

    if (!InitializeOlc())
        {
            return false;
        }
    if (!InitializeOlcs())
        {
            return false;
        }

    if (!InitializeSocials())
        {
            return false;
        }

    InitializePlayer();

    if (!InitializeExternalModules())
        {
            return false;
        }
    if (!InitializeExternalExtensions())
        {
            return false;
        }

    return true;
}
