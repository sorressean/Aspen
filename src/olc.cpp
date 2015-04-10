#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "mud.h"
#include "conf.h"
#include "olc.h"
#include "baseObject.h"
#include "world.h"
#include "objectManager.h"
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

    st << Repeat('-', 80)  << std::endl;
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


//this has to be a vnum:
            try
                {
                    num = tonum(arg.c_str());
                    return true;
                }
            catch (std::bad_cast e)
                {
                    if (inroom)
                        {
                            num = mobile->GetLocation()->GetOnum();
                            return true;
                        }

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
            num = tonum(arg.c_str());
            return true;
        }
    catch (std::bad_cast e)
        {
            return false;
        }

    return true;
}

//Used to call edit on the object.
static inline bool DoEdit(Player* mobile, BaseObject* obj, IOlcEntry* entry, const std::string& input, OlcEditType type)
{
    switch(type)
        {
        case OlcEditType::Room:
            return entry->HandleInput(mobile, (Room*)obj, input);
        case OlcEditType::Npc:
            return entry->HandleInput(mobile, (Living*)obj, input);
        case OlcEditType::Object:
            return entry->HandleInput(mobile, (StaticObject*)obj, input);
        default:
            return false;
        }

    return false;
}

bool HandleEntry(Player* mobile, BaseObject* obj, OlcGroup* group, std::vector<std::string> &args, OlcEditType type)
{
    IOlcEntry* entry = nullptr;
    std::string name;
    std::string value;
    std::vector<std::string>::iterator it;

    if (isnum(args[0].c_str()))
        {
            if (args.size() == 1) //we only have object/object.component, show entries.
                {
                    ShowGroup(mobile, group);
                    return true;
                }
        }
    else if (args.empty())
        {
            ShowGroup(mobile, group);
        }

    if (!isnum(args[0].c_str()) && group->GetEntry(args[0]) != nullptr)
        {
            name = args[0];
            entry = group->GetEntry(name);
        }
    else if (isnum(args[0].c_str()) && group->GetEntry(args[0]) == nullptr)
        {
            name = args[1];
            entry = group->GetEntry(name);
        }
    if (entry == nullptr)
        {
            mobile->Message(MSG_ERROR, "That entry does not exist.");
            return false;
        }

    //if (args.size() == 2) //we hope this is an editor. Otherwise it fails.
    //  {
    if (entry->GetInputType() == OLCDT::EDITOR)
        {
            return DoEdit(mobile, obj, entry, "", OlcEditType::Room);
        }
    //    else
    //      {
    //        mobile->Message(MSG_ERROR, "Invalid syntax.");
    //      return false;
    // }
    //}

    if (args.size() >= 2) //we have input.
        {
            it = args.begin();
            if (isnum(args[0].c_str()))
                {
                    advance(it, 2);
                }
            else
                {
                    advance(it, 1);
                }
            value = Explode(args, it);
            if( DoEdit(mobile, obj, entry, value, OlcEditType::Room) )
                {
                    mobile->Message(MSG_INFO, "OK.");
                }
        }

//we should never get here.
    return false;
}

CMDREdit::CMDREdit()
{
    SetName("redit");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDREdit::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* objmanager = world->GetObjectManager();
    Zone* zon = mobile->GetLocation()->GetZone();
    Component* component = nullptr;
    IComponentMeta* cmeta = nullptr;
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = nullptr;
    Room* targ = nullptr;
    VNUM num = 0;
    std::string comp;

    if (args.size() == 0)
        {
            mobile->Message(MSG_ERROR, "Syntax: redit [<vnum>][.component] [field] [value].");
            return false;
        }

    if (!ParseVnum(mobile, args[0], num, comp, true))
        {
            mobile->Message(MSG_ERROR, "Invalid vnum.");
            return false;
        }

    if (num < 1 || !zon->RoomExists(num))
        {
            mobile->Message(MSG_ERROR, "That vnum does not exist.");
            return false;
        }

    targ = objmanager->GetRoom(num);
    if (targ == nullptr)
        {
            mobile->Message(MSG_ERROR, "Could not retrieve specified room.");
            return false;
        }

//no component:
    if (!comp.length())
        {
            group = omanager->GetGroup(OLCGROUP::ROOM);
            if (!group)
                {
                    mobile->Message(MSG_ERROR, "That group does not exist.");
                    return false;
                }
            return HandleEntry(mobile, targ, group, args, OlcEditType::Room);
        }

//component was provided:
    component = targ->GetComponent(comp);
    if (!component)
        {
            mobile->Message(MSG_ERROR, "That component does not exist.");
            return false;
        }
    cmeta = component->GetMeta();
    if (!cmeta)
        {
            mobile->Message(MSG_ERROR, "That component does not have a meta attached.");
            return false;
        }
    if (cmeta->GetOlcGroup() == OLCGROUP::NONE)
        {
            mobile->Message(MSG_ERROR, "There is no olc attached to that component.");
            return false;
        }
    group = omanager->GetGroup(cmeta->GetOlcGroup());
    if (!group)
        {
            mobile->Message(MSG_ERROR, "That group does not exist.");
            return false;
        }

    return HandleEntry(mobile, targ, group, args, OlcEditType::Room);
}

