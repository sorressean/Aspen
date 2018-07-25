#include <sstream>
#include <vector>
#include <functional>
#include <string>
#include "player.h"
#include "socket.h"
#include "utils.h"
#include "editor.h"
#include "event.h"
#include "eventargs.h"
#include "inputHandlers.h"

Editor::Editor()
{
    events.RegisterEvent("save");
    events.RegisterEvent("load");
    events.RegisterEvent("abort");
    events.RegisterEvent("atexit");

    _cursor = -1;
    _dirty=false;
    _arg = nullptr;
}

bool Editor::Load()
{
    EditorLoadedArgs arg(this);
    events.CallEvent("load", &arg, _mobile);
    return true;
}
void Editor::Save()
{
    _dirty=false;
    _mobile->Message(MSG_INFO, "saved.");
    EditorSavedArgs arg(this);
    events.CallEvent("save", &arg, this);
    return;
}
void Editor::Abort()
{
    _mobile->Message(MSG_INFO,"Aborting.");
    EditorAbortedArgs arg(this);
    events.CallEvent("abort", &arg, _mobile);
    LeaveEditor();
}
void Editor::Quit()
{
    if (!_dirty)
        {
            _mobile->Message(MSG_INFO,"Exiting editor.");
            LeaveEditor();
        }
    else
        {
            _mobile->Message(MSG_ERROR,"You have not saved your text. Please use \'s\', to save changes, or \'a\', to abort without saving.");
        }
}

void Editor::List(bool num)
{
    int i = 1;
    std::stringstream st;

    if (!_lines.size())
        {
            _mobile->Message(MSG_INFO, "Buffer is empty.");
            return;
        }

    for (auto it: _lines)
        {
            if (_cursor == i-1)
                {
                    st << "||";
                }
            if (num)
                {
                    st << i << ": ";
                }
            st << it << "\n";
            i++;
        }
    _mobile->Message(MSG_LIST,st.str());

    if (_cursor==-1)
        {
            _mobile->Message(MSG_LIST,"||");
        }
}

void Editor::Add(const std::string &line, bool quiet)
{
    std::stringstream st;
    std::vector <std::string>::iterator it;

    if (_cursor==-1)
        {
            _lines.push_back(line);
            if (!quiet)
                {
                    st << "Line " << _lines.size() << " added.";
                    _mobile->Message(MSG_INFO,st.str());
                }
            return;
        }
    else
        {
//the cursor is pointing to the insertion point; jump the iterator there and insert.
            it = _lines.begin();
            advance(it, _cursor-1);
            _lines.insert(it, line);
            if (!quiet)
                {
                    _mobile->Message(MSG_INFO,"Line inserted.");
                }
            _cursor++;
        }
}

void Editor::Insert(int index)
{
    std::stringstream st;

    if (index == 0)
        {
            _mobile->Message(MSG_ERROR, "Line out of range.");
            return;
        }
    if ((index==-1)||(index>=(int)_lines.size()))
        {
            _cursor=-1;
            _mobile->Message(MSG_INFO,"Cursor append to end of text.");
            return;
        }

    _cursor=index;
    st << "Insertion point set at line " << index << ".";
    _mobile->Message(MSG_INFO,st.str());
}

void Editor::Delete()
{
    std::vector <std::string>::iterator it;

    if (!_lines.size())
        {
            _mobile->Message(MSG_INFO,"Buffer is empty.");
            return;
        }
    if (_cursor==-1)
        {
            _lines.erase(_lines.end());
            _mobile->Message(MSG_INFO, "Deleted line.");
            return;
        }

    it = _lines.begin();
    advance(it, _cursor-1);
    _lines.erase(it);
    _cursor--;
    if (_cursor > ((int)_lines.size()-1))
        {
            _cursor = -1;
        }

    _mobile->Message(MSG_INFO,"Line erased.");
}
void Editor::Delete(int index)
{
    std::vector<std::string>::iterator it;
    int i=0;

    if ((index<=0)||(index>(int)_lines.size()))
        {
            _mobile->Message(MSG_ERROR,"Line out of range.");
            return;
        }

    it = _lines.begin();
    advance(it, index-1);
    _lines.erase(it);
    _mobile->Message(MSG_INFO,"Line erased.");

    if ((_cursor != -1) && (i < _cursor))
        {
            _cursor--;
        }
}
void Editor::Delete(int first, int second)
{
    int i=0;
    std::vector <std::string>::iterator top,bottom;

    if (((first>second)&&(second!=-1))||(first==second)||(first<=0)||(second>(int)_lines.size()))
        {
            _mobile->Message(MSG_ERROR,"Invalid range.");
            return;
        }
    if (second==(int)_lines.size())
        {
            second=-1;
        }

    for (top=_lines.begin(),i=0; i<first; top++,i++);
    if (second==-1)
        {
            bottom=_lines.end();
        }
    else
        {
            for (bottom=_lines.begin(),i=0; i<second; bottom++,i++);
        }

    _lines.erase(top,bottom);
    _cursor = -1; //we jump the cursor to the end.
    _mobile->Message(MSG_INFO,"Lines deleted.");
}

