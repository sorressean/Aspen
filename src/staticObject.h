#pragma once
#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H
#include "conf.h"
#include "mud.h"
#include "baseObject.h"
#include "olc.h"
#include "componentMeta.hpp"
class Npc;
class StaticObject:public BaseObject
{
    std::string _plural;
    std::string _short; //the description you see in a room.
    unsigned int _totalCount;
    std::vector<Entity*> descendants;
    std::vector<IComponentMeta*> _components;
public:
    StaticObject();
    ~StaticObject();
    virtual std::string GetShort() const;
    virtual void SetShort(const std::string &s);
    virtual std::string GetPlural() const;
    virtual void SetPlural(const std::string &s);
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
