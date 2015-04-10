#include "mud.h"
#include "conf.h"
#include "player.h"
#include "playerManager.h"
#include "socket.h"
#include "utils.h"

PlayerManager::PlayerManager()
{
}
PlayerManager::~PlayerManager()
{
    for (auto it: _users)
        {
            delete it;
        }
}

std::list <Player*> *PlayerManager::GetPlayers()
{
    return &_users;
}

BOOL PlayerManager::AddPlayer(Player* player)
{
    if (!player)
        {
            return false;
        }
    if (player->GetSocket()->GetConnectionType() != CON_Game)
        {
            return false;
        }

    _users.push_back(player);
    return true;
}
BOOL PlayerManager::RemovePlayer(Player* player)
{
    std::list<Player*>::iterator it, itEnd;

    itEnd = _users.end();
    for (it = _users.begin(); it != itEnd; ++it)
        {
            if ((*it) == player)
                {
                    _users.erase(it);
                    return true;
                }
        }

    return false;
}
Player* PlayerManager::FindPlayer(const std::string &name) const
{
    for (auto it: _users)
        {
            if (it->GetName()==name)
                {
                    return it;
                }
        }

    return nullptr;
}

Player* PlayerManager::LoadPlayer(const std::string &name) const
{
    if (PlayerExists(name))
        {
            Player* p=new Player();
            p->SetName(name);
            p->Load();
            return p;
        }

    return nullptr;
}

void PlayerManager::Shutdown()
{
    std::list<Player*>::iterator it, itEnd;

    itEnd = _users.end();
    for (it = _users.begin(); it != itEnd; ++it)
        {
            (*it)->Message(MSG_CRITICAL,"The mud is shutting down now. Your Character will be autosaved.");
            (*it)->Save(true);
            (*it)->GetSocket()->Kill();
            it = _users.begin();
        }
}
void PlayerManager::Update()
{
    if (_users.empty())
        {
            return;
        }

    for (auto pit: _users)
        {
            pit->Update();
        }
}
