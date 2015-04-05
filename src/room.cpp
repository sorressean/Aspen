#include <sstream>
#include <string>
#include <list>
#include <functional>
#include <tinyxml.h>
#include "room.h"
#include "living.h"
#include "exit.h"
#include "player.h"
#include "event.h"
#include "eventargs.h"
#include "zone.h"
#include "olc.h"
#include "olcManager.h"
#include "olcGroup.h"
#include "world.h"

Room::Room()
{
    _rflag = 0;
    SetOnum(ROOM_NOWHERE);
    SetPersistent(false);
    events.AddCallback("PostLook", std::bind(&Room::PostLook, this, std::placeholders::_1, std::placeholders::_2));
}
Room::~Room()
{
    for (auto it: _exits)
        {
            delete it;
        }
}

BOOL Room::AddExit(Exit* exit)
{
    if (exit==nullptr)
        {
            return false;
        }

    if (_exits.size())
        {
            for (auto it: _exits)
                {
                    if (exit == it)
                        {
                            return false;
                        }
                }
        }

    if (ExitExists(exit->GetDirection()))
        {
            return false;
        }

    _exits.push_back(exit);
    return true;
}

BOOL Room::ExitExists(ExitDirection dir)
{
    if (_exits.size())
        {
            for (auto it: _exits)
                {
                    if (it->GetDirection() == dir)
                        {
                            return true;
                        }
                }
        }

    return false;
}

Exit* Room::GetExit(ExitDirection dir)
{
    if (_exits.size())
        {
            for (auto it: _exits)
                {
                    if (it->GetDirection() == dir)
                        {
                            return it;
                        }
                }
        }

    return nullptr;
}

std::vector<Exit*>* Room::GetExits()
{
    return &_exits;
}

void Room::SetRoomFlag(FLAG flag)
{
    _rflag = flag;
}
FLAG Room::GetRoomFlag()
{
    return _rflag;
}

void Room::TellAll(const std::string &message)
{
    std::list<Living*>::iterator it, itEnd;

    itEnd = _mobiles.end();
    for (it = _mobiles.begin(); it != itEnd; ++it)
        {
            if ((*it)->IsPlayer())
                {
                    ((Player*)(*it))->Message(MSG_INFO,message);
                }
        }
}
void Room::TellAllBut(const std::string &message, std::list <Player*>* players)
{
    std::list<Player*> left;
    std::list<Player*>::iterator pit, pitEnd;
    std::list <Living*>::iterator lit, litEnd;

    BOOL found = false;

    pitEnd = players->end();
    litEnd = _mobiles.end();

    for (lit = _mobiles.begin(); lit != litEnd; ++lit)
        {
            for (pit = players->begin(); pit != pitEnd; ++pit)
                {
                    if ((*lit) == (*pit))
                        {
                            found = true;
                            break;
                        }
                }
            if (!found)
                {
                    left.push_back((Player*)(*lit));
                }
            found = false;
        }

    pitEnd = left.end();
    for (pit = left.begin(); pit != pitEnd; ++pit)
        {
            (*pit)->Message(MSG_INFO, message);
        }
}
void Room::TellAllBut(const std::string &message, Player* exclude)
{
    std::list<Living*>::iterator it, itEnd;

    itEnd = _mobiles.end();
    for (it = _mobiles.begin(); it != itEnd; ++it)
        {
            if ((*it)->IsPlayer() &&(*it) != exclude)
                {
                    ((Player*)(*it))->Message(MSG_INFO,message);
                }
        }
}

point* Room::GetCoord()
{
    return &_coord;
}
void Room::SetCoord(point& coord)
{
    _coord = coord;
}

BOOL Room::IsRoom() const
{
    return true;
}

