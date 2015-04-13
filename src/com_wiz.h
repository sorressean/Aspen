#ifndef COM_WIZ_H
#define COM_WIZ_H
#include "mud.h"
#include "conf.h"
#include "command.h"

//Command initialization
void InitializeWizCommands();

//Command declarations
class CMDCopyover:public Command
{
public:
    CMDCopyover();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDMkgod:public Command
{
public:
    CMDMkgod();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDMkbuilder:public Command
{
public:
    CMDMkbuilder();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDShutdown:public Command
{
public:
    CMDShutdown();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDBan:public Command
{
public:
    CMDBan();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDSilence:public Command
{
public:
    CMDSilence();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDUnsilence:public Command
{
public:
    CMDUnsilence();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDDisconnect:public Command
{
public:
    CMDDisconnect();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDEcho:public Command
{
public:
    CMDEcho();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDPdelete:public Command
{
public:
    CMDPdelete();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDSstatus:public Command
{
public:
    CMDSstatus();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDForce:public Command
{
public:
    CMDForce();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDPaste:public Command
{
public:
    CMDPaste();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
    void TextInput(Socket* sock, std::vector<std::string>* lines, void* args);
};
#endif
