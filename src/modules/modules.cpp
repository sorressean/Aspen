#include "../mud.h"
#include "../conf.h"
#include "modules.h"
#include "board/boardModule.h"

bool InitializeExternalModules()
{
    if (!InitializeBoards())
        {
            return false;
        }

    return true;
}
