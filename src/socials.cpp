#include <tinyxml2.h>
#include <string>
#include <map>
#include "socials.h"
#include "world.h"
#include "living.h"
#include "log.h"
#include "command.h"
#include "utils.h"

Socials* Socials::_ptr;
Socials* Socials::GetPtr()
{
    if (!Socials::_ptr)
        {
            Socials::_ptr = new Socials();
            Socials::_ptr->Initialize();
        }
    return Socials::_ptr;
}

void Socials::Initialize()
{
    _socid = 1;
}
Socials::~Socials()
{
    for (auto it:_slist)
        {
            delete it.second;
        }
}

void Socials::Save()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* socials = doc.NewElement("socials");
    tinyxml2::XMLElement* social = nullptr;
    SOCIAL_DATA* sdata = nullptr;
    doc.InsertEndChild(doc.NewDeclaration());

    for (auto it: _slist)
        {
            sdata = it.second;
            social = doc.NewElement("social");
            social->SetAttribute("id", sdata->id);
            social->SetAttribute("name", sdata->name.c_str());
            social->SetAttribute("ynotarg", sdata->ynotarg.c_str());
            social->SetAttribute("rnotarg", sdata->rnotarg.c_str());
            social->SetAttribute("ttarg", sdata->ttarg.c_str());
            social->SetAttribute("rtarg", sdata->rtarg.c_str());
            social->SetAttribute("ytarg", sdata->ytarg.c_str());
            socials->InsertEndChild(social);
        }

    doc.InsertEndChild(socials);
    doc.SaveFile(SOCIALS_FILE);
}
void Socials::Load()
{
    int id = 0;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* socials = nullptr;
    tinyxml2::XMLElement* social = nullptr;
    SOCIAL_DATA* data = nullptr;

    if (!FileExists(SOCIALS_FILE))
        {
#ifdef NO_INIT_DEFAULTS
            WriteLog("No socials file exists, and NO_INIT_DEFAULTS was enabled.", CRIT);
#else
            InitializeDefaultSocials();
#endif
            return;
        }

    if (doc.LoadFile(SOCIALS_FILE) != tinyxml2::XML_NO_ERROR)
        {
            WriteLog("Could not load XML socials file.");
            return;
        }

    socials = doc.FirstChildElement("socials");
    if (socials)
        {
            for (social = socials->FirstChildElement(); social; social = social->NextSiblingElement())
                {
                    data = new SOCIAL_DATA();
                    data->id = social->IntAttribute("id");
                    data->name = social->Attribute("name");
                    data->ynotarg = social->Attribute("ynotarg");
                    data->rnotarg = social->Attribute("rnotarg");
                    data->ttarg = social->Attribute("ttarg");
                    data->rtarg = social->Attribute("rtarg");
                    data->ytarg = social->Attribute("ytarg");

                    if (id > _socid)
                        {
                            _socid = id+1;
                        }

                    _slist[data->name] = data;
                }
        }
}

bool Socials::SocialExists(const std::string &name) const
{
    return (_slist.count(name) == 0? false:true);
}
bool Socials::AddSocial(const std::string &name, const std::string &ynotarg, const std::string &rnotarg, const std::string &ytarg, const std::string &rtarg, const std::string &ttarg)
{
    if (SocialExists(name))
        {
            return false;
        }

    SOCIAL_DATA* data = new SOCIAL_DATA();
    data->name = name;
    data->ynotarg = ynotarg;
    data->rnotarg = rnotarg;
    data->ttarg = ttarg;
    data->rtarg = rtarg;
    data->ytarg = ytarg;
    data->id = _socid;
    _socid++;

    _slist[name] = data;
    return true;
}
SOCIAL_DATA* Socials::GetSocial(const std::string &name)
{
    if (!SocialExists(name))
        {
            return nullptr;
        }

    return _slist[name];
}

void Socials::InitializeDefaultSocials()
{
#ifndef NO_INIT_DEFAULTS
    AddSocial("smile", "You %a %v.", "%N %a %vs.", "You %a %v at %T.", "%N %a %vs at %T.", "%N %a %vs at you.");
    AddSocial("grin", "You %v %a.", "%N %vs %a.", "You %v %a at %T.", "%N %vs %a at %T.", "%N %vs %a at you.");
    AddSocial("nod", "You %v %a.", "%N %vs %a.", "You %v %a at %T.", "%N %vs %a at %T.", "%N %vs %a at you.");
    AddSocial("frown", "You %v %a.", "%N %vs %a.", "You %v %a at %T.", "%N %vs %a at %T.", "%N %vs %a at you.");
    AddSocial("wave", "You %a %v.", "%N %a %vs.", "You %a %v to %T.", "%N %a %vs to %T.", "%N %a %vs to you.");
    AddSocial("agree", "You %a %v.", "%N %a %vs.", "You %a %v with %T.", "%N %a %vs with %T.", "%N %a %vs with you.");
    AddSocial("chuckle", "You %v %a.", "%N %vs %a.", "You %v %a at %T.", "%N %vs %a at %T.", "%N %vs %a at you.");
    AddSocial("cower", "You %v in a corner.", "%N %vs in a corner.", "You %v in fear from %T.", "%N %vs in fear from %T.", "%N %vs in fear from you.");
    AddSocial("cry", "You %v.", "%N %vs.", "You %v on %T's shoulder.", "%N %vs on %T's shoulder.", "%N %vs on your shoulder.");
    AddSocial("wink", "you %v suggestively", "%N %vs suggestively", "You %v suggestively at %T.", "%N %vs suggestively at %T.", "%N %vs suggestively at you.");
    AddSocial("addict", "You stand and proclaim, \"I am %N, and I am a mud addict.\"", "%N stands and proclaims, \"I am %N, and I am a mud addict.\"", "You stand and proclaim to %T, \"I am %N, and I am a mud addict.\"", "%N stands and proclaims to %T, \"I am %N, and I am a mud addict.\"", "%N proclaims to you, \"I am %N, and I am a mud addict.\"");
    AddSocial("aargh", "You shout, %v!", "%N shouts, %v!", "You shout, %v at %T.", "%N shouts, %v at %T.", "%N shouts, %v at you.");
    Save();
#endif
}

void Socials::AddCommands()
{
    World* world = World::GetPtr();

    WriteLog("Adding social commands.");
    for (auto it: _slist)
        {
            CMDSocials*com = new CMDSocials();
            com->SetName(it.second->name);
            com->SetType(CommandType::Social);
            com->SetSubcmd(it.second->id);
            world->commands.AddCommand(com);
        }
}

EVENT(socials_shutdown)
{
    WriteLog("Cleaning up socials.");
    Socials* soc = Socials::GetPtr();
    soc->Save();
    delete soc;
}

CMDSocials::CMDSocials()
{
}
bool CMDSocials::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    Socials* soc = Socials::GetPtr();
    SOCIAL_DATA* data = soc->GetSocial(verb);
    if (data == nullptr)
        {
            mobile->Message(MSG_ERROR, "That social doesn't exist.");
            return false;
        }
    return true;
}

bool InitializeSocials()
{
    World* world = World::GetPtr();
    WriteLog("Initializing socials.");
    Socials* soc = Socials::GetPtr();
    soc->Load();
    soc->AddCommands();
    world->events.AddCallback("Shutdown", socials_shutdown);
    world->events.AddCallback("Copyover", socials_shutdown);
    return true;
}
