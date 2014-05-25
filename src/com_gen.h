#ifndef COM_GEN_H
#include "mud.h"
#include "conf.h"
#include "command.h"

//command initialization
void InitializeGenCommands();

//Command declarations
class CMDQuit:public Command
{
public:
    CMDQuit();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDSave:public Command
{
public:
    CMDSave();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDBackup:public Command
{
public:
    CMDBackup();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDWho:public Command
{
public:
    CMDWho();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDToggle:public Command
{
public:
    CMDToggle();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDScore:public Command
{
public:
    CMDScore();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDChan:public Command
{
public:
    CMDChan();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDCommands:public Command
{
public:
    CMDCommands();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDHist:public Command
{
public:
    CMDHist();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDUptime:public Command
{
public:
    CMDUptime();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDWhois:public Command
{
public:
    CMDWhois();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDLook:public Command
{
public:
    CMDLook();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDCoord:public Command
{
public:
    CMDCoord();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDSuicide:public Command
{
public:
    CMDSuicide();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
    void Confirm(Socket* sock, BOOL choice);
};

class CMDSay:public Command
{
public:
    CMDSay();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDEmote:public Command
{
public:
    CMDEmote();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDPrompt:public Command
{
public:
    CMDPrompt();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDSockstats:public Command
{
public:
    CMDSockstats();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd);
};

class CMDExits:public Command
{
public:
    CMDExits();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd);
};

class CMDExperience:public Command
{
public:
    CMDExperience();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd);
};
#endif
