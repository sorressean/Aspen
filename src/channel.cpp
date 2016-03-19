#include <sstream>
#include <string>
#include <list>
#include <functional>
#include "mud.h"
#include "conf.h"
#include "channel.h"
#include "player.h"
#include "world.h"
#include "utils.h"
#include "option.h"
#include "eventTable.h"

void InitializeChannels()
{
    World* world = World::GetPtr();
EventTable* events = EventTable::GetPtr();

    world->WriteLog("Initializing channels");
    events->AddCallback(GEVENT_PLAYER_CONNECT, std::bind(&Channel::SubscribeChannels, std::placeholders::_1, std::placeholders::_2));
    events->AddCallback(GEVENT_PLAYER_DISCONNECT, std::bind(&Channel::UnsubscribeChannels, std::placeholders::_1, std::placeholders::_2));

//Register channels here.
    world->AddChannel(new Channel("chat","ch",RANK_PLAYER));
}

CEVENT(Channel, SubscribeChannels)
{
    World* world = World::GetPtr();
    std::list <std::string> names;
    Channel* chan=nullptr;
    Player* mobile=(Player*)caller;

    mobile->events.AddCallback("OptionChanged", std::bind(&Channel::OptionChanged, std::placeholders::_1, std::placeholders::_2));
    world->GetChannelNames(&names);

    for (auto it: names)
        {
            if (GetOptionValue(it, mobile).GetInt() == 1)
                {
                    chan=world->FindChannel(it);
                    if (chan)
                        {
                            chan->AddListener(mobile,true);
                        }
                }
        }
}
CEVENT(Channel, UnsubscribeChannels)
{
    World* world = World::GetPtr();
    std::list <std::string> names;
    Channel* chan=nullptr;
    Player* mobile=(Player*)caller;

    world->GetChannelNames(&names);
    for (auto it: names)
        {
            chan=world->FindChannel(it);
            if (chan)
                {
                    chan->RemoveListener(mobile,true);
                }
        }
}
CEVENT(Channel, OptionChanged)
{
    World* world = World::GetPtr();
    Player* mobile = (Player*)caller;
    OptionChangedArgs* arg = (OptionChangedArgs*)args;
    Option* node = arg->opt;
    Channel* chan = world->FindChannel(node->GetMeta()->GetName());

    if (chan)
        {
            if (node->GetValue().GetInt())
                {
                    chan->AddListener(mobile);
                }
            else
                {
                    chan->RemoveListener(mobile);
                }
        }
}

Channel::Channel(const std::string &name,const std::string &alias,const FLAG access):
    _name(name), _alias(alias), _access(access)
{
    _pattern="[%N]: %P says, \'%M.\'";
}
Channel::~Channel()
{
    for (auto it: _history)
        {
            delete it;
        }
}

void Channel::_AddHistoryEntry(const std::string &message)
{
    HistoryNode* node=nullptr;

    if (message=="")
        {
            return;
        }
    if (_history.size()==MAX_CHAN_HISTORY_LENGTH)
        {
            node=_history.front();
            _history.pop_front();
            delete node;
            node = nullptr;
        }

    node=new HistoryNode();
    node->when=time(NULL);
    node->message=message;
    _history.push_back(node);
}

std::string Channel::_Patternize(const std::string &message,Player* caller)
{
    std::string m=message;
    std::stringstream format;
    std::string::iterator it, itEnd;
    BOOL emote=false;

    if (m[0]==':')
        {
            emote=true;
            m.erase(m.begin());
        }
    if (emote)
        {
            itEnd = _epattern.end();
        }
    else
        {
            itEnd = _pattern.end();
        }

    for (it=_pattern.begin(); it != itEnd; ++it)
        {
            if (((*it)=='%')&&((it++)!=(emote?_epattern.end():_pattern.end())))
                {
                    switch ((*it))
                        {
                        case 'p':
                            format << caller->GetName();
                            break;
                        case 'P':
                            format << Capitalize(caller->GetName());
                            break;
                        case 'n':
                            format << _name;
                            break;
                        case 'N':
                            format << Capitalize(_name);
                            break;
                        case 'm':
                            format <<m;
                            break;
                        case 'M':
                            format << Capitalize(m);
                            break;
                        case '%':
                            format << "%";
                            break;
                        default:
                            break;
                        }
                }
            else
                {
                    format << (*it);
                }
        }

    return format.str();
}

void Channel::SetName(const std::string &name)
{
    _name=name;
}
std::string Channel::GetName() const
{
    return _name;
}

void Channel::SetAlias(const std::string &alias)
{
    _alias=alias;
}
std::string Channel::GetAlias() const
{
    return _alias;
}

void Channel::SetPattern(const std::string &pattern)
{
    _pattern=pattern;
}
std::string Channel::GetPattern() const
{
    return _pattern;
}

void Channel::SetAccess(const FLAG access)
{
    _access=access;
}
FLAG Channel::GetAccess() const
{
    return _access;
}

std::list <HistoryNode*>* Channel::GetHistory()
{
    return &_history;
}

void Channel::AddListener(Player* subscriber,BOOL quiet)
{
    if (!subscriber->HasAccess(_access))
        {
            if (!quiet)
                {
                    subscriber->Message(MSG_ERROR,"You don't have seficient permissions to access this channel.");
                }
            return;
        }

    if (HasListener(subscriber))
        {
            if (!quiet)
                {
                    subscriber->Message(MSG_ERROR,"You are already subscribed to that channel.");
                }
            return;
        }

    _listeners.push_back(subscriber);
    if (!quiet)
        {
            subscriber->Message(MSG_INFO,"Tuned in.");
        }
}
void Channel::RemoveListener(Player* subscriber,BOOL quiet)
{
    if (HasListener(subscriber))
        {
            _listeners.remove(subscriber);
            if (!quiet)
                {
                    subscriber->Message(MSG_INFO,"Tuned out.");
                }
            return;
        }
    else
        {
            if (!quiet)
                {
                    subscriber->Message(MSG_INFO,"You are not tuned into that channel.");
                }
        }
}
BOOL Channel::HasListener(Player* mobile)
{
    for (auto it: _listeners)
        {
            if (it == mobile)
                {
                    return true;
                }
        }

    return false;
}

BOOL Channel::CanBroadcastWithoutListening(Player* mobile) const
{
    return false;
}

void Channel::Broadcast(Player* caller,const std::string &message,BOOL access)
{
    std::string paternized;

    if (!caller->HasAccess(_access))
        {
            caller->Message(MSG_ERROR,"You don't have the access needed to be able to broadcast to this channel.");
            return;
        }
    if (message=="")
        {
            caller->Message(MSG_ERROR,"You must provide a message.");
            return;
        }
    if (BitIsSet(caller->GetPflag(), PF_SILENCE))
        {
            caller->Message(MSG_ERROR, "You can not broadcast to channels while you are silenced.");
            return;
        }
    if (!HasListener(caller))
        {
            if (!CanBroadcastWithoutListening(caller))
                {
                    caller->Message(MSG_ERROR, "You must be tuned into that channel before you can broadcast on it.");
                    return;
                }
        }

    paternized=_Patternize(message,caller);

    for (auto it: _listeners)
        {
            it->Message(MSG_CHANNEL,paternized);
        }

    _AddHistoryEntry(paternized);
}
