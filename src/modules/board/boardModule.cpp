#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "boardModule.h"
#include "boardManager.h"
#include "boardPost.h"
#include "board.h"
#include "../modules.h"
#include "../../mud.h"
#include "../../conf.h"
#include "../../property.h"
#include "../../event.h"
#include "../../command.h"
#include "../../world.h"
#include "../../utils.h"
#include "../../editor.h"

#ifdef MODULE_BOARD

using std::right;
using std::endl;
using std::left;
using std::setw;

CMDBoard::CMDBoard()
{
    SetName("board");
}
CEVENT(CMDBoard, _SavePost)
{
    Editor* ed = ((EditorSavedArgs*)args)->editor;
    BoardPost* post = (BoardPost*)ed->GetArg();
    std::vector<std::string>* lines;

    lines = ed->GetLines();
    post->SetMessage(Explode(*lines, "\n"));
}
CEVENT(CMDBoard, _AbortPost)
{
    Editor* ed = ((EditorAbortedArgs*)args)->editor;
    BoardPost* post = (BoardPost*)ed->GetArg();

    delete post;
}
CEVENT(CMDBoard, _ExitPost)
{
    World* world = World::GetPtr();
    Editor* ed = ((EditorExitedArgs*)args)->editor;
    BoardPost* post = (BoardPost*)ed->GetArg();
    Player* mobile = (Player*)caller;
    BoardManager* bmanager = (BoardManager*)world->GetProperty("boards");
    Property* prop = NULL;
    Board* board = NULL;
    int id = 0;

    if (!bmanager)
        {
            mobile->Message(MSG_ERROR, "Can not access the board system.");
            return;
        }

    prop = mobile->variables.FindProperty("board");
    if (!prop)
        {
            mobile->Message(MSG_ERROR, "No board set.");
            return;
        }

    id = prop->GetValue().GetInt();
    board = bmanager->GetBoardByIndex(id);
    if (!board)
        {
            mobile->Message(MSG_ERROR, "The board you have set does not exist.");
            return;
        }

    board->AddPost(post);
}
void CMDBoard::Help(Player* mobile)
{
    mobile->Message(MSG_LIST, "board <help|list|show|read|write|set>");
    mobile->Message(MSG_LIST, "Help: shows this message.");
    mobile->Message(MSG_LIST, "show: shows all of the boards you have access to.");
    mobile->Message(MSG_LIST, "list: shows a list of messages on the board you are currently reading.");
    mobile->Message(MSG_LIST, "read [#]: allows you to read a message.");
    mobile->Message(MSG_LIST, "write [subject]: writes a message on the board currently set.");
    mobile->Message(MSG_LIST, "set [#]: sets your current board to the one specified.");
}
void CMDBoard::List(Player* mobile)
{
    World* world = World::GetPtr();
    BoardManager* bmanager = (BoardManager*)world->GetProperty("boards");
    Property* prop = NULL;
    int id = 0;
    int i = 0;
    Board* board = NULL;
    std::vector<BoardPost*>* posts;
    std::vector<BoardPost*>::iterator it, itEnd;
    std::stringstream st;

    if (!bmanager)
        {
            mobile->Message(MSG_ERROR, "Can not access the board system.");
            return;
        }

    prop = mobile->variables.FindProperty("board");
    if (!prop)
        {
            mobile->Message(MSG_ERROR, "No board set.");
            mobile->Message(MSG_ERROR, "Use board set to choose a board.");
            return;
        }

    id = prop->GetValue().GetInt();
    board = bmanager->GetBoardByIndex(id);
    if (!board)
        {
            mobile->Message(MSG_ERROR, "The board you have set does not exist.");
            return;
        }

    posts = board->GetPosts();
    if (!posts->size())
        {
            mobile->Message(MSG_INFO, "No messages.");
            return;
        }

    st << left << "#" << setw(12) << "poster" << right << "subject" << endl;
    st << Repete("-", 80) << endl;
    itEnd = posts->end();
    i = 1;
    for (it = posts->begin(); it != itEnd; ++it, ++i)
        {
            st << left << "[" << setw(5) << i << "]" << setw(12) << (*it)->GetPoster() << right << (*it)->GetSubject() << endl;
        }
    mobile->Message(MSG_LIST, st.str());
}
void CMDBoard::Write(Player* mobile, const std::string &subject)
{
    BoardPost*post = NULL;
    Editor* ed = NULL;

    if ((!subject.length()) || (subject.length() > 60))
        {
            mobile->Message(MSG_INFO, "You must provide a subject that is no longer than 60 characters.");
            return;
        }

    post = new BoardPost();
    post->SetSubject(subject);
    post->SetPoster(mobile);
    ed = new Editor();
    ed->SetArg((void*)post);
    ed->events.AddCallback("save", std::bind(&CMDBoard::_SavePost, this, std::placeholders::_1, std::placeholders::_2));
    ed->events.AddCallback("abort", std::bind(&CMDBoard::_AbortPost, this, std::placeholders::_1, std::placeholders::_2));
    ed->events.AddCallback("atexit", std::bind(&CMDBoard::_ExitPost, this, std::placeholders::_1, std::placeholders::_2));
    ed->EnterEditor(mobile);
}
void CMDBoard::Show(Player* mobile)
{
    World* world = World::GetPtr();
    BoardManager* bmanager = (BoardManager*)world->GetProperty("boards");
    std::vector<Board*> boards;
    std::vector<Board*>::iterator it, itEnd;
    int i = 1;
    std::stringstream st;

    bmanager->GetBoards(&boards);
    if (!boards.size())
        {
            mobile->Message(MSG_INFO, "There are no boards to show.");
            return;
        }

    itEnd = boards.end();
    for (it = boards.begin(); it!= itEnd; ++it, ++i)
        {
            st << left << "[" << setw(3) << i << "]" << right << (*it)->GetName() << endl;
        }
    mobile->Message(MSG_LIST, st.str());
}
void CMDBoard::Set(Player* mobile, int boardid)
{
    World* world = World::GetPtr();
    BoardManager* bmanager = (BoardManager*)world->GetProperty("boards");
    Board* board = NULL;
    Property* prop = NULL;

    if (!bmanager)
        {
            mobile->Message(MSG_ERROR, "Can not access the board system.");
            return;
        }

    board = bmanager->GetBoardByIndex(boardid);
    if (!board)
        {
            mobile->Message(MSG_ERROR, "Invalid board.");
            return;
        }

    prop = mobile->variables.FindProperty("board");
    if (!prop)
        {
            mobile->variables.AddProperty("board", boardid);
        }
    else
        {
            prop->SetValue(boardid);
        }

    mobile->Message(MSG_INFO, "board set");
}
void CMDBoard::Read(Player* mobile, int id)
{
    World* world = World::GetPtr();
    BoardPost*post = NULL;
    BoardManager* bmanager = (BoardManager*)world->GetProperty("boards");
    Board* board = NULL;
    Property* prop = NULL;
    int boardid = 0;
    std::stringstream st;

    if (!bmanager)
        {
            mobile->Message(MSG_ERROR, "Can not access the board system.");
            return;
        }

    prop = mobile->variables.FindProperty("board");
    if (!prop)
        {
            mobile->Message(MSG_ERROR, "No board set.");
            mobile->Message(MSG_ERROR, "Use board set to choose a board.");
            return;
        }

    boardid = prop->GetValue().GetInt();
    board = bmanager->GetBoardByIndex(boardid);
    if (!board)
        {
            mobile->Message(MSG_ERROR, "The board you have set does not exist.");
            return;
        }

    post = board->GetPostByIndex(id);
    if (!post)
        {
            mobile->Message(MSG_ERROR, "That post does not exist.");
            return;
        }

    st << left << setw(15) << "From: " << right << post->GetPoster() << endl;
    st << left << setw(15) << "Subject: " << right << post->GetSubject() << endl;
    st << post->GetMessage() << endl;
    mobile->Message(MSG_INFO, st.str());
}
BOOL CMDBoard::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args,int subcmd)
{
    if (!args.size())
        {
            Help(mobile);
            return true;
        }
    else if (args[0] == "show")
        {
            Show(mobile);
            return true;
        }
    else if(args[0] == "set" && args.size() == 2)
        {
            int indx = atoi(args[1].c_str());
            Set(mobile, indx);
            return true;
        }
    else if (args[0] == "write" && args.size() > 1)
        {
            std::vector<std::string>::iterator it, itEnd;
            std::vector<std::string> parts;
            std::string subject;

            itEnd = args.end();
            for (it = ++(args.begin()); it != itEnd; ++it)
                {
                    parts.push_back((*it));
                }
            subject = Explode(parts, " ");
            Write(mobile, subject);
            return true;
        }
    else if(args[0] == "read" && args.size() == 2)
        {
            int id = atoi(args[1].c_str());
            Read(mobile, id);
            return true;
        }
    else if(args[0] == "list")
        {
            List(mobile);
            return true;
        }

    Help(mobile);
    return true;
}