CMDMEdit::CMDMEdit()
{
    SetName("medit");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDMEdit::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* objmanager = world->GetObjectManager();
    Zone* zon = mobile->GetLocation()->GetZone();
    Component* component = nullptr;
    IComponentMeta* cmeta = nullptr;
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = nullptr;
    Npc* targ = nullptr;
    VNUM num = 0;
    std::string comp;

    if (args.size() == 0)
        {
            mobile->Message(MSG_ERROR, "Syntax: medit <vnum>[.component] [field] [value].");
            return false;
        }

    if (!ParseVnum(mobile, args[0], num, comp))
        {
            mobile->Message(MSG_ERROR, "Invalid vnum.");
            return false;
        }
    if (num < 1 || !zon->NpcExists(num))
        {
            mobile->Message(MSG_ERROR, "That vnum does not exist.");
            return false;
        }

    targ = objmanager->GetNpc(num);
    if (targ == nullptr)
        {
            mobile->Message(MSG_ERROR, "Could not retrieve specified NPC.");
            return false;
        }

//no component:
    if (!comp.length())
        {
            group = omanager->GetGroup(OLCGROUP::NPC);
            if (!group)
                {
                    mobile->Message(MSG_ERROR, "That group does not exist.");
                    return false;
                }
            return HandleEntry(mobile, targ, group, args, OlcEditType::Npc);
        }

//component was provided:
    component = targ->GetComponent(comp);
    if (!component)
        {
            mobile->Message(MSG_ERROR, "That component does not exist.");
            return false;
        }
    cmeta = component->GetMeta();
    if (!cmeta)
        {
            mobile->Message(MSG_ERROR, "That component does not have a meta attached.");
            return false;
        }
    if (cmeta->GetOlcGroup() == OLCGROUP::NONE)
        {
            mobile->Message(MSG_ERROR, "There is no olc attached to that component.");
            return false;
        }
    group = omanager->GetGroup(cmeta->GetOlcGroup());
    if (!group)
        {
            mobile->Message(MSG_ERROR, "That group does not exist.");
            return false;
        }

    return HandleEntry(mobile, targ, group, args, OlcEditType::Npc);
}

CMDOEdit::CMDOEdit()
{
    SetName("oedit");
    SetAccess(RANK_BUILDER);
    SetType(CommandType::Builder);
}
BOOL CMDOEdit::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
{
    World* world = World::GetPtr();
    ObjectManager* objmanager = world->GetObjectManager();
    Zone* zon = mobile->GetLocation()->GetZone();
    Component* component = nullptr;
    IComponentMeta* cmeta = nullptr;
    OlcManager* omanager = world->GetOlcManager();
    OlcGroup* group = nullptr;
    StaticObject* targ = nullptr;
    VNUM num = 0;
    std::string comp;

    if (args.size() == 0)
        {
            mobile->Message(MSG_ERROR, "Syntax: oedit <vnum>[.component] [field] [value].");
            return false;
        }

    if (!ParseVnum(mobile, args[0], num, comp))
        {
            mobile->Message(MSG_ERROR, "Invalid vnum.");
            return false;
        }
    if (num < 1 || !zon->VirtualExists(num))
        {
            mobile->Message(MSG_ERROR, "That vnum does not exist.");
            return false;
        }

    targ = objmanager->GetVirtual(num);
    if (targ == nullptr)
        {
            mobile->Message(MSG_ERROR, "Could not retrieve specified virtual object.");
            return false;
        }

//no component:
    if (!comp.length())
        {
            group = omanager->GetGroup(OLCGROUP::STATIC);
            if (!group)
                {
                    mobile->Message(MSG_ERROR, "That group does not exist.");
                    return false;
                }
            return HandleEntry(mobile, targ, group, args, OlcEditType::Object);
        }

//component was provided:
    component = targ->GetComponent(comp);
    if (!component)
        {
            mobile->Message(MSG_ERROR, "That component does not exist.");
            return false;
        }
    cmeta = component->GetMeta();
    if (!cmeta)
        {
            mobile->Message(MSG_ERROR, "That component does not have a meta attached.");
            return false;
        }
    if (cmeta->GetOlcGroup() == OLCGROUP::NONE)
        {
            mobile->Message(MSG_ERROR, "There is no olc attached to that component.");
            return false;
        }
    group = omanager->GetGroup(cmeta->GetOlcGroup());
    if (!group)
        {
            mobile->Message(MSG_ERROR, "That group does not exist.");
            return false;
        }

    return HandleEntry(mobile, targ, group, args, OlcEditType::Object);
}

BOOL InitializeOlc()
{
    World* world = World::GetPtr();
    world->commands.AddCommand(new CMDREdit());
    world->commands.AddCommand(new CMDMEdit());
    world->commands.AddCommand(new CMDOEdit());
    return true;
}
