/*
*world
*This class is responsible for managing users, zones, mobs and coordenating the mud world
*All of the core functions will be kept within this class, and tasks delegated to other classes as needed for organizational purposes.
*/
#pragma once
#ifndef WORLD_H
#define WORLD_H
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include "mud.h"
#include "conf.h"
#include "server.h"
#include "log.h"
#include "command.h"
#include "channel.h"
#include "event.h"
#include "eventManager.h"
#include "player.h"
#include "npc.h"
#include "component.h"
#include "componentMeta.h"
#include "ComponentFactory.h"
#include "serializer.h"
#include "room.h"
#include "entity.h"
#include "staticObject.h"
#include "npc.h"
#include "zone.h"
#include "olcManager.h"
#include "optionManager.h"

typedef std::function<std::string (Player*)> PROMPTCB;

class World
{
    static World* _ptr;
    Server* _server; //our main server object.
    ComponentFactory* _cfactory;
    std::list <Player*> *_users; //a list of the currently connected players.
    std::unordered_map<char, PROMPTCB> *_prompts;
    std::map <int,Channel*>* _channels;
    std::map<std::string, Log*> *_logs;
    std::map <std::string,void*> *_properties;
    std::map<std::string, ISerializable*> *_state;
    std::vector<Zone*> *_zones;
    std::unordered_map<VNUM ,Room*> *_rooms;
    std::unordered_map<VNUM, StaticObject*> *_objects;
    std::unordered_map<VNUM, Npc*> *_npcs;
    OlcManager* _olcs;
    int _chanid;
    unsigned long long int _updates;
    unsigned long long int _totalUpdateTime;
    unsigned long long int _totalSleepTime;
    time_t _ruptime; //real uptime.
    time_t _cuptime; //uptime since last copyover.
    BOOL _running;
//mud text buffers
    char* _motd;
    char* _banner;
    OptionManager* _options;
public:
    static World* GetPtr();
//these are objects we need to store on each entity.
    EventManager events;
    Commandable commands;

    World();
    ~World();
    /*
    *Initializes the server object.
    */
    void InitializeServer();
    /*
    *Shuts down the mud: saves open objects, sends messages to players, etc.
    */
    void Shutdown();
    /*
    *Performs a copyover.
    *Param: [in] the person who initiated the copyover.
    */
    void Copyover(Player* mobile);
    /*
    *Returns: a pointer to the main server object.
    */
    Server* GetServer() const;
    OlcManager* GetOlcManager() const;
    /*
    *Returns a pointer to the component factory object.
    */
    ComponentFactory* GetComponentFactory();
    /*
    *Returns: a pointer to a list of pointers to player objects.
    */
    std::list <Player*> *GetPlayers() const;
    /*
    *Adds the player to the players list.
    *Only connected players should be added. Inactive players shouldn't be in this list.
    *Param: [in] a pointer to the player object to add.
    */
    BOOL AddPlayer(Player* player);
    /*
    *Removes the specified player from the list of active players.
    *Param: [in] A pointer to the player object
    */
    BOOL RemovePlayer(Player* player);
    /*
    *Locates the specified player:
    *Param: [in] The name of the player.
    *Return: A pointer to the player's object, NULL if the player wasn't found.
    */
    Player* FindPlayer(const std::string &name) const;
    /*
    *Loads the specified player and returns a pointer to the object.
    *Param: [in] the name of the player to load.
    *Return: a pointer to the player's object, if the player could be loaded.
    */
    Player* LoadPlayer(const std::string &name) const;
    /*
    *Adds a channel to the worlds channel lookup table.
    *Param: [in] A pointer to the object to add.
    */
    BOOL AddChannel(Channel* chan,BOOL command=true);
    /*
    *Will locate a channel object based on it's ID.
    *Param: [in] the id of the channel.
    *Return: a pointer to the channel object if it was found, NULLotherwise.
    */
    Channel* FindChannel(const int id) const;
    /*
    *Will locate a channel object based on it's name.
    *Param: [in] The name of the channel to locate.
        *Return: a pointer to the channel object if it was found, NULLotherwise.
    */
    Channel* FindChannel(const std::string &name);
    /*
    *Returns a list of all channel names, used to show channels and to allow for easily getting access to their pointers.
    *Param: [out] A pointer to a list of strings that will receive the chanel names.
    */
    void GetChannelNames(std::list <std::string>* out);
    /*
    *Checks to see if the specified channel exists.
    *Param: [in] a pointer to the channel.
    *Return: True if the channel exists, false otherwise.
    */
    BOOL ChannelExists(Channel* chan);
    /*
    *Registers the specified component
    *Params: [in] The name of the component.
    *[in] The component registration callback.
    *[in] the component init callback.
    *Return: True if the component was registered, false otherwise.
    */
    BOOL RegisterComponent(ComponentMeta* meta);
    /*
    *Creates the specified component
    *Parameters: [in] the name of the component to create.
    *Return: The object created.
    */
    Component*  CreateComponent(const std::string &name);
    /**
    *Gets the real uptime.
    *Return: the time the mud was last rebooted.
    */
    time_t GetRealUptime() const;
    /*
    *Sets the real uptime
    *Param: [in] the time of the last hard reboot.
    */
    void SetRealUptime(time_t tm);
    /*
    *gets the time of the last copyover.
    *Return: time of last copyover.
    */
    time_t GetCopyoverUptime() const;
    /*
    *Sets the time of the last copyover.
    *Param: [in] the time of last copyover.
    */
    void SetCopyoverUptime(time_t tm);
    /*
    *Adds a property to the world's property table.
    *Param: [in] the name of the property to add.
    *[in] a pointer to the object.
    *Return: true on success, false if the property couldn't be added (null pointer provided, or the property already exists).
    */
    BOOL AddProperty(const std::string &name,void* ptr);
    /*
    *Retrieves the property from the world's property table.
    *Param: [in] the name of the property.
    *Return: a pointer to the object, NULL if the property wasn't found.
    */
    void* GetProperty(const std::string &name) const;
    /*
    *Removes the property from the property table.
    *Param: [in] the name of the property to remove.
    *Return: true on success, false otherwise.
    *Note: this will not call delete on the object associated with that property.
    */
    BOOL RemoveProperty(const std::string &name);
    /*
    *Loads files into memory such as the MOTD so we won't have to read them every time we need them.
    */
    BOOL InitializeFiles();
    /*
    *The following functions return pointers to memory where the loaded files can be found.
    */
    const char* GetBanner() const;
    const char* GetMotd() const;
    /*
    *Updates the world.
    *Polls sockets, updates events, and keeps the world clean with tasks that should be ran every game loop.
    *Keep this function as minimal as possible; everything that occurs here runs every game loop iteration.
    */
    void Update();
    /*
    *Will perform a command for the specified player.
    *Params: [in] a pointer to the player object.
    *[in] A string with the arguments passed to the command.
    *Returns: true on success, false on failure.
    */
    BOOL DoCommand(Player* mobile, std::string args);

