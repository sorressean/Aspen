/**
* Contains: Channel
*
* Channels are responsible for enabling communication between players.
*/

#ifndef CHANNEL_H
#define CHANNEL_H
#include <list>
#include <string>
#include <ctime>
#include "mud.h"
#include "conf.h"
#include "event.h"
#include "player.h"

void InitializeChannels();

/**
* Channel history entry
*
* Each entry contains both a string and the time of the
*Message, which will be recorded in the history log.
*/
struct HistoryNode
{
    time_t when;
    std::string message;
};

/**
* Channels enable communication between players.
*
* Handles add and removal of users (listeners), and channel-specific patterns for broadcasting.
* Also handles an ACL-type interface for a minimum broadcast.
* Contains the information used for each channel (ID, history, listeners, access, and pattern.
* The only concept that must be explained is pattern.
* The pattern is what defines how messages are shown to players.
* Below is a list of modifiers; I have only used lowercase, but capitalizing the letter of the modifier will capitalize the first letter of it's
* translated value.
* %p: The player name.
* %m: The message.
* %n: The channel name.
*/
class Channel
{
protected:
    std::string _name;
    std::string _alias;
    std::string _pattern;
    std::string _epattern;
    FLAG _access;
    std::list <HistoryNode*> _history;
    std::list <Player*> _listeners;

    /**
    * Adds the history entry
    *
    * @param [in] message the message to add to the history.
    */
    void _AddHistoryEntry(const std::string &message);
    /**
    * Formats the specified message according to the pattern.
    *
    * @param [in] message the message to paternize.
    * @param [in] caller the player to use.
    * @return the new message, paternized.
    */
    std::string _Patternize(const std::string &message,Player* caller);
public:
    /**
    * Constructs a new channel.
    *
    * @param [in] name the name of the channel.
    * @param [in] an alias to be used for the channel.
    * @param [in] access Determines who can access this channel.
    */
    Channel(const std::string &name,const std::string &alias,const FLAG access);
    virtual ~Channel();

    /*
    *Getters and setters.
    *Exposes channel properties that are supposed to be visible to other objects.
    *Setters take the value to set, while getters return the value.
    */
    virtual void SetName(const std::string &name);
    virtual std::string GetName() const;
    virtual void SetAlias(const std::string &alias);
    virtual std::string GetAlias() const;
    virtual void SetPattern(const std::string &pattern);
    virtual std::string GetPattern() const;
    virtual void SetAccess(const FLAG access);
    virtual FLAG GetAccess() const;
    virtual std::list <HistoryNode*>* GetHistory();
    /*
    *Adds and removes a listener from the list.
    *Param: [in] The player to add to the list.
    *[in]: Determines whether or not the players add/remove is announced to the player. useful for logging in or adding mobs to channels.
    */
    virtual void AddListener(Player* subscriber,BOOL quiet = false);
    virtual void RemoveListener(Player* subscriber,BOOL quiet = false);
    /*
    *Checks to see if a listener exists.
    *Param: [in] a pointer to the player
    *Return: True if the specified listener exists, false otherwise.
    */
    virtual BOOL HasListener(Player* mobile);
    /*
    *Checks to see if a player can broadcast without listening to the channel.
    *Useful for channels where players events are being logged, etc.
    */
    virtual BOOL CanBroadcastWithoutListening(Player* mobile) const;
    /*
    *Sends a message to all those subscribed to the channel.
    *Param: [in] the player doing the broadcast.
    *[in] The message that will be broadcast.
    *[in] Whether or not to check the access rights of the player before broadcasting.
    *Those who don't have the access right of the channel can not broadcast by default.
    */
    virtual void Broadcast(Player* caller,const std::string &message,BOOL access=true);
    static EVENT(SubscribeChannels);
    static EVENT(UnsubscribeChannels);
    static EVENT(OptionChanged);
};
#endif /*CHANNEL_H*/
