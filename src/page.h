#pragma once
#include <string>
#include <vector>
#include "mud.h"
#include "conf.h"
#include "player.h"
#include "socket.h"
#include "inputHandlers.h"
#include "utils.h"

class Pager
{
protected:
    int _pagepos;
    int _pagelen;
    Player* _mobile;
    std::vector<std::string> _lines;

    void AddLines(const std::vector<std::string>& lines);
    void AddLines(const std::string& text);
public:
    Pager(const std::vector<std::string>& lines, int plen);
    Pager(const std::string& text, int plen);

    bool EnterPager(Player* mobile);
    void Input(void* arg, const std::string& input);
    int PageCount() const;
    void PrintPage();
};
