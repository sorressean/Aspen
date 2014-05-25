/*
 *Basic editor object
 */
#ifndef EDITOR_H
#define EDITOR_H
#include <vector>
#include <string>
#include "mud.h"
#include "conf.h"
#include "player.h"
#include "socket.h"
#include "event.h"
#include "inputHandlers.h"

class Editor
{
protected:
    Player* _mobile;
    std::vector <std::string> _lines;
    int _cursor;
    BOOL _dirty;
    void* _arg;
public:
    EventManager events;
    Editor();
    virtual    ~Editor();
    virtual BOOL Load();
    virtual void Save();
    virtual void Abort();
    virtual void Quit();
    virtual void List(BOOL lnum);
    virtual void Add(const std::string &line, BOOL quiet = false);
    virtual void Insert(int index);
    virtual void Delete();
    virtual void Delete(int index);
    virtual void Delete(int first, int second);
    virtual BOOL EnterEditor(Player* mobile);
    virtual void LeaveEditor();
    void SetArg(void* arg);
    void* GetArg() const;
    std::vector<std::string>* GetLines();
    void Input(void* arg, const std::string &input);
    void TextInput(Socket* sock, std::vector<std::string>* lines, void* args);
};
#endif
