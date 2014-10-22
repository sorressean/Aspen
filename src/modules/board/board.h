/*
*The main board system.
*Holds posts for a single board etc.
*/
#pragma once
#include <tinyxml.h>
#include <vector>
#include <string>
#include "boardPost.h"
#include "../modules.h"
#include "../../mud.h"
#include "../../mud.h"

#ifdef MODULE_BOARD

class Board
{
    std::vector<BoardPost*> _posts;
    std::string _name;
    FLAG _access;
public:
    Board();
    ~Board();
    void SetName(const std::string &name);
    std::string GetName() const;
    void SetAccess(FLAG access);
    FLAG GetAccess() const;
    void AddPost(BoardPost* post);
    std::vector<BoardPost*>* GetPosts();
    BoardPost* GetPostByIndex(int index);
    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
};

#endif
