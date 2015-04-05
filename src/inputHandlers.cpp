#include "mud.h"
#include "conf.h"
#include "socket.h"
#include "utils.h"
#include "inputHandlers.h"

YesNoHandler::YesNoHandler(YESNOCB cb, void* arg):InputHandle()
{
    _cb = cb;
    _arg = arg;
}

void YesNoHandler::Input(void* arg, const std::string &input)
{
    std::string text = input;
    Lower(text);
    if (text == "y" || text == "yes")
        {
            _cb((Socket*)arg, true, _arg);
        }
    _cb((Socket*)arg, false, _arg);
}

BOOL YesNoHandler::CreateHandler(Socket* sock,  YESNOCB cb, void* arg)
{
    if (sock == NULL)
        {
            return false;
        }

    in_data* input = new in_data();
    if (!input)
        {
            return false;
        }

    YesNoHandler* handle = new YesNoHandler(cb, arg);
    if (!handle)
        {
            delete input;
            return false;
        }

    input->handle = handle;
    input->args = (void*)sock;
    return sock->SetInput(input);
}

LineHandler::LineHandler(LINECB cb, void* arg):InputHandle()
{
    _cb = cb;
    _arg = arg;
}

void LineHandler::Input(void* arg, const std::string &input)
{
    std::string data = input;
    _cb((Socket*)arg, data, _arg);
}

BOOL LineHandler::CreateHandler(Socket* sock,  LINECB cb)
{
    if (sock == NULL)
        {
            return false;
        }

    in_data* input = new in_data();
    if (!input)
        {
            return false;
        }

    LineHandler* handle = new LineHandler(cb);
    if (!handle)
        {
            delete input;
            return false;
        }

    input->handle = handle;
    input->args = (void*)sock;
    return sock->SetInput(input);
}

TextBlockHandler::TextBlockHandler(TEXTBLOCKCB cb,  std::vector<std::string>* lines, void* arg)
{
    _arg = arg;
    _cb = cb;
    _lines = lines;
}
void TextBlockHandler::Input(void* arg, const std::string &input)
{
    if (input == ".")
        {
            std::string data = input;
            _cb((Socket*)arg, _lines, _arg);
            return;
        }
    if (_lines)
        {
            _lines->push_back(input);
        }
}

BOOL TextBlockHandler::CreateHandler(Socket* sock, TEXTBLOCKCB cb, std::vector<std::string>* lines, void* arg)
{
    if (sock == NULL || lines == NULL)
        {
            return false;
        }

    in_data* input = new in_data();
    if (!input)
        {
            return false;
        }

    TextBlockHandler* handle = new TextBlockHandler(cb, lines, arg);
    if (!handle)
        {
            delete input;
            return false;
        }

    input->handle = handle;
    input->args = (void*)sock;
    return sock->SetInput(input);
}
