#ifndef COM_MOVEMENT_H
#define COM_MOVEMENT_H
#include "mud.h"
#include "conf.h"
#include "command.h"

void InitializeMovementCommands();
BOOL DoMove(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
class CMDNorth:public Command
{
public:
    CMDNorth();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDSouth:public Command
{
public:
    CMDSouth();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDEast:public Command
{
public:
    CMDEast();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDWest:public Command
{
public:
    CMDWest();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDNortheast:public Command
{
public:
    CMDNortheast();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDSoutheast:public Command
{
public:
    CMDSoutheast();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDSouthwest:public Command
{
public:
    CMDSouthwest();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDNorthwest:public Command
{
public:
    CMDNorthwest();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDUp:public Command
{
public:
    CMDUp();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDDown:public Command
{
public:
    CMDDown();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
#endif
