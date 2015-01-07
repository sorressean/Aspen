#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include "mud.h"
#include "conf.h"
#include "page.h"
#include "player.h"
#include "socket.h"
#include "inputHandlers.h"
#include "utils.h"

void Pager::AddLines(const std::vector<std::string>& lines)
{
    std::copy(lines.begin(), lines.end(), std::back_inserter(_lines));
}
void Pager::AddLines(const std::string& text)
{
    std::vector<std::string> lines;
    Tokenize(text, lines, "\r\n");
    AddLines(lines);
}

Pager::Pager(const std::vector<std::string>& lines, int plen)
{
    _pagelen = plen;
    _pagepos = 0;
    _mobile = nullptr;

    AddLines(lines);
}
Pager::Pager(const std::string& text, int plen)
{
    _pagelen = plen;
    _mobile = nullptr;

    AddLines(text);
}

bool Pager::EnterPager(Player* mobile)
{
    _mobile = mobile;
    LineHandler::CreateHandler(_mobile->GetSocket(), std::bind(&Pager::Input, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}
void Pager::Input(void* arg, const std::string& input)
{
}
int Pager::PageCount() const
{
    return Min((int)(_lines.size()/(_pagelen-3)), 1);
}
void Pager::PrintPage()
{
    if (_mobile == nullptr)
        {
            return;
        }
}
