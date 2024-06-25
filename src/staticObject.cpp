#include <tinyxml2.h>
#include <algorithm>
#include <functional>
#include "conf.h"
#include "mud.h"
#include "staticObject.h"
#include "baseObject.h"
#include "world.h"
#include "objectManager.h"
#ifdef OLC
#include "olc.h"
#include "olcGroup.h"
#include "olcManager.h"
#endif

StaticObject::StaticObject()
{
    _totalCount = 0;
}
StaticObject::~StaticObject()
{
}

std::string StaticObject::GetShort() const
{
    return _short;
}
void StaticObject::SetShort(const std::string &s)
{
    _short = s;
}
std::string StaticObject::GetPlural() const
{
    return _plural;
}
void StaticObject::SetPlural(const std::string &s)
{
    _plural = s;
}

unsigned int StaticObject::GetTotalCount() const
{
    return _totalCount;
}

unsigned int StaticObject::CountDescendants() const
{
    return (unsigned int)descendants.size();
}

bool StaticObject::IsDescendant(Entity* obj)
{
    std::vector<Entity*>::iterator it, itEnd;

    itEnd = descendants.end();
    for (it = descendants.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetUuid() == obj->GetUuid())
                {
                    return true;
                }
        }
    return false;
}
bool StaticObject::RemoveDescendant(Entity* obj)
{
    std::vector<Entity*>::iterator it, itEnd;

    itEnd = descendants.end();
    for (it = descendants.begin(); it != itEnd; ++it)
        {
            if ((*it)->GetUuid() == obj->GetUuid())
                {
                    descendants.erase(it);
                    return true;
                }
        }

    return false;
}

Entity* StaticObject::Create()
{
    Entity* obj = new Entity();
    obj->SetParent(this);
    obj->Initialize();
    Copy((BaseObject*)obj);
    _totalCount ++;
    descendants.push_back(obj);
    return obj;
}
bool StaticObject::Recycle(Entity* obj)
{
    if (obj == nullptr)
        {
            return false;
        }

    if (IsDescendant(obj))
        {
            _totalCount--;
            return RemoveDescendant(obj);
        }

    return false;
}

bool StaticObject::RecycleContents()
{
    World* world = World::GetPtr();
    ObjectManager* omanager = world->GetObjectManager();
    std::vector<Entity*>::iterator it, itEnd;

    if (descendants.size())
        {
            itEnd = descendants.end();
            for (it = descendants.begin(); it != itEnd; ++it)
                {
                    omanager->RecycleObject((*it));
                }
        }

    return true;
}

void StaticObject::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* obj = doc->NewElement("staticobj");

    obj->SetAttribute("short", _short.c_str());
    obj->SetAttribute("plural", _plural.c_str());

    SerializeList<IComponentMeta*, std::vector<IComponentMeta*>>("components", root, _components);
    BaseObject::Serialize(obj);
    root->InsertEndChild(obj);
}
void StaticObject::Deserialize(tinyxml2::XMLElement* root)
{
    World* world = World::GetPtr();

    const char* val_short = root->Attribute("short");
    if (val_short)
        _short = val_short;
    const char* plural = root->Attribute("plural");
    if (plural)
        _plural = plural;
    DeserializeCollection(root, "components", [world](tinyxml2::XMLElement* visitor)
    {
        IComponentMeta* com = nullptr;
        std::string cname;
        cname = visitor->Attribute("name");
        com = world->GetComponentFactory()->GetMeta(cname);
        com->Deserialize(visitor);
    });
}

//initialize olcs.
bool InitializeStaticObjectOlcs()
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* sgroup = new OlcGroup();

    sgroup->SetInheritance(omanager->GetGroup(OLCGROUP::BaseObject));
    sgroup->AddEntry(new OlcStringEntry<StaticObject>("short", "the title of the object seen in rooms", OF_NORMAL, OLCDT::STRING,
                     std::bind(&StaticObject::GetShort, std::placeholders::_1),
                     std::bind(&StaticObject::SetShort, std::placeholders::_1, std::placeholders::_2)));
    sgroup->AddEntry(new OlcStringEntry<StaticObject>("plural", "Sets the plural of the object", OF_NORMAL, OLCDT::STRING,
                     std::bind(&StaticObject::GetPlural, std::placeholders::_1),
                     std::bind(&StaticObject::SetPlural, std::placeholders::_1, std::placeholders::_2)));
    omanager->AddGroup(OLCGROUP::STATIC, sgroup);
    return true;
}
