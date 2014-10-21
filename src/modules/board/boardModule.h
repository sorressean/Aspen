#pragma once
#include <vector>
#include <string>
#include "../../mud.h"
#include "../../conf.h"
#include "../../event.h"
#include "../../command.h"
#include "../../eventargs.h"

class CMDBoard:public Command
{
    EVENT(_SavePost);
    EVENT(_AbortPost);
    EVENT(_ExitPost);
public:
    CMDBoard();
    void Help(Player* mobile);
    void List(Player* mobile);
    void Write(Player* mobile, const std::string& subject);
    void Show(Player* mobile);
    void Set(Player* mobile, int boardid);
    void Read(Player* mobile, int id);
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};
class CMDABoard:public Command
{
public:
    CMDABoard();
    void Help(Player* mobile);
    BOOL Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd);
};

EVENT(CleanupBoards);
bool InitializeBoards();
