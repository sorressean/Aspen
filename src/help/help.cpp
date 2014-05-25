#include "../mud.h"
#include "../conf.h"
#include "../world.h"
#include "../player.h"
#include "../utils.h"
#include "../inputHandlers.h"
#include "../olc.h"
#include "../editor.h"
#include "../event.h"
#include "../eventargs.h"
#include "help.h"
#include "HelpEntry.h"
#include "HelpTable.h"

#ifdef MODULE_HELP

CMDHelp::CMDHelp()
{
    SetName("help");
    AddAlias("h");
    SetType(normal);
}
BOOL CMDHelp::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    HelpTable* table = NULL;

    if (!args.size())
        {
            mobile->Message(MSG_ERROR, "Syntax: help <topic>.\nYou can also use help <index> to get a listing of help categories.");
            return false;
        }

    table = (HelpTable*)world->GetProperty("help");
    if (args.size() == 1)
        {
            if (!table->ShowEntry(args[0], mobile))
                {
                    mobile->Message(MSG_ERROR, "That topic does not exist.");
                    return false;
                }
        }
    else
        {
            std::string arg;
            arg = Explode(args);
            if (!table->ShowEntry(arg, mobile))
                {
                    mobile->Message(MSG_ERROR, "That topic does not exist.");
                    return false;
                }
        }

    return true;
}

CMDHedit::CMDHedit()
{
    SetName("hedit");
    AddAlias("hed");
    SetAccess(RANK_ADMIN);
}
BOOL CMDHedit::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    return true;
}
#endif

BOOL InitializeHelp()
{
#ifdef MODULE_HELP
    World* world = World::GetPtr();

    world->WriteLog("Initializing help.");
    HelpTable* table = new HelpTable();
    if (!table)
        {
            world->WriteLog("Error creating help table.", CRIT);
            return false;
        }
    world->AddProperty("help", (void*)table);
    //world->events.AddCallback("Shutdown", CleanupHelp);
    world->commands.AddCommand(new CMDHelp());
    world->commands.AddCommand(new CMDHedit());
#endif

    return true;
}

