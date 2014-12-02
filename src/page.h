#pragma once
#include <string>
#include <vector>
#include "mud.h"
#include "conf.h"
#include "player.h"
#include "socket.h"
#include "inputHandlers.h"

class Pager
{
protected:
    int _pages;
    int _pagelen;
    Player* _mobile;
    std::vector<std::string> _lines;

    void AddLines(const std::vector<std::string>& lines);
    void AddLines(const std::string& text);
public:
    Pager(const std::vector<std::string>& lines, int plen);
    Pager(const std::string& text, int plen);
};
