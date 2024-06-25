#include <tinyxml2.h>
#include "mud.h"
#include "living.h"
#include "event.h"
#include "delayedEvent.h"
#include "world.h"

Living::Living()
{
    events.RegisterEvent("HeartBeat");

    _position = POSITION_STANDING;
    _gender = Gender::Neuter;
    _following = nullptr;
}

void Living::EnterGame()
{
}
void Living::LeaveGame()
{
}

void Living::Update()
{
    events.CallEvent("HeartBeat", nullptr, (void*)this);
}

bool Living::IsLiving() const
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

unsigned int Living::GetPosition() const
{
    return _position;
}
void Living::SetPosition(unsigned int pos)
{
    _position = pos;
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

void Living::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* node = doc->NewElement("living");

    node->SetAttribute("gender", (int)_gender);
    Entity::Serialize(node);
    root->InsertEndChild(node);
}
void Living::Deserialize(tinyxml2::XMLElement* root)
{
    _gender = (Gender)root->IntAttribute("gender");
    Entity::Deserialize(root->FirstChildElement("entity"));
}
