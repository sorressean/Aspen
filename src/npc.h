#pragma once
#ifndef NPC_H
#define NPC_H
#include "mud.h"
#include "conf.h"
#include "living.h"

class Room;
class Npc:public Living
{
    Room* _origin;
public:
    Npc();
    ~Npc();
    Room* GetOrigin() const;
    void SetOrigin(Room* o);
    BOOL IsNpc() const;
    virtual void Copy(BaseObject* obj) const;
};
#endif
