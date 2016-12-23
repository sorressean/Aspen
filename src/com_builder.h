/*
* Contains: builder commands.
*
* @todo Columnize output for commands like zlist and rlist.
*/
#ifndef COM_BUILDER_H
#define COM_BUILDER_H
#include "mud.h"
#include "conf.h"
#include "command.h"

//Command initialization
void InitializeBuilderCommands();

/**
* Displays a list of zones.
*/
class CMDZlist:public Command
{
public:
    CMDZlist();
    void Syntax(Player* mobile, int subcmd=0) const;
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Shows a list of all rooms belonging to a zone.
*/
class CMDRlist:public Command
{
public:
    CMDRlist();
    void Syntax(Player* mobile, int subcmd=0) const;
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/*
* Creates another room and adds an exit from the current room (and optionally back).
* @todo clean up code in dig, most particularly command parsing.
*/
class CMDDig:public Command
{
public:
    CMDDig();
    void Syntax(Player* mobile, int subcmd=0) const;
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Shows statistics for an area.
* @todo allow for area to be specified.
*/
class CMDAStats: public Command
{
public:
    CMDAStats();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
    std::string Stats(Player* mobile, Zone* area);
};

/**
* Creates a virtual object.
*/
class CMDVCreate:public Command
{
public:
    CMDVCreate();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Lists all virtuals that belong to a specified zone.
* @todo allow for the specification of a zone other than current.
* @todo allow for the columnization of data.
*/
class CMDVList:public Command
{
public:
    CMDVList();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Creates an NPC.
*/
class CMDMCreate:public Command
{
public:
    CMDMCreate();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Lists all NPCS in the zone.
* @todo allow for specification of a zone other than current.
* @todo allow for columnization of output.
*/
class CMDMList:public Command
{
public:
    CMDMList();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Clones a specified NPC.
* @todo determine what to do with NPC after it's been created.
* @todo Remove number parsing code to a utility.
*/
class CMDMLoad:public Command
{
public:
    CMDMLoad();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Adds the specified component to an object.
* @todo remove the number parsing code.
*/
class CMDAddComponent:public Command
{
public:
    CMDAddComponent();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Moves to the specified room.
*/
class CMDGoto:public Command
{
public:
    CMDGoto();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Creates the specified zone.
*/
class CMDZcreate:public Command
{
public:
    CMDZcreate();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Destroys the specified zone.
*/
class CMDZdestroy:public Command
{
public:
    CMDZdestroy();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Creates a room in the zone.
*/
class CMDRcreate :public Command
{
public:
    CMDRcreate();
    BOOL Execute(const std::string &verb, Player* mobile, std::vector<std::string> &args, int subcmd);
};
#endif
