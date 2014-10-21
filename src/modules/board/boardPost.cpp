#include "../../mud.h"
#include "../../conf.h"
#include "../../uuid.h"
#include "../../player.h"
#include "boardPost.h"

BoardPost::BoardPost()
{
}
BoardPost::BoardPost(const std::string &s, const std::string &m)
{
    _message = m;
    _subject = s;
}
BoardPost::~BoardPost()
{
}

std::string BoardPost::GetSubject() const
{
    return _subject;
}
void BoardPost::SetSubject(const std::string &s)
{
    _subject = s;
}
std::string BoardPost::GetMessage() const
{
    return _message;
}
void BoardPost::SetMessage(const std::string &m)
{
    _message = m;
}
std::string BoardPost::GetPoster() const
{
    return _poster;
}

BOOL BoardPost::IsPoster(Player* mobile)
{
    return (mobile->GetRealUuid() == _pid);
}
void BoardPost::SetPoster(Player* mobile)
{
    _poster = mobile->GetName();
    _pid = mobile->GetUuid();
}

void BoardPost::Serialize(TiXmlElement* root)
{
    TiXmlElement* post = new TiXmlElement("post");
    post->SetAttribute("subject", _subject.c_str());
    post->SetAttribute("message", _message.c_str());
    post->SetAttribute("poster", _poster.c_str());
    _pid.Serialize(post);
    root->LinkEndChild(post);
}
void BoardPost::Deserialize(TiXmlElement* root)
{
    _pid.Deserialize(root);
    _subject = root->Attribute("subject");
    _message = root->Attribute("message");
    _poster = root->Attribute("poster");
}
