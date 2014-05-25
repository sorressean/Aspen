/*
*The main scripts header.
*Initializes other script components, as well as contains utilities
*that all of the script components can use.
*/
#pragma once
#ifndef SCRIPTS_H
#define SCRIPTS_H
#include "../mud.h"
#include "../conf.h"
#include "../command.h"
#include "../player.h"
#include "../server.h"
#include "../socket.h"
#include "../variant.h"

#ifdef MODULE_SCRIPTING
extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/luaconf.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>
}

//Basic userdata class
//used to pass in the type of class we're working with.
enum UType {
    OBJECT, COMPONENT
};
struct UserData
{
    Entity* ptr;
    UType type;
};
struct ComponentData
{
    Component* com;
    UType type;
};

class Script
{
    static lua_State *state;
public:
    Script();
    ~Script();
    EVENT(ObjectDestroyed);
    void Execute(Entity* obj, const std::string &code);
    static lua_State* GetState();
};
EVENT(ScriptShutdown);

class CMDExecute:public Command
{
public:
    CMDExecute();
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

/*
*Auxiliary functions
*/
/*
*Helps with printing an error message, when a problem is encountered.
*Param: [in] the lua state that the error belongs to.
*Param: [in] the error message.
*/
void SCR_Error(lua_State* l, const char* msg);
/*
*converts a value on the stack to a variant.
*Param: [in] the lua state where the index will be pulled from.
*Param: [in] the index on the Lua stack where the variant should be converted from.
*Return: The variant with the value at the given index, or an empty Variant if there was a problem.
*/
Variant IndexToVariant(lua_State*l, int index);
/*
*Converts a given Variant to a value on the stack.
*Param: [in] the state of which the stack belongs that we will put the variant on.
*the variant we will push to the stack.
*Return: True on success, false on failure.
*/
BOOL VariantToStack(lua_State* l, Variant &var);
/*
*checks to see if the type of the udata is a player.
*Param: [in] the lua state for the operation.
*[in] the UserData object to check.
*Return: True if the object is a player, false otherwise.
*/
BOOL IsPlayer(lua_State* l, UserData* udata);
/*
*Checks to see if the provided object is living.
*Param: [in] the active lua state.
*[in] the UserData struct.
*Return: True if the object is living, false otherwise.
*/
BOOL IsLiving(lua_State* l, UserData* udata);
/*
*Checks to see if the specified object is a room.
*Param: [in] the active lua state.
*[in] the UserData struct.
*Return: True if the object is a room, false otherwise.
*/
BOOL IsRoom(lua_State* l, UserData* udata);
/*
*Checks to see if the specified object is of type Entity
*Param: [in] the currently active lua state.
*[in] The object to check.
*Return: True if the object is a valid entity, false otherwise.
*/
BOOL IsObject(lua_State* l, UserData* udata);
/*
*Checks to see if the user data is a component.
*Param: [in] the active lua state.
*[in] a pointer to the user data.
*Return: True if the object is a component, false otherwise.
*/
BOOL IsComponent(lua_State* l, UserData* udata);
/*
*Creates a UserData structure to hold an object and pushes it on the stack.
*Param: [in] the state that this userdata belongs to.
*[in]: A pointer to the object that you want in the userdata.
*Return: A pointer to the userdata object created.
*/
UserData* ObjectToStack(lua_State* l, Entity* obj);
/*
*Used as a drop-in replacement for print.
*/
int SCR_Print(lua_State* l);
std::string SCR_TypeToStr(lua_State* l, int index);
#endif

BOOL InitializeScript();
#endif
