#ifndef COM_BUILDER_H
#define COM_BUILDER_H
#include "mud.h"
#include "conf.h"
#include "command.h"

//Command initialization
void InitializeBuilderCommands();

class CMDZlist:public Command
{
public:
  CMDZlist();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDRlist:public Command
{
public:
  CMDRlist();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDDig:public Command
{
public:
  CMDDig();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDAStats: public Command
{
public:
  CMDAStats();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
  std::string Stats(Player* mobile, Zone* area);
};

class CMDVCreate:public Command
{
public:
  CMDVCreate();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDVList:public Command
{
public:
  CMDVList();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDMCreate:public Command
{
public:
  CMDMCreate();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDMList:public Command
{
public:
  CMDMList();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDMLoad:public Command
{
public:
  CMDMLoad();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

class CMDAddComponent:public Command
{
public:
  CMDAddComponent();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};


class CMDGoto:public Command
{
public:
  CMDGoto();
  BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};


#endif
