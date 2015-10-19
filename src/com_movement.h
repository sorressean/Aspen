#ifndef COM_MOVEMENT_H
#define COM_MOVEMENT_H
#include "mud.h"
#include "conf.h"
#include "command.h"

void InitializeMovementCommands();

class CMDMove:public Command
{
public:
    CMDMove(int direction);
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
#endif