CMDABoard::CMDABoard()
{
    SetName("aboard");
    SetAccess(RANK_ADMIN);
}
void CMDABoard::Help(Player*mobile)
{
    mobile->Message(MSG_LIST, "aboard <create>.");
    mobile->Message(MSG_LIST, "Help: shows this help message.");
    mobile->Message(MSG_LIST, "Create <name>: creates the specified board.");
}
BOOL CMDABoard::Execute(const std::string &verb, Player* mobile,std::vector<std::string> &args, int subcmd)
{
    World* world = World::GetPtr();

    if (!args.size())
        {
            Help(mobile);
            return true;
        }
    if (args[0] == "create" && args.size() > 1)
        {
            std::vector<std::string>::iterator it, itEnd;
            std::vector<std::string> parts;
            std::string name;
            BoardManager* bmanager;

            itEnd = args.end();
            for (it = ++(args.begin()); it != itEnd; ++it)
                {
                    parts.push_back((*it));
                }
            name = Explode(parts, " ");
            Board* board = new Board();
            board->SetName(name);
            bmanager = (BoardManager*)world->GetProperty("boards");
            bmanager->AddBoard(board);
            mobile->Message(MSG_INFO, "Added "+name+".");
            return true;
        }

    return  false;
}

EVENT(CleanupBoards)
{
    World* world = World::GetPtr();
    BoardManager* bmanager = (BoardManager*)world->GetProperty("boards");
    if (bmanager)
        {
            world->RemoveProperty("boards");
        }
}

#endif

bool InitializeBoards()
{
#ifdef MODULE_BOARD
    World* world = World::GetPtr();
    BoardManager* bmanager = new BoardManager();

    world->WriteLog("Initializing boards.");
    world->AddState("boards", bmanager);
    world->commands.AddCommand(new CMDBoard());
    world->commands.AddCommand(new CMDABoard());
    world->AddProperty("boards", (void*)bmanager);
    world->events.AddCallback("Shutdown", CleanupBoards);
#endif
    return true;
}
