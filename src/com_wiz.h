/**
* Contains wizard commands.
*/
#ifndef COM_WIZ_H
#define COM_WIZ_H
#include "mud.h"
#include "conf.h"
#include "command.h"

/**
* Initializes all commands, belonging to wizards.
*/
void InitializeWizCommands();

/**
* Performs a copyover.
*
* Copyover allows the mud to save state and player connections.
* This also allows the mud to reload and resume the game.
*/
class CMDCopyover:public Command
{
public:
    CMDCopyover();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Promotes the specified player to a wizard.
*/
class CMDMkwiz:public Command
{
public:
    CMDMkwiz();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Promotes the specified player to a builder.
*/
class CMDMkbuilder:public Command
{
public:
    CMDMkbuilder();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Terminates the mud.
*/
class CMDShutdown:public Command
{
public:
    CMDShutdown();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Adds, lists and manages addresses.
*
* @todo clean up command to allow for add/remove/etc.
*/
class CMDBan:public Command
{
public:
    CMDBan();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Removes the player's ability to speak on channels, etc.
*/
class CMDSilence:public Command
{
public:
    CMDSilence();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Unsilences specified player.
* @todo combine this with silence.
*/
class CMDUnsilence:public Command
{
public:
    CMDUnsilence();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Disconnects the specified player.
*/
class CMDDisconnect:public Command
{
public:
    CMDDisconnect();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Echos a message to the world.
*/
class CMDEcho:public Command
{
public:
    CMDEcho();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Deletes the specified player.
* @todo finish logic.
*/
class CMDPdelete:public Command
{
public:
    CMDPdelete();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Shows server status.
*/
class CMDSstatus:public Command
{
public:
    CMDSstatus();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Forces the specified player to execute a command.
*/
class CMDForce:public Command
{
public:
    CMDForce();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/**
* Allows for the pasting of a lot of text.
*
* Displays to the environment.
*/
class CMDPaste:public Command
{
public:
    CMDPaste();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
    void TextInput(Socket* sock, std::vector<std::string>* lines, void* args);
};

/**
* Retrieves stats on scripts.
* These are all internal stats.
*/
class CMDScriptStats:public Command
{
public:
    CMDScriptStats();
    bool Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
#endif
