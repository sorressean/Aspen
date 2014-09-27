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

Entity* Entity::GetLocation() const
{
    return _location;
}
void Entity::SetLocation(Entity* location)
{
    _location=location;
}

std::list<Entity*>* Entity::GetContents()
{
    return &_contents;
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
    BaseObject::Serialize(ent);
    Uuid::Serialize(ent);
    TiXmlElement* contents = new TiXmlElement("contents");

    std::list<Entity*>::iterator cit, citEnd;
    citEnd=_contents.end();

    if (_contents.size())
        {
            if (_persistent)
                {
                    for (cit = _contents.begin(); cit != citEnd; ++cit)
                        {
                            if (!(*cit)->IsLiving())
                                {
                                    (*cit)->Serialize(ent);
                                }
                        }
                }
        }
    ent->LinkEndChild(contents);

    ent->SetAttribute("location", (_location?_location->GetOnum():0));
    root->LinkEndChild(ent);
}
void Entity::Deserialize(TiXmlElement* root)
{
    TiXmlElement* contents = NULL;
    TiXmlNode* node = NULL;
    TiXmlElement* obj = NULL;
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    int loc;

//contents
    node = root->FirstChild("contents");
    if (node)
        {
            contents = node->ToElement();
            for (node = contents->FirstChild(); node; node = node->NextSibling())
                {
                    obj = node->ToElement();
                    Entity* object = new Entity();
                    object->Deserialize(obj);
                    object->SetLocation(this);
                    _contents.push_back(object);
                }
        }

    root->Attribute("location", &loc);
    Uuid::Deserialize(root);
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
    BaseObject::Deserialize(root->FirstChild("BaseObject")->ToElement());
#ifdef MODULE_SCRIPTING
    /*
        Script* script = (Script*)world->GetProperty("script");
        script->Execute(this, GetScript());
    */
#endif
}

std::string Entity::DoLook(Player* mobile)
{
    std::string str;

    LookArgs* args=new LookArgs(mobile,this,str);
    events.CallEvent("PreLook", args, (void*)mobile);
    str+=Capitalize(this->GetName())+"\n";
    str+=this->GetDescription()+"\n";
    events.CallEvent("PostLook", args, (void*)mobile);
    delete args;
    return str;
}

BOOL Entity::CanReceive(Entity* obj) const
{
    return true;
}

BOOL Entity::MoveTo(Entity* targ)
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
void Entity::ObjectEnter(Entity* obj)
{
    _contents.push_back(obj);
}
void Entity::ObjectLeave(Entity* obj)
{
    std::list<Entity*>::iterator it, itEnd;

    itEnd = _contents.end();
    for (it = _contents.begin(); it != itEnd; ++it)
        {
            if ((*it) == obj)
                {
                    it = _contents.erase(it);
                    break;
                }
        }
}

BOOL Entity::FromRoom()
{
    Room* loc = (Room*)_location;
    if (!_location || !_location->IsRoom())
        {
            return false;
        }
    ObjectLeave(loc);
    loc->events.CallEvent("OnExit", NULL, (void*)this);

    return true;
}
BOOL Entity::IsNpc() const
{
    return false;
}
BOOL Entity::IsPlayer() const
{
    return false;
}
BOOL Entity::IsLiving() const
{
    if (IsPlayer() || IsNpc())
        {
            return true;
        }

    return false;
}
BOOL Entity::IsRoom() const
{
    return false;
}

std::string Entity::Identify(Player* mob)
{
    std::stringstream st;
    st << BaseObject::Identify(mob);
    st << "UUID: " << GetUuid() << std::endl;
    return st.str();
}

bool InitializeEntityOlcs()
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = new OlcGroup();
    group->SetInheritance(omanager->GetGroup(OLCGROUP::BaseObject));
    omanager->AddGroup(OLCGROUP::BaseObject, group);
    return true;
}
