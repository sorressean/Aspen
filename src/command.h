/**
* Contains: Command
*
* Commands enable players to complete specific tasks.
* For example, a command defined with the name of "north"might move
* the player north one room.
* @todo move commandable.
*/
#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>
#include <list>
#include <functional>
#include "mud.h"
#include "conf.h"

/**
* Defines the type of an individual command.
* mostly used for filtering and soarting.
*/
enum class CommandType
{
    Misc, Builder, Admin, God, Information, Object, Movement, Script, Social, Channel, Communication, All
};

/**
* Command hook callback.
*/
typedef std::function<bool (const std::string &, Player* ,std::vector<std::string> &, int)> CHOOK_CB;

/**
* This structure holds data for individual command hooks.
*
* The priority is what is used to soart the commands.
* For example, if you want your stats section to show up in score before your questpoints section
* you would give the stats a priority of 1, and maybe 10 for questpoints etc.
*/
struct CommandHook
{
    int id; //ID for removing hooks, if needed.
    int priority;
    CHOOK_CB cb;
};

/**
* The command class is what individual commands should inherit.
*
* Defines all methods which commands should use or override.
*/
class Command
{
protected:
    std::string _name;
    std::vector<std::string> _aliases;
    std::list<CommandHook*> _prehook;
    std::list<CommandHook*> _posthook;
    int _subcmd;
    CommandType _type;
    FLAG _access;
    unsigned int _position;

    /**
    * Adds a command hook.
    *
    * @param [in] hook the hook to add.
    * @param [in] _hooks the list to add the hook to.
    */
    bool AddHook(CommandHook* hook, std::list<CommandHook*>* _hooks);
    /**
    * Remove a specified hook, by ID.
    *
    * @param [in] id the ID of the hook to remove.
    * @param [in] _hooks the list to remove the hook from.
    */
    bool RemoveHook(int id, std::list<CommandHook*>* _hooks);
    /**
    * Executes the hooks as if they were commands.
    * The parameters for this method are the same as execute.
    * @sa Command::Execute
    */
    bool ExecuteHooks(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd, std::list<CommandHook*>* _hooks);
public:
    Command();
    virtual ~Command() = default;
    /**
    * Sets the name of the command.
    *
    * @param [in] name the name of the command.
    */
    void SetName(const std::string &name);
    /**
    * Gets the name of the command.
    */
    std::string GetName() const;
    /**
    * Adds an alias to the command.
    *
    * @param [in] name the alias to add.
    * @return true if the alias was added, false otherwise (the alias already existed).
    */
    BOOL AddAlias(const std::string &name);
    /**
    * Removes an alias from the command.
    *
    * @param [in] name the name of the alias.
    * @return true if the alias was removed, false otherwise.
    */
    BOOL RemoveAlias(const std::string &name);
    /**
    * Determines if the command has the specified alias.
    *
    * @param [in] name the name of the alias to search for.
    * @param [in] partialMatch whether or not the alias has to match entirely.
    * @todo could have better matching here.
    */
    BOOL HasAlias(const std::string &name, BOOL partialMatch = false);
    /**
    * Sets the subcommand.
    *
    * The subcommand is a method by which a single command class can be responsible for multiple commands.
    * For example, the command which moves n/s/e/w/etc is one command.
    * Without this, command logic would have to exist for each individual direction.
    * We still create objects per command (so we can set names), but it is not needed to reuse the execute or other logic.
    * The subcommand is simply passed into the execute method.
    * @param [in] subcmd the new subcommand.
    * @sa Command::Execute
    */
    void SetSubcmd(int subcmd);
    /**
    * Gets the subcommand associated with this particular instance.
    */
    int GetSubcmd() const;
    /**
    * Sets the type of the command.
    *
    * @param [in] type the new type of command.
    */
    void SetType(CommandType type);
    /**
    * Gets the current command type.
    */
    CommandType GetType() const;
    /**
    * Sets the required access to execute this command.
    *
    * @param [in] access the new access required for this command to execute.
    */
    void SetAccess(FLAG access);
    /**
    * Gets the current required access to execute the command.
    */
    FLAG GetAccess() const;
    /**
    * Sets the minimum position required to execute the command.
    *
    * This is needed in order to prevent a player from chopping down a tree while unconcious, for example.
    * @param [in] position the minimum position at which the command can execute.
    */
    void SetPosition(unsigned int position);
    /**
    * Retrieves the minimum position for this command to execute.
    */
    unsigned int GetPosition() const;
    /**
    * Executes the command.
    *
    * This is where most command logic will be placed.
    * @param [in] verb the string representation of the command that called this.
    * @param [in] mobile the calling player.
    * @param [in] arguments passed to the command.
    * @param [in] subcmd the subcommand this object is associated with (0 by default).
    * @return true if execution was successful, false otherwise.
    */
    virtual BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd) = 0;
    /**
    * Determine if the command is able to be executed.
    * @return true until overridden.
    */
    virtual BOOL CanExecute(Player* mobile, int subcmd);
    /**
    * Print command syntax.
    *
    * This enables for a command called syntax to exist.
    * This command will be able to get the syntax for any command (including itself).
    * @param [in] mobile the calling player.
    * @param [in] subcmd the subcommand associated with this object.
    * @todo make this an abstract method, forcing all commands to use it.
    */
    virtual void Syntax(Player* mobile, int subcmd) const;
    /**
    * Adds a command hook to the execution chain.
    *
    * @param [in] hook a pointer to the hook to be added.
    * @return true if the hook could be added, false otherwise.
    */
    bool AddPrehook(CommandHook* hook);
    /**
    * Removes a hook from the execution chain.
    *
    * @param [in] id the ID of the hook to remove.
    * @return true if the hook was removed, false otherwise.
    */
    bool RemovePrehook(int id);
    /**
    * Executes all command hooks associated with this chain.
    *
    * @sa Command::Execute
    */
    bool ExecutePrehooks(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
    /**
    * @sa Command::AddPrehook
    */
    bool AddPosthook(CommandHook* hook);
    /**
    * @sa Command::RemovePosthook
    */
    bool RemovePosthook(int id);
    /**
    * @sa Command::ExecutePrehooks
    */
    bool ExecutePosthooks(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/*
*Our commandable class
*Fascilitates adding and removing commands to an object.
*/
class Commandable
{
protected:
    std::vector <Command*> _commands;
public:
    Commandable();
    virtual ~Commandable();
    /*
    *Adds a command to the list of active commands:
    *Param: [in] A pointer to the command to add.
    */
    virtual BOOL AddCommand(Command* com);
    /*
    *Removes the command from the list.
    *Param: [in] the name of the command.
    *Return: true if the command was found and removed, false otherwise.
    */
    virtual BOOL RemoveCommand(const std::string &name);
    /*
    *Checks to see if the specified command exists.
    *Param: [in] the name of the command.
    *Return: True if the command exists, false otherwise.
    */
    virtual BOOL CommandExists(const std::string &name);
    /*
    *Retrieves the names of the commands the player has access to.
    *Param: [out] A pointer to a list of strings that the function can fill in.
    *[in] The player object.
    */
    virtual void ListCommands(std::vector <std::string>* list,Player* mobile, CommandType filter=CommandType::All);
    /*
    *Gets a pointer to the vector of commands.
    *return: a pointer to the vector of pointers to Command objects.
    */
    std::vector <Command*> *GetPtr();
};

/*
*Command initialization
*Param: [in] a pointer to the world object.
*/
BOOL InitializeCommands();
#endif
