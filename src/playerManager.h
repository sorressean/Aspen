#pragma once
#include "mud.h"
#include "conf.h"
#include "living.h"
#include "player.h"

/**
This class manages all of the players online.
Also provides useful utilities such as broadcast.
*/
class PlayerManager
{
    std::list <Player*> _users; //a list of the currently connected players.
public:
    PlayerManager();
    ~PlayerManager();
    /**
    Returns: a pointer to a list of pointers to player objects.
    */
    std::list <Player*> *GetPlayers();
    /**
    Adds the player to the players list.
    Only connected players should be added. Inactive players shouldn't be in this list.
    \param: [in] a pointer to the player object to add.
    */
    bool AddPlayer(Player* player);
    /**
    Removes the specified player from the list of active players.
    \param: [in] A pointer to the player object
    */
    bool RemovePlayer(Player* player);
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

    /**
    Should only be called by world's internal workings.
    */
    void Shutdown();
    /**
    Should only be called by world's internal workings.
    */
    void Update();
};
