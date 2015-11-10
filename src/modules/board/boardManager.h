#pragma once
#include <tinyxml2.h>
#include <vector>
#include "board.h"
#include "../modules.h"
#include "../../mud.h"
#include "../../conf.h"
#include "../../serializer.h"

#ifdef MODULE_BOARD

class BoardManager:public ISerializable
{
    std::vector<Board*> _boards;
public:
    BoardManager();
    ~BoardManager();
    void AddBoard(Board* board);
    void GetBoards(std::vector<Board*>* boards);
    Board* GetBoardByIndex(int index);
    void Serialize(tinyxml2::XMLElement* root);
    void Deserialize(tinyxml2::XMLElement* root);
};

#endif
