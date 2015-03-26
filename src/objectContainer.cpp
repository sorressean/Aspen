#include <list>
#include "mud.h"
#include "conf.h"
#include "baseObject.h"
#include "objectContainer.h"
#include "entity.h"

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

void ObjectContainer::Serialize(TiXmlElement* root)
{
    TiXmlElement* ent = new TiXmlElement("objc");
    BaseObject::Serialize(ent);

    TiXmlElement* contents = new TiXmlElement("contents");
    for (auto it: _contents)
        {
            it->Serialize(ent);
        }
    ent->LinkEndChild(contents);
    root->LinkEndChild(ent);
}
void ObjectContainer::Deserialize(TiXmlElement* root)
{
    TiXmlElement* contents = NULL;
    TiXmlNode* node = NULL;
    TiXmlElement* obj = NULL;

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

    BaseObject::Deserialize(root->FirstChild("BaseObject")->ToElement());
}
