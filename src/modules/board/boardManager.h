#pragma once
#include <tinyxml.h>
#include <vector>
#include "board.h"
#include "../../mud.h"
#include "../../conf.h"
#include "../../serializer.h"

class BoardManager:public ISerializable
{
    std::vector<Board*> _boards;
public:
    BoardManager();
    ~BoardManager();
    void AddBoard(Board* board);
    void GetBoards(std::vector<Board*>* boards);
    Board* GetBoardByIndex(int index);
    void Serialize(TiXmlElement* root);
    void Deserialize(TiXmlElement* root);
};