bool Editor::EnterEditor(Player* mobile)
{
    _mobile=mobile;
    _mobile->Message(MSG_INFO,"Entering editor. use \'h\' for help.");
    Load();
    LineHandler::CreateHandler(_mobile->GetSocket(), std::bind(&Editor::Input, this, std::placeholders::_1, std::placeholders::_2));
    return true;
}
void Editor::LeaveEditor()
{
    _mobile->GetSocket()->ClearInput();
    EditorExitedArgs arg(this);
    events.CallEvent("atexit", &arg, _mobile);
    delete this;
}

void Editor::SetArg(void* arg)
{
    _arg = arg;
}
void* Editor::GetArg() const
{
    return _arg;
}

std::vector<std::string>* Editor::GetLines()
{
    return &_lines;
}

void Editor::Input(void* arg, const std::string &input)
{
    int index=0;
    std::vector <std::string> tokens;

    if (input.empty())
        {
            return;
        }

    Tokenize(input,tokens);
//a list of lines
    if ((tokens[0]=="l")||(tokens[0]=="list"))
        {
            if ((tokens.size()>1)&&(tokens[1]=="#"))
                {
                    List(true);
                }
            else
                {
                    List(false);
                }
            return;
        }

//insertion
    if ((tokens[0]=="i")||(tokens[0]=="insert"))
        {
            if (tokens.size()==1)
                {
                    _mobile->Message(MSG_ERROR,"You must provide the line number where you want to place the cursor.");
                    return;
                }
            if (tokens[1]=="$")
                {
                    Insert(-1);
                }
            else
                {
                    index = atoi(tokens[1].c_str());
                    Insert(index);
                }
            return;
        }

//delete
    if ((tokens[0]=="delete")||(tokens[0]=="d"))
        {
            if (tokens.size()==1)
                {
                    Delete();
                }
            if (tokens.size()==2)
                {
                    Delete(atoi(tokens[1].c_str()));
                }
            if (tokens.size()==3)
                {
                    if (tokens[2]=="$")
                        {
                            Delete(atoi(tokens[1].c_str()),-1);
                        }
                    else
                        {
                            Delete(atoi(tokens[1].c_str()),atoi(tokens[2].c_str()));
                        }
                }
            return;
        }

//abort
    if (tokens[0]=="abort")
        {
            Abort();
            return;
        }

//help
    if ((tokens[0]=="h")||(tokens[0]=="help"))
        {
            _mobile->Message(MSG_INFO,"\t\tEditor help.");
            _mobile->Message(MSG_INFO, "\"[text]: insert a line. using \" by its self will append a blank line.");
            _mobile->Message(MSG_INFO,"i <index>: will place your cursor. Note: the insertion mark will appear on the line below your insertion point when you get a listing. i<$> will set the insertion mark at the end.");
            _mobile->Message(MSG_INFO,"D|d<line>|d<start> <end>: delete lines. d by its self will delete the line where the insertion point is located. Providing one argument will delete a specified line. You may also delete multiple lines, providing the start and end lines. You can substitute the '$' in any of these to signify the end.");
            _mobile->Message(MSG_INFO,"L <#>: will list the lines in the editor. Providing the optional '#' will show line numbers.");
            _mobile->Message(MSG_INFO, "t: enters text mode. This mode allows you to type all of your input without prefixing it with a \" character.");
            _mobile->Message(MSG_INFO, "H: shows this help.");
            _mobile->Message(MSG_INFO, "Q: Quit and prompt to save.");
            _mobile->Message(MSG_INFO,"A: abort without saving.");
            _mobile->Message(MSG_INFO, "s: Save changes.");
            return;
        }

//quit
    if ((tokens[0]=="quit")||(tokens[0]=="q"))
        {
            Quit();
            return;
        }

//save
    if ((tokens[0]=="save")||(tokens[0]=="s"))
        {
            Save();
            return;
        }

//enter text mode
    if (tokens[0] == "t" || tokens[0] == "text")
        {
            _mobile->Message(MSG_INFO, "Entering text mode. Use a '.' on a blank line to finish, or '@abort' on a blank line to abort.");
            std::vector<std::string>* text = new std::vector<std::string>();
            if (!TextBlockHandler::CreateHandler(_mobile->GetSocket(), std::bind(&Editor::TextInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), text))
                {
                    _mobile->Message(MSG_ERROR, "Could not create textblock input handler.");
                    return;
                }
        }

//append line
    if (input[0]=='\"')
        {
            if (input.length()==1)
                {
                    Add("\n");
                    return;
                }
            if ((input[1]==' ')&&(input.length()>2))
                {
                    index=2;
                }
            else
                {
                    index=1;
                }
//we strip off the quote and add the rest.
            Add(input.substr(index,input.length()));
            return;
        }

    _mobile->Message(MSG_ERROR, "Unknown editor command. Use \'h\' for help.");
}
void Editor::TextInput(Socket* sock, std::vector<std::string>* lines, void* args)
{
    std::vector<std::string>::iterator ins;

    if (_cursor == -1)
        {
            for (auto it: _lines)
                {
                    _lines.push_back(it);
                }
        }
    else
        {
            ins=_lines.begin();
            advance(ins, _cursor-1);
            for (auto it: _lines)
                {
                    _lines.insert(ins, it);
                }
        }

    _mobile->GetSocket()->ClearInput();
    _mobile->Message(MSG_INFO, "Returning to editor.");
    delete lines;
}
