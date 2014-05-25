/*
*These are basic utility classes, that create generic input handlers.
*/
#pragma once
#ifndef INPUT_HANDLERS_H
#define INPUT_HANDLERS_H
#include "mud.h"
#include "conf.h"
#include "socket.h"
#include <functional>
typedef std::function<void (Socket*, BOOL, void*)> YESNOCB;
typedef std::function<void (Socket*, std::string&, void*)> LINECB;
typedef std::function<void (Socket*, std::vector<std::string>*, void*)> TEXTBLOCKCB;

class YesNoHandler:public InputHandle
{
    YESNOCB _cb;
    void* _arg;
public:
    YesNoHandler(YESNOCB cb, void* arg=NULL);
    void Input(void* arg, const std::string &input);
    static BOOL CreateHandler(Socket* sock, YESNOCB cb, void* arg = NULL);
};

class LineHandler:public InputHandle
{
    LINECB _cb;
    void* _arg;
public:
    LineHandler(LINECB cb, void* arg = NULL);
    void Input(void* arg, const std::string &input);
    static BOOL CreateHandler(Socket* sock, LINECB cb);
};

/*
*Our textblock handler, used for receiving large amounts of text, such as a message,
*multiple lines in an editor, etc.
*/
class TextBlockHandler:public InputHandle
{
    TEXTBLOCKCB _cb;
    std::vector<std::string>* _lines;
    void* _arg;
public:
    TextBlockHandler(TEXTBLOCKCB cb,  std::vector<std::string>* lines, void* arg = NULL);
    void Input(void* arg, const std::string &input);
    static BOOL CreateHandler(Socket* sock, TEXTBLOCKCB cb, std::vector<std::string>* lines, void* arg = NULL);
};
#endif
