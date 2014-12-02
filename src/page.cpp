#include <string>
#include <vector>
#include <algorithm>
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
    AddLines(lines);
}
Pager::Pager(const std::string& text, int plen)
{
    _pagelen = plen;
    AddLines(text);
}
