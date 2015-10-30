#include <tinyxml.h>
#include "mud.h"
#include "living.h"
#include "event.h"
#include "delayedEvent.h"
#include "world.h"

Living::Living()
{
    events.RegisterEvent("HeartBeat", new DelayedEvent(LIVING_PULSE,0));

    _position = POSITION::standing;
    _gender = Gender::Neuter;
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

bool Living::AddAttribute(Attribute* attr)
{
    _attributes.push_back(attr);
    return true;
}
void Living::FindAttribute(int apply, int id, std::vector<Attribute*> &results)
{
    for (Attribute* attr: _attributes)
        {
            if (attr->GetApply() == apply && attr->GetId() == id)
                {
                    results.push_back(attr);
                }
        }
}
void Living::FindAttribute(int type, std::vector<Attribute*>& results)
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
    Entity::Serialize(node);
    root->LinkEndChild(node);
}
void Living::Deserialize(TiXmlElement* root)
{
    int temp = 0;

    root->Attribute("gender", &temp);
    _gender = (Gender)temp;
    Entity::Deserialize(root->FirstChild("entity")->ToElement());
}
