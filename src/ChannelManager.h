/**
* The Channel manager collects, updates and stores channels.
*/
#ifndef ASPEN_CHANNEL_MANAGER_H
#define ASPEN_CHANNEL_MANAGER_H

class ChannelManager
{
static ChannelManager* _instance;

ChannelManager() = default;
~ChannelManager() = default;
public:
static ChannelManager* GetInstance();
static void Initialize();
static void Release();
};
#endif
