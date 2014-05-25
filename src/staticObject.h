#pragma once
#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H
#include "conf.h"
#include "mud.h"
#include "baseObject.h"
#include "olc.h"
#include "componentMeta.h"
#ifdef MODULE_SCRIPTING
#include "scripts/scripts.h"
#endif

class Npc;
class StaticObject:public BaseObject
{
    unsigned int _totalCount;
    std::vector<Entity*> descendants;
    std::vector<ComponentMeta*> _components;
public:
    StaticObject();
    ~StaticObject();
    unsigned int GetTotalCount() const;
    unsigned int CountDescendants() const;
    BOOL IsDescendant(Entity* obj);
    BOOL RemoveDescendant(Entity* obj);
    Entity* Create();
    BOOL Recycle(Entity* obj);
    BOOL RecycleContents();
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
};

BOOL InitializeStaticObjectOlcs();
#endif