    /*
    *Matching methods.
    */

    Entity* MatchKeyword(const std::string &name, Player* caller);
    /*
    *Finds an object from the input.
    *Param: [in] the name of the object.
    *Return: A pointer to the object, NULL if it wasn't found.
    */

    Entity* MatchObject(const std::string &name,Player* caller);
    /*
    *Matches an object in a list.
    *Param: [in] the name of the object to find.
    *param: [in] a pointer to the list of objects to match against.
    *Return: An std::pair containing the return code and a pointer to the object.
    */
    Entity* MatchObjectInList(const std::string &name, std::list<Entity*> *olist);
    /*
    *Matches an object in a vector.
    *Param: [in] the name of the object to find.
    *param: [in] a pointer to the vector of objects to match against.
    *Return: An std::pair containing the return code and a pointer to the object.
    */
    Entity* MatchObjectInVector(const std::string &name, std::vector<Entity*> *olist);
    /*
    *Adds a zone to the list of zones.
    *Param: [in] a pointer to the zone to add.
    *Return: True on success, false on failure.
    */
    BOOL AddZone(Zone* zone);
    /*
    *Removes the zone from the list.
    *Param: [in] a pointer to the object to remove.
    *Return: True on success, false on failure.
    */
    BOOL RemoveZone(Zone* zone);
    /*
    *Retrieves a pointer to the specified zone.
    *Param: [in] The name of the zone object.
    *Return: A pointer to the object, NULLif it wasn't found.
    */
    Zone* GetZone(const std::string &name);
    /*
    *Populates a vector with pointers to the currently loaded zones.
    *Param: [in] a pointer to the vector to populate.
    *Return: True on success, false on failure.
    */
    BOOL GetZones(std::vector<Zone*> *zones);

