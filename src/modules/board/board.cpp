#include <tinyxml.h>
#include <string>
#include <vector>
#include "board.h"
#include "boardPost.h"
#include "../../mud.h"
#include "../../conf.h"
#include "../../serializationHelpers.hpp"

#ifdef MODULE_BOARD
Board::Board()
{
}
Board::~Board()
{
    std::vector<BoardPost*>::iterator it, itEnd;

    itEnd = _posts.end();
    for (it = _posts.begin(); it != itEnd; ++it)
        {
            delete (*it);
        }
}

void Board::SetName(const std::string &name)
{
    _name = name;
}
std::string Board::GetName() const
{
    return _name;
}

void Board::SetAccess(FLAG access)
{
    _access = access;
}
FLAG Board::GetAccess() const
{
    return _access;
}

void Board::AddPost(BoardPost* post)
{
    _posts.push_back(post);
}
std::vector<BoardPost*>* Board::GetPosts()
{
    return &_posts;
}
BoardPost* Board::GetPostByIndex(int index)
{
    if ((index < 1) || (index > (int)_posts.size()))
        {
            return NULL;
        }

    return _posts[index-1];
}
void Board::Serialize(TiXmlElement* root)
{
    TiXmlElement* board = new TiXmlElement("board");
    board->SetAttribute("name", _name.c_str());
    board->SetAttribute("access", _access);
    SerializeList<BoardPost, std::vector<BoardPost*>, std::vector<BoardPost*>::iterator>("posts", "post", board, _posts);
    root->LinkEndChild(board);
}
void Board::Deserialize(TiXmlElement* board)
{
    unsigned int flag = 0;
    _name = board->Attribute("name");
    board->Attribute("access", &flag);
    _access = flag;

    DeserializeList<BoardPost, std::vector<BoardPost*> >(board, "posts", _posts);
}
#endif
