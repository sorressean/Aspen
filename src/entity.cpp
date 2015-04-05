#include <string>
#include <list>
#include <map>
#include <sstream>
#include <tinyxml.h>
#include "entity.h"
#include "world.h"
#include "room.h"
#include "eventargs.h"
#include "event.h"
#include "eventManager.h"
#include "utils.h"
#include "command.h"
#include "olc.h"
#include "editor.h"
#include "objectManager.h"
#ifdef MODULE_SCRIPTING
#include "scripts/script.h"
#endif

Entity::Entity()
{
    _location=NULL;
    _parent = NULL;
    events.RegisterEvent("PostLook", new Event());
    events.RegisterEvent("PreLook",new Event());
}
Entity::~Entity()
{
}

std::string Entity::GetShort() const
{
    return _short;
}
void Entity::SetShort(const std::string &s)
{
    _short = s;
}


ObjectContainer* Entity::GetLocation() const
{
    return _location;
}
void Entity::SetLocation(ObjectContainer* location)
{
    _location=location;
}

StaticObject* Entity::GetParent() const
{
    return _parent;
}
void Entity::SetParent(StaticObject* parent)
{
    _parent = parent;
}

void Entity::Serialize(TiXmlElement* root)
{
    TiXmlElement* ent = new TiXmlElement("entity");
    ObjectContainer::Serialize(ent);
    _uuid.Serialize(ent);

    ent->SetAttribute("location", (_location?_location->GetOnum():0));
    ent->SetAttribute("short", _short.c_str());
    root->LinkEndChild(ent);
}
void Entity::Deserialize(TiXmlElement* root)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    int loc;

    root->Attribute("location", &loc);
    _uuid.Deserialize(root);
    _short = root->Attribute("short");
    if (!loc)
        {
            _location=NULL;
        }
    else
        {
            _location=omanager->GetRoom(loc);
        }

//and now we notify everything that an object was loaded:
    world->events.CallEvent("ObjectLoaded", NULL, this);
    ObjectContainer::Deserialize(root->FirstChild("objc")->ToElement());
#ifdef MODULE_SCRIPTING
    /*
        Script* script = (Script*)world->GetProperty("script");
        script->Execute(this, GetScript());
    */
#endif
}

BOOL Entity::MoveTo(ObjectContainer* targ)
{
    if (targ->CanReceive(this))
        {
            if (_location)
                {
                    _location->ObjectLeave(this);
                }
            _location=targ;
            targ->ObjectEnter(this);
            return true;
        }
    return false;
}
BOOL Entity::FromRoom()
{
    Room* loc = (Room*)_location;
    if (!_location || !_location->IsRoom())
        {
            return false;
        }
    loc->ObjectLeave(this);
    loc->events.CallEvent("OnExit", NULL, (void*)this);

    return true;
}

void Entity::Initialize()
{
    _uuid.Initialize();
}
Uuid& Entity::GetUuid()
{
    return _uuid;
}

std::string Entity::Identify(Player* mob)
{
    std::stringstream st;
    st << BaseObject::Identify(mob);
    st << "UUID: " << _uuid.ToString() << std::endl;
    return st.str();
}

BOOL Entity::IsObject() const
{
    return true;
}

bool InitializeEntityOlcs()
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = new OlcGroup();
    group->SetInheritance(omanager->GetGroup(OLCGROUP::BaseObject));
    group->AddEntry(new OlcStringEntry<Entity>("short", "the title of the object seen in rooms", OF_NORMAL, OLCDT::STRING,
                    std::bind(&Entity::GetShort, std::placeholders::_1),
                    std::bind(&Entity::SetShort, std::placeholders::_1, std::placeholders::_2)));

    omanager->AddGroup(OLCGROUP::Entity, group);
    return true;
}