    /*
    *Tries to create a copy of an Entity from a virtual object.
    *Param: [in] the vnum of the object to clone.
    *Return: A pointer to the new object on success, NULL on failure.
    */
    Entity* CreateObject(VNUM obj);
    /*
    *Recycles the specified  object, as well as removes it from the virtual obj statistics.
    *Also cleans up the object and the contents of that object.
    */
    BOOL RecycleObject(Entity* obj);
    /*
    *Adds the specified virtual object to the list.
    *Param: [in] a pointer to the static object to add.
    *Return: True on success, false on failure.
    */
    BOOL AddVirtual(StaticObject* obj);
    /*
    *Checks to see if the specified virtual object exists.
    *Param: [in] the  num of the virtual object to check.
    *Return: True if the object exists, false otherwise.
    */
    BOOL VirtualExists(VNUM num);
    /*
    *Retrieves a pointer to the specified virtual object.
    *Param: [in] the vnum of the object.
    *Return: A pointer to the object, NULL if it was not found.
    */
    StaticObject* GetVirtual(VNUM num);
    /*
    *Removes the specified virtual, as well as deletes each individual object that belongs to it.
    *Param: [in] the vnum of the object to remove.
    *Return: True on success, false on failure.
    */
    BOOL RemoveVirtual(VNUM num);
    /*
    *Tries to add the specified room.
    *Param: [in] a pointer to the room to be added.
    *Return: True on success, false on failure.
    */
    BOOL AddRoom(Room* room);
    /*
    *Tries to remove the specified room.
    *Param: [in] the VNUM of the room to remove.
    *Return: True on success, false on failure.
    */
    BOOL RemoveRoom(VNUM num);
    /*
    *Checks to see if the specified room exists.
    *Param: [in] the VNUM of the room.
    *Return: True on success, false on failure.
    */
    BOOL RoomExists(VNUM num);
    /*
    *Tries to retrieve the specified room.
    *Param: [in] the vnum of the room.
    *Return: a pointer to the room object on success, NULL otherwise.
    */
    Room* GetRoom(VNUM num);
    BOOL AddNpc(Npc* mob);
    BOOL RemoveNpc(VNUM num);
    bool NpcExists(VNUM num);
    Npc*  GetNpc(VNUM num);
    /*
    *Checks to see if the log exists.
    *Param: [in] the name of the log.
    *Return: True if the log has been registered, false otherwise.
    */
    BOOL LogExists(const std::string &name);
    /*
    *Registers the logger if it does not already exist.
    *Param: [in] the name of the logger to register.
    *Return: True on success, false on failure.
    */
    BOOL RegisterLog(const std::string &path, const std::string &name);
    /*
    *Writes the specified data.
    *Param: [in] the messsage to write.
    *[in] [optional] the log file to write to.
    *note: The log file must have already been registered with RegisterLog
    *[in] the level of the message.
    */
    void WriteLog(const std::string &data, LOG_LEVEL l=INFORM, const std::string &name = EVENT_NAME);
    /*
    *Returns a pointer to the logger.
    *Param: [in] the name of the logger.
    *Return: A pointer to the specified logger, or NULL if it does not exist.
    */
    Log* GetLog(const std::string &name);
    OptionManager* GetOptionManager();
    /*
    *Checks to see if the game engine is still running.
    *Return: True if the game loop should continue, false otherwise.
    */
    BOOL IsRunning() const;
    /*
    *Sets whether the game loop should continue.
    *This will be caught each iteration.
    *Param: [in] A boolian value that controls the game loop.
    */
    void SetRunning(BOOL running);
    /*
    *Checks to see if the specified prompt handler exists.
    *Param: [in] the letter of the handler to check for.
    *return: True if the prompt exists, false otherwise.
    */
    BOOL PromptExists(char prompt);
    /*
    *Registers the prompt variable and handler.
    *Param: [in] the letter to register for the prompt.
    *Param: [in] a std::function that points to the callback to retrieve information.
    *Return: True if the prompt could be registered, false otherwise.
    */
    BOOL RegisterPrompt(char prompt, PROMPTCB callback);
    /*
    *Format's a player's prompt.
    *Param: [in] the string with formatting characters.
    *Param: [in] a pointer to the player object.
    *Return: the new prompt with formatting characters replaced.
    */
    std::string BuildPrompt(const std::string &prompt, Player* mobile);
    /*
    *Adds the state to the list of pre-existing states to be serialized.
    *Param: [in] the name of the state to add.
    *Param: [in] a pointer to an object which inherits ISerializable.
    *Return: True if the state could be added, false otherwise.
    */
    BOOL AddState(const std::string &name, ISerializable* s);
    /*
    *Removes the specified state by name.
    *Param: [in] the name of the state to remove.
    *Return: True if the state can be removed, false otherwise.
    */
    BOOL RemoveState(const std::string &name);
    /*
    *Checks to see if the specified state exists.
    *Param: [in] the name of the state.
    *Return: True if the state exists, false otherwise.
    */
    BOOL StateExists(const std::string &name);
    /*
    *Saves and loads the world state, respectively.
    *Return: True on success, false on failure.
    */
    BOOL SaveState();
    BOOL LoadState();
    unsigned long long int GetUpdates() const;
    unsigned long long int GetUpdateTime() const;
    unsigned long long int GetSleepTime() const;
};
#endif
