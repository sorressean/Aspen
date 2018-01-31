#include "mud.h"
#include "socket.h"
#include "utils.h"
#include "inputHandlers.h"

YesNoHandler::YesNoHandler(YESNOCB cb, void* arg):
    InputHandle(), _cb(cb), _arg(arg)
{
}

void YesNoHandler::Input(void* arg, const std::string &input)
{
    std::string text = input;
    Lower(text);
    if (text == "y" || text == "yes")
        {
            _cb((Socket*)arg, true, _arg);
        }
    else
        {
            _cb((Socket*)arg, false, _arg);
        }
}

bool YesNoHandler::CreateHandler(Socket* sock,  YESNOCB cb, void* arg)
{
    in_data* input = nullptr;
    YesNoHandler* handle  = nullptr;

    if (sock == nullptr)
        {
            return false;
        }

    input = new in_data();
    handle = new YesNoHandler(cb, arg);
    input->handle = handle;
    input->args = (void*)sock;
    return sock->SetInput(input);
}

LineHandler::LineHandler(LINECB cb, void* arg):
    InputHandle(), _cb(cb), _arg(arg)
{
}

void LineHandler::Input(void* arg, const std::string &input)
{
    std::string data = input;
    _cb((Socket*)arg, data, _arg);
}

bool LineHandler::CreateHandler(Socket* sock,  LINECB cb)
{
    in_data* input = nullptr;
    LineHandler* handle = nullptr;

    if (sock == nullptr)
        {
            return false;
        }

    input = new in_data();
    handle = new LineHandler(cb);
    input->handle = handle;
    input->args = (void*)sock;
    return sock->SetInput(input);
}

TextBlockHandler::TextBlockHandler(TEXTBLOCKCB cb,  std::vector<std::string>* lines, void* arg):
    InputHandle(), _cb(cb), _lines(lines), _arg(arg)
{
}

void TextBlockHandler::Input(void* arg, const std::string &input)
{
    if (input == ".")
        {
            _cb((Socket*)arg, _lines, _arg);
            return;
        }

    if (_lines)
        {
            _lines->push_back(input);
        }
}

bool TextBlockHandler::CreateHandler(Socket* sock, TEXTBLOCKCB cb, std::vector<std::string>* lines, void* arg)
{
    in_data* input = nullptr;
    TextBlockHandler* handle = nullptr;

    if (sock == nullptr || lines == nullptr)
        {
            return false;
        }

    input = new in_data();
    handle = new TextBlockHandler(cb, lines, arg);
    input->handle = handle;
    input->args = (void*)sock;
    return sock->SetInput(input);
}
