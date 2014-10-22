#pragma once
#include <tinyxml.h>
#include <string>
#include "../modules.h"
#include "../../mud.h"
#include "../../conf.h"
#include "../../uuid.h"
#include "../../player.h"

#ifdef MODULE_BOARD

class BoardPost
{
    std::string _subject;
    std::string _message;
    std::string _poster;
    Uuid _pid;
public:
    BoardPost();
    BoardPost(const std::string &s, const std::string& m);
    ~BoardPost();
    std::string GetSubject() const;
    void SetSubject(const std::string &s);
    std::string GetMessage() const;
    void SetMessage(const std::string &m);
    std::string GetPoster() const;
    BOOL IsPoster(Player* mobile);
    void SetPoster(Player* mobile);
    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
};

#endif
