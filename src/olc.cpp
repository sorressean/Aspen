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

CMDOlcSet::CMDOlcSet()
{
}
void CMDOlcSet::ShowGroup(Player* mobile, OlcGroup* group)
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
BOOL CMDOlcSet::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    World* world = World::GetPtr();
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = NULL;
    IOlcEntry* entry = NULL;
    VNUM vnum = 0;
    size_t dotpos = 0;
    std::string component;
    Component* ocomponent = NULL;
    ComponentMeta* meta = NULL;
    std::vector<std::string>::iterator it;
    std::string name;
    std::string value;

    union
    {
        Room* r;
        StaticObject* o;
        Npc* n;
    } o;
    o.o = NULL;

    if (args.size() == 0)
        {
            mobile->Message(MSG_ERROR, "Syntax: "+verb+" <vnum>[.component] [field] [value].");
            return false;
        }

//we see if there was a component attached to the object number.
    dotpos = args[0].find('.');
    if(dotpos == std::string::npos) //no component iidentifier found
        {
            if (subcmd == COS_ROOM && args[0]  == "here")
                {
                    vnum = mobile->GetLocation()->GetOnum();
                }
            else
                {
                    try
                        {
                            vnum = (VNUM)boost::lexical_cast<unsigned int>(args[0]);
                        }
                    catch (boost::bad_lexical_cast e)
                        {
                            mobile->Message(MSG_ERROR, "Invalid vnum.");
                            return false;
                        }
                }
        }
    else //there was a component attached.
        {
            if (subcmd == COS_ROOM && args[0].substr(0,dotpos) == "here")
                {
                    vnum = mobile->GetLocation()->GetOnum();
                }
            else
                {
                    try
                        {
                            vnum = boost::lexical_cast<unsigned int>(args[0].substr(0, dotpos));
                        }
                    catch (boost::bad_lexical_cast e)
                        {
                            mobile->Message(MSG_ERROR, "Invalid vnum.");
                            return false;
                        }
                }
            component = args[0].erase(0, dotpos + 1);
        }
    if (vnum < 1)
        {
            mobile->Message(MSG_ERROR, "Invalid vnum.");
            return false;
        }

