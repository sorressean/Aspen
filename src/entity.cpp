#include <tinyxml2.h>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include "entity.h"
#include "world.h"
#include "eventargs.h"
#include "event.h"
#include "eventManager.h"
#include "utils.h"
#include "command.h"
#include "olc.h"
#include "editor.h"
#include "objectManager.h"
#include "room.h"
#ifdef MODULE_SCRIPTING
#include "scripts/script.h"
#endif

Entity::Entity():
    _location(nullptr), _parent(nullptr)
{
    events.RegisterEvent("PostLook");
    events.RegisterEvent("PreLook");
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

bool Entity::MoveTo(ObjectContainer* targ)
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

bool Entity::FromRoom()
{
    Room* loc = (Room*)_location;
    if (!_location || !_location->IsRoom())
        {
            return false;
        }
    loc->ObjectLeave(this);
    loc->events.CallEvent("OnExit", nullptr, (void*)this);

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
bool Entity::AddAlias(const std::string &alias)
{
    if (AliasExists(alias) && !alias.empty())
        {
            return false;
        }

    _aliases.push_back(alias);
    return true;
}
bool Entity::AliasExists(const std::string & name)
{
    for (auto it: _aliases)
        {
            if (it == name)
                {
                    return true;
                }
        }

    return false;
}
std::vector<std::string>* Entity::GetAliases()
{
    return &_aliases;
}

std::string Entity::Identify(Player* mob)
{
    std::stringstream st;
    st << BaseObject::Identify(mob);
    st << "UUID: " << _uuid.ToString() << std::endl;
    return st.str();
}

bool Entity::IsObject() const
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

void Entity::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* ent = doc->NewElement("entity");
    ObjectContainer::Serialize(ent);
    _uuid.Serialize(ent);

    ent->SetAttribute("location", (_location?_location->GetOnum():0));
    ent->SetAttribute("short", _short.c_str());

    SerializeCollection<std::vector<std::string>, std::string>("aliases", "alias", ent, _aliases, [](tinyxml2::XMLElement* aelement, const std::string &alias)
    {
        aelement->SetAttribute("name", alias.c_str());
    });
    root->InsertEndChild(ent);
}
void Entity::Deserialize(tinyxml2::XMLElement* root)
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    int loc = 0;

    loc = root->IntAttribute("location");
    _short = root->Attribute("short");

    if (!loc)
        {
            _location=nullptr;
        }
    else
        {
            _location=omanager->GetRoom(loc);
        }

//and now we notify everything that an object was loaded:
    world->events.CallEvent("ObjectLoaded", nullptr, this);

    _uuid.Deserialize(root);
    DeserializeCollection(root, "aliases", [this](tinyxml2::XMLElement* visitor)
    {
        AddAlias(visitor->Attribute("name"));
    });

    ObjectContainer::Deserialize(root->FirstChildElement("objc"));
#ifdef MODULE_SCRIPTING
    /*
        Script* script = (Script*)world->GetProperty("script");
        script->Execute(this, GetScript());
    */
#endif
}
