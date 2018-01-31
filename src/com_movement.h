#ifndef COM_MOVEMENT_H
#define COM_MOVEMENT_H
#include "mud.h"
#include "conf.h"
#include "command.h"

/**
* Initializes commands used for movement.
* Mostly north/south/etc.
*/
void InitializeMovementCommands();

/**
* Contains logic for moving the player through specified directions.
* This command is mostly used via subcmd with the direction to prevent class duplication.
*/
class CMDMove:public Command
{
public:
    CMDMove(int direction);
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
#endif