void Room::Serialize(TiXmlElement* root)
{
    TiXmlElement* room = new TiXmlElement("room");
    TiXmlElement* exits = new TiXmlElement("exits");
    ObjectContainer::Serialize(room);

    room->SetAttribute("rflag", _rflag);
    room->SetAttribute("x", _coord.x);
    room->SetAttribute("y", _coord.y);
    room->SetAttribute("z", _coord.z);

    if (_exits.size())
        {
            for (auto it: _exits)
                {
                    it->Serialize(exits);
                }
        }
    room->LinkEndChild(exits);

    root->LinkEndChild(room);
}
void Room::Deserialize(TiXmlElement* room)
{
    TiXmlElement* exit = NULL;
    TiXmlElement* exits = NULL;
    TiXmlNode* node = NULL;
    Exit* ex = NULL;

    room->Attribute("rflag", &_rflag);
    room->Attribute("x", &_coord.x);
    room->Attribute("y", &_coord.y);
    room->Attribute("z", &_coord.z);
    exits = room->FirstChild("exits")->ToElement();
    for (node = exits->FirstChild(); node; node = node->NextSibling())
        {
            exit = node->ToElement();
            ex = new Exit();
            ex->Deserialize(exit);
            _exits.push_back(ex);
            ex = NULL;
        }

    ObjectContainer::Deserialize(room->FirstChild("objc")->ToElement());
}

void Room::ObjectEnter(Entity* obj)
{
    if (obj->IsLiving())
        {
            _mobiles.push_back((Living*)obj);
        }
    else
        {
            ObjectContainer::ObjectEnter(obj);
        }
}
void Room::ObjectLeave(Entity* obj)
{
    if (obj->IsLiving())
        {
            std::list<Living*>::iterator it, itEnd;
            itEnd = _mobiles.end();
            for (it = _mobiles.begin(); it != itEnd; ++it)
                {
                    if ((*it) == obj)
                        {
                            _mobiles.erase(it);
                            break;
                        }
                }
        }
    else
        {
            ObjectContainer::ObjectLeave(obj);
        }
}

//events
CEVENT(Room, PostLook)
{
    std::stringstream st;
    LookArgs* largs=(LookArgs*)args;

//we need to show everything in the room first:
    if (_mobiles.size() != 1)
        {
            std::list<Living*>::iterator it, itEnd;
            itEnd = _mobiles.end();

            for (it = _mobiles.begin(); it != itEnd; ++it)
                {
                    if ((*it) == largs->_caller)
                        {
                            continue;
                        }
                    largs->_desc += (*it)->GetShort()+"\r\n";
                }
        }
    if (_contents.size())
        {
            std::list<Entity*>::iterator it, itEnd;
            std::map<std::string, int> counts;
            std::map<std::string, int>::iterator mit, mitEnd;

//we need to try to combine the objects. First, we go through the list of everything and see how many of x there are.
//after that, we can add (x) foobars to the string.
//this is a slightly slow process...
            std::list<Entity*>::iterator cit, citEnd;
            citEnd = _contents.end();
            for (cit = _contents.begin(); cit != itEnd; ++cit)
                {
                    if (counts.count((*cit)->GetShort()))
                        {
                            counts[(*cit)->GetShort()]++;
                        }
                    else
                        {
                            counts[(*cit)->GetShort()] = 1;
                        }
                }

//now we iterate:
            mitEnd = counts.end();
            for (mit = counts.begin(); mit != mitEnd; ++mit)
                {
                    if ((*mit).second > 1)
                        {
                            st << "(" << (*mit).second << ") " << (*mit).first << "\r\n";
                        }
                    else
                        {
                            st << (*mit).first << "\r\n";
                        }
                }
            largs->_desc = st.str();
        }

    largs->_desc += TellObviousExits();
}
std::string Room::TellObviousExits()
{
    std::stringstream st;
    size_t count = 0;
    size_t i = 0;
    std::vector<Exit*> *exits = GetExits();

    if (!exits->size())
        {
            return "You see no obvious exits.";
        }
    else
        {
            st << "Obvious exits: [";
            count = exits->size();
            for (i = 0; i < count-1; i++)
                {
                    st << exits->at(i)->GetName() << ", ";
                }
            st << exits->at(exits->size()-1)->GetName();
            st << "].";
        }

    return st.str();
}

bool InitializeRoomOlcs()
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = new OlcGroup();
    group->SetInheritance(omanager->GetGroup(OLCGROUP::BaseObject));
    omanager->AddGroup(OLCGROUP::ROOM, group);
    return true;
}
