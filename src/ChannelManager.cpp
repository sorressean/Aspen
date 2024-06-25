#include "mud.h"
#include "ChannelManager.h"

ChannelManager* ChannelManager::_instance = nullptr;

ChannelManager* ChannelManager::GetInstance()
{
    return _instance;
}

void ChannelManager::Initialize()
{
    if (!_instance)
        ChannelManager::_instance = new ChannelManager();
}
void ChannelManager::Release()
{
    if (ChannelManager::_instance)
        {
            delete ChannelManager::_instance;
            ChannelManager::_instance = nullptr;
        }
}
