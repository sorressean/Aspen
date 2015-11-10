#include <tinyxml2.h>
#include <list>
#include "mud.h"
#include "conf.h"
#include "baseObject.h"
#include "objectContainer.h"
#include "entity.h"
#include "serializationHelpers.h"

std::list<Entity*>* ObjectContainer::GetContents()
{
    return &_contents;
}

bool ObjectContainer::CanReceive(Entity* obj) const
{
    return true;
}
void ObjectContainer::ObjectLeave(Entity* obj)
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
void ObjectContainer::ObjectEnter(Entity* obj)
{
    _contents.push_back(obj);
}

void ObjectContainer::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->ToDocument();
    tinyxml2::XMLElement* ent = doc->NewElement("objc");
    BaseObject::Serialize(ent);

    SerializeList<Entity, std::list<Entity*>>("contents", root, _contents);
    root->InsertEndChild(ent);
}
void ObjectContainer::Deserialize(tinyxml2::XMLElement* root)
{
    DeserializeList<Entity, std::list<Entity*>>(root, "contents", _contents);
    for (auto it: _contents)
        {
            it->SetLocation(this);
        }

    BaseObject::Deserialize(root->FirstChildElement("BaseObject"));
}
