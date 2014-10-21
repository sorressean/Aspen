#include <tinyxml.h>
#include <vector>
#include "board.h"
#include "boardManager.h"
#include "../../mud.h"
#include "../../conf.h"
#include "../../serializationHelpers.hpp"

BoardManager::BoardManager()
{
}
BoardManager::~BoardManager()
{
    std::vector<Board*>::iterator it, itEnd;

    itEnd = _boards.end();
    for (it = _boards.begin(); it != itEnd; ++it)
        {
            delete (*it);
        }
}

void BoardManager::AddBoard(Board* board)
{
    _boards.push_back(board);
}
void BoardManager::GetBoards(std::vector<Board*>* boards)
{
    std::vector<Board*>::iterator it, itEnd;

    itEnd = _boards.end();
    for (it = _boards.begin(); it != itEnd; ++it)
        {
            boards->push_back((*it));
        }
}
Board* BoardManager::GetBoardByIndex(int index)
{
    if ((index < 1) || (index > (int)_boards.size()))
        {
            return NULL;
        }

    return _boards[index-1];
}

void BoardManager::Serialize(TiXmlElement* root)
{
    SerializeList<Board, std::vector<Board*>, std::vector<Board*>::iterator>("boards", "board", root, _boards);
}
void BoardManager::Deserialize(TiXmlElement* root)
{
    DeserializeList<Board, std::vector<Board*> >(root, "boards", _boards);
}
