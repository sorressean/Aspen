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

BOOL StaticObject::IsDescendant(Entity* obj)
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
BOOL StaticObject::RemoveDescendant(Entity* obj)
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
BOOL StaticObject::Recycle(Entity* obj)
{
    if (obj == NULL)
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

BOOL StaticObject::RecycleContents()
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

void StaticObject::Serialize(TiXmlElement* root)
{
    std::vector<IComponentMeta*>::iterator it, itEnd;
    TiXmlElement* obj = new TiXmlElement("staticobj");
    TiXmlElement* component = NULL;
    BaseObject::Serialize(obj);
    obj->SetAttribute("short", _short.c_str());
    obj->SetAttribute("plural", _plural.c_str());
    TiXmlElement* components = new TiXmlElement("components");
    itEnd = _components.end();
    for (it = _components.begin(); it != itEnd; ++it)
        {
            component = new TiXmlElement("component");
            (*it)->Serialize(component);
            components->LinkEndChild(component);
        }

    obj->LinkEndChild(components);
    root->LinkEndChild(obj);
}
void StaticObject::Deserialize(TiXmlElement* root)
{
    World* world = World::GetPtr();
    TiXmlNode* node = NULL;
    TiXmlElement* cmeta = NULL;
    TiXmlElement* components = NULL;
    IComponentMeta* com = NULL;
    std::string cname;

    _short = root->Attribute("short");
    _plural = root->Attribute("plural");
    node = root->FirstChild("components");
    if (node)
        {
            components = node->ToElement();
            for (node = components->FirstChild(); node; node = node->NextSibling())
                {
                    cmeta = node->ToElement();
                    cname = cmeta->Attribute("name");
                    com = world->GetComponentFactory()->GetMeta(cname);
                    com->Deserialize(cmeta);
                }
        }
}

//initialize olcs.
BOOL InitializeStaticObjectOlcs()
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