//now we need to get the object we're actually editing.
    switch(subcmd)
        {
        case COS_OBJECT:
            o.o = world->GetVirtual(vnum);
            if (!component.length())
                {
                    group = omanager->GetGroup(OLCGROUP::STATIC);
                    if (!group)
                        {
                            mobile->Message(MSG_ERROR, "That group does not exist.");
                            return false;
                        }
                    break;
                }
            else
                {
                    ocomponent = o.o->GetComponent(component);
                    if (!ocomponent)
                        {
                            mobile->Message(MSG_ERROR, "That component does not exist.");
                            return false;
                        }
                    meta = ocomponent->GetMeta();
                    if (!meta)
                        {
                            mobile->Message(MSG_ERROR, "That component does not have a meta attached.");
                            return false;
                        }
                    if (meta->GetOlcGroup() == OLCGROUP::NONE)
                        {
                            mobile->Message(MSG_ERROR, "There is no olc attached to that component.");
                            return false;
                        }
                    group = omanager->GetGroup(meta->GetOlcGroup());
                    if (!group)
                        {
                            mobile->Message(MSG_ERROR, "That group does not exist.");
                            return false;
                        }
                }
            break;
        case COS_ROOM:
            o.r = world->GetRoom(vnum);
            if (!component.length())
                {
                    group = omanager->GetGroup(OLCGROUP::ROOM);
                    if (!group)
                        {
                            mobile->Message(MSG_ERROR, "That group does not exist.");
                            return false;
                        }
                    break;
                }
            else
                {
                    ocomponent = o.r->GetComponent(component);
                    if (!ocomponent)
                        {
                            mobile->Message(MSG_ERROR, "That component does not exist.");
                            return false;
                        }
                    meta = ocomponent->GetMeta();
                    if (!meta)
                        {
                            mobile->Message(MSG_ERROR, "That component does not have a meta attached.");
                            return false;
                        }
                    if (meta->GetOlcGroup() == OLCGROUP::NONE)
                        {
                            mobile->Message(MSG_ERROR, "There is no olc attached to that component.");
                            return false;
                        }
                    group = omanager->GetGroup(meta->GetOlcGroup());
                    if (!group)
                        {
                            mobile->Message(MSG_ERROR, "That group does not exist.");
                            return false;
                        }
                }
            break;
        case COS_MOB:
            o.n = world->GetNpc(vnum);
            if (!component.length())
                {
                    group = omanager->GetGroup(OLCGROUP::NPC);
                    if (!group)
                        {
                            mobile->Message(MSG_ERROR, "That group does not exist.");
                            return false;
                        }
                    break;
                }
            else
                {
                    ocomponent = o.n->GetComponent(component);
                    if (!ocomponent)
                        {
                            mobile->Message(MSG_ERROR, "That component does not exist.");
                            return false;
                        }
                    meta = ocomponent->GetMeta();
                    if (!meta)
                        {
                            mobile->Message(MSG_ERROR, "That component does not have a meta attached.");
                            return false;
                        }
                    if (meta->GetOlcGroup() == OLCGROUP::NONE)
                        {
                            mobile->Message(MSG_ERROR, "There is no olc attached to that component.");
                            return false;
                        }
                    group = omanager->GetGroup(meta->GetOlcGroup());
                    if (!group)
                        {
                            mobile->Message(MSG_ERROR, "That group does not exist.");
                            return false;
                        }
                }
            break;
        }

    if (!o.o)
        {
            mobile->Message(MSG_ERROR, "That vnum does not exist.");
            return false;
        }

    if (args.size() == 1) //we only have an object or object.component, show entries.
        {
            ShowGroup(mobile, group);
            return true;
        }
    name = args[1];
    entry = group->GetEntry(name);

    if (args.size() <= 2 && entry && entry->GetInputType() != OLCDT::EDITOR)
        {
            mobile->Message(MSG_ERROR, "Syntax: "+verb+" <vnum> <field");
            return false;
        }

    if (entry->RequiresInput())
        {
            it = args.begin();
            advance(it, 2);
            value = Explode(args, it);
        }
    else
        {
            value.clear();
        }

    switch(subcmd)
        {
        case COS_OBJECT:
        {
            if (entry->HandleInput(mobile, o.o, value))
                {
                    mobile->Message(MSG_INFO, "ok.");
                }
            else
                {
                    mobile->Message(MSG_ERROR, "Invalid input.");
                }
            break;
        }
        case COS_ROOM:
        {
            if (entry->HandleInput(mobile, o.r, value))
                {
                    mobile->Message(MSG_INFO, "ok.");
                }
            else
                {
                    mobile->Message(MSG_ERROR, "Invalid input.");
                }
            break;
        }
        case COS_MOB:
        {
            if (entry->HandleInput(mobile, o.n, value))
                {
                    mobile->Message(MSG_INFO, "ok.");
                }
            else
                {
                    mobile->Message(MSG_ERROR, "Invalid input.");
                }
            break;
        }
        }

    return true;
}

BOOL InitializeOlc()
{
    World* world = World::GetPtr();
    CMDOlcSet* cmd = NULL;

    if (!InitializeStaticObjectOlcs())
        {
            return false;
        }

    cmd = new CMDOlcSet();
    cmd->SetName("oedit");
    cmd->SetSubcmd(COS_OBJECT);
    world->commands.AddCommand(cmd);
    cmd = new CMDOlcSet();
    cmd->SetName("redit");
    cmd->SetSubcmd(COS_ROOM);
    world->commands.AddCommand(cmd);
    cmd = new CMDOlcSet();
    cmd->SetName("medit");
    cmd->SetSubcmd(COS_MOB);
    world->commands.AddCommand(cmd);
    return true;
}
