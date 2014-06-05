#include <boost/lexical_cast.hpp>
#include <string>
#include <sstream>
#include <iomanip>
#include "mud.h"
#include "conf.h"
#include "olc.h"
#include "baseObject.h"
#include "world.h"
#include "socket.h"
#include "editor.h"
#include "eventargs.h"
#include "entity.h"
#include "staticObject.h"
#include "room.h"
#include "npc.h"
#include "olcs.h"
#include "olcGroup.h"
#include "olcManager.h"

IOlcEntry::    IOlcEntry(const std::string &name, const std::string &help, FLAG flag, OLCDT dt)
{
    _flag = flag;
    _dt = dt;
    _name = name;
    _help = help;
}
IOlcEntry::~IOlcEntry()
{
}

std::string IOlcEntry::GetName() const
{
    return _name;
}
void IOlcEntry::SetName(const std::string& v)
{
    _name = v;
}
std::string IOlcEntry::GetHelp() const
{
    return _help;
}
void IOlcEntry::SetHelp(const std::string& v)
{
    _help = v;
}
FLAG IOlcEntry::GetFlag() const
{
    return _flag;
}
void IOlcEntry::SetFlag(FLAG v)
{
    _flag = v;
}
OLCDT IOlcEntry::GetInputType() const
{
    return _dt;
}
void IOlcEntry::SetInputType(OLCDT v)
{
    _dt = v;
}

std::string OlcTypeToString(OLCDT dt)
{
    switch(dt)
        {
        case OLCDT::INTEGER:
            return "int";
        case OLCDT::STRING:
            return "str";
        case OLCDT::DECIMAL:
            return "dec";
        case OLCDT::BOOLEAN:
            return "bool";
        case OLCDT::FLG:
            return "flag";
        case OLCDT::EDITOR:
            return "edit";
        case OLCDT::ENUM:
            return "enum";
        default:
            return "inv";
        }
}

void ShowGroup(Player* mobile, OlcGroup* group)
{
    std::stringstream st;
    std::vector<IOlcEntry*>* entries = new std::vector<IOlcEntry*>();

    st << Repete('-', 80)  << std::endl;
    group->ListEntries(entries);
    for (IOlcEntry* entry: *entries)
        {
            st << std::left << std::setw(13) << entry->GetName();
            st << std::left << std::setw(6) << "<" +OlcTypeToString(entry->GetInputType())+">";
            st << std::right << entry->GetHelp() << std::endl;
        }
    mobile->Message(MSG_LIST, st.str());
}

bool ParseVnum(Player* mobile, std::string& arg, VNUM & num, std::string& comp, bool inroom)
{
    num = 0;
    comp.clear();
    size_t dotpos = 0;
    std::string numpart;

//check to see if there was a component attached.
//if not, the vnum shouldn't contain a '.'
    dotpos = arg.find('.');
    if (dotpos == std::string::npos) //no component found
        {
//check if we're parsing a room vnum and whether or not "here" was used.
            if (inroom && arg == "here")
                {
                    num = mobile->GetLocation()->GetOnum();
                    return true;
                }
//this has to be a vnum:
            try
                {
                    num = (VNUM)boost::lexical_cast<VNUM>(arg);
                    return true;
                }
            catch (boost::bad_lexical_cast e)
                {
                    return false;
                }
        }

//there was a component attached.
//we split it up first:
    numpart = arg.substr(0, dotpos);
    comp = arg.erase(0, dotpos + 1);
//check for "here" and room:
    if (inroom && numpart == "here")
        {
            num = mobile->GetLocation()->GetOnum();
            return true;
        }
//no here, convert the vnum:
    try
        {
            num = boost::lexical_cast<VNUM>(arg.substr(0, dotpos));
            return true;
        }
    catch (boost::bad_lexical_cast e)
        {
            return false;
        }

    return true;
}

BOOL InitializeOlc()
{
    World* world = World::GetPtr();
    return true;
}
