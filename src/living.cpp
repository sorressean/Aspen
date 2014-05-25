#include "living.h"
#include "event.h"
#include "delayedEvent.h"
#include "world.h"
#include <tinyxml.h>

Living::Living()
{
    events.RegisterEvent("HeartBeat", new DelayedEvent(LIVING_PULSE,0));
    _level = 1;
    _position = POSITION::standing;
    _gender = Gender::Female;
    World* world = World::GetPtr();
}
Living::~Living()
{
}

void Living::EnterGame()
{
}
void Living::LeaveGame()
{
}

void Living::Update()
{
    events.CallEvent("HeartBeat", NULL, (void*)this);
}
BOOL Living::IsLiving() const
{
    return true;
}

Gender Living::GetGender() const
{
    return _gender;
}
void Living::SetGender(Gender gender)
{
    _gender = gender;
}

int Living::GetLevel() const
{
    return _level;
}
void Living::SetLevel(int level)
{
    _level = level;
}

bool Living::AddAttribute(Attribute* attr)
{
    _attributes.push_back(attr);
    return true;
}
void Living::FindAttribute(AttributeApplication apply, int id, std::vector<Attribute*> &results)
{
    for (Attribute* attr: _attributes)
        {
            if (attr->GetApply() == apply && attr->GetId() == id)
                {
                    results.push_back(attr);
                }
        }
}
void Living::FindAttribute(AttributeType type, std::vector<Attribute*>& results)
{
    for (Attribute* attr:_attributes)
        {
            if (attr->GetType() == type)
                {
                    results.push_back(attr);
                }
        }
}

void Living::Serialize(TiXmlElement* root)
{
    TiXmlElement* node = new TiXmlElement("living");
    node->SetAttribute("gender", (int)_gender);
    node->SetAttribute("level", _level);
    Entity::Serialize(node);
    root->LinkEndChild(node);
}
void Living::Deserialize(TiXmlElement* root)
{
    int temp = 0;

    root->Attribute("gender", &temp);
    _gender = (Gender)temp;
    root->Attribute("level", &_level);
    Entity::Deserialize(root->FirstChild("entity")->ToElement());
}
