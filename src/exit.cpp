#include <tinyxml2.h>
#include <string>
#include "exit.h"
#include "living.h"

Exit::Exit(VNUM to):
    _to(to)
{
    _direction = nowhere;
}
Exit::Exit():
    _to(EXIT_NOWHERE)
{
}

VNUM Exit::GetTo() const
{
    return _to;
}
void Exit::SetTo(VNUM to)
{
    _to=to;
}

ExitDirection Exit::GetDirection() const
{
    return _direction;
}
void Exit::SetDirection(ExitDirection dir)
{
    _direction = dir;
}

std::string Exit::GetName() const
{
    switch(_direction)
        {
        case north:
            return "north";
        case south:
            return "south";
        case east:
            return "east";
        case west:
            return "west";
        case northwest:
            return "northwest";
        case northeast:
            return "northeast";
        case southwest:
            return "southwest";
        case southeast:
            return "southeast";
        default:
            return "unknown";
        }
}

bool Exit::CanEnter(Living* mobile)
{
    return true;
}

void Exit::Serialize(tinyxml2::XMLElement* root)
{
    tinyxml2::XMLDocument* doc = root->GetDocument();
    tinyxml2::XMLElement* node = doc->NewElement("exit");

    node->SetAttribute("direction", _direction);
    node->SetAttribute("to", _to);
    root->InsertEndChild(node);
}
void Exit::Deserialize(tinyxml2::XMLElement* node)
{
    _to = node->IntAttribute("to");
    _direction = (ExitDirection)node->IntAttribute("direction");
}
