/*
*The main living class
*Holds functions needed for both players and mobs.
*/
#pragma once
#ifndef LIVING_H
#define LIVING_H
#include <queue>
#include <vector>
#include <tinyxml.h>
#include "mud.h"
#include "conf.h"
#include "event.h"
#include "entity.h"
#include "attribute.h"
#include "affect.h"

struct _aff_comp
{
    bool operator()(Affect* a, Affect* b)
    {
        return ((a->GetDuration() > b->GetDuration()) ? true : false);
    }
};

enum class Gender
{
    Male=1,
    Female
};

class Living:public Entity
{
    int _level;
    POSITION _position;
    Gender _gender;
    std::vector<Attribute*> _attributes;
    std::priority_queue<Affect*, std::vector<Affect*>, _aff_comp> _taffects;
    std::vector<Affect*> _paffects;
public:
    Living();
    virtual ~Living();
    /*
    *This is called when an object enters or leaves the game environment
    */
    virtual void EnterGame();
    virtual void LeaveGame();
    /*
    *This is updated on a set interval to keep the objects up-to-date.
    *This includes heartbeat, removing uneeded objects, etc.
    */
    virtual void Update();

    /*
    *Find out what type of living this is
    *Return: False
    *Note: Should overwrite this for player and NPC objects to return true for each object.
    */
    virtual BOOL IsLiving() const;
    /*getters and setters*/
    Gender GetGender() const;
    void SetGender(Gender gender);
    int GetLevel() const;
    void SetLevel(int level);
    bool AddAttribute(Attribute* attr);
    void FindAttribute(AttributeApplication apply, int id, std::vector<Attribute*> &results);
    void FindAttribute(AttributeType type, std::vector<Attribute*>& results);
    bool RemoveAttribute(AttributeType type, int id);
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
};
#endif
