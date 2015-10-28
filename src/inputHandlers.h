/**
* Contains: various input handlers.
*
* Input handlers are used when you wish to request player input.
* They receive all input from the socket.
*/
#pragma once
#ifndef INPUT_HANDLERS_H
#define INPUT_HANDLERS_H
#include <functional>
#include "mud.h"
#include "socket.h"

/**
* The callback for a yes/no handler.
*
* @param [in] the socket associated with the handler.
* @param [in] true if yes was chosen, false otherwise.
* @param [in] associated args.
*/
typedef std::function<void (Socket*, BOOL, void*)> YESNOCB;
/**
* Line input callback.
*
* @param [in] the associated socket.
* @param [in] the provided line of input.
* @param [in] associated args.
*/
typedef std::function<void (Socket*, std::string&, void*)> LINECB;
typedef std::function<void (Socket*, std::vector<std::string>*, void*)> TEXTBLOCKCB;

/**
* This handler is used to provide a yes/no question to the player.
*/
class YesNoHandler:public InputHandle
{
    YESNOCB _cb;
    void* _arg;
public:
    /**
    * Constructs an input handler.
    *
    * @param [in] cb the callback to fire on comletion.
    * @param [in] args associated args with this handler.
    */
    YesNoHandler(YESNOCB cb, void* arg=NULL);
    /**
    * @copydoc InputHandle::Input
    */
    void Input(void* arg, const std::string &input);
    /**
    * Creates this handle and handles attachment.
    *
    * @param [in] sock the socket associated with the handle.
    * @param [in] cb the callback.
    * @param [in] args associated arguments.
    */
    static BOOL CreateHandler(Socket* sock, YESNOCB cb, void* arg = nullptr);
};

/**
* This handler presents a line input for the player.
*
* Use for requesting a line of input such as a pet name, etc.
*/
class LineHandler:public InputHandle
{
    LINECB _cb;
    void* _arg;
public:
    /**
    * Constructs an input handler.
    *
    * @param [in] cb the callback to fire on comletion.
    * @param [in] args associated args with this handler.
    */
    LineHandler(LINECB cb, void* arg = NULL);
    /**
    * @copydoc InputHandle::Input
    */
    void Input(void* arg, const std::string &input);
    /**
    * Creates this handle and handles attachment.
    *
    * @param [in] sock the socket associated with the handle.
    * @param [in] cb the callback.
    */
    static BOOL CreateHandler(Socket* sock, LINECB cb);
};

/**
* This handler is used for receiving a large block of text or multiple lines.
*/
class TextBlockHandler:public InputHandle
{
    TEXTBLOCKCB _cb;
    std::vector<std::string>* _lines;
    void* _arg;
public:
    /**
    * Constructs an input handler.
    *
    * @param [in] cb the callback to fire on comletion.
    * @param [in] lines a pointer to a vector which will receive the lines from the handler.
    * @param [in] args associated args with this handler.
    */
    TextBlockHandler(TEXTBLOCKCB cb,  std::vector<std::string>* lines, void* arg = NULL);
    /**
    * @copydoc InputHandle::Input
    */
    void Input(void* arg, const std::string &input);
    /**
    * Creates this handle and handles attachment.
    *
    * @param [in] sock the socket associated with the handle.
    * @param [in] cb the callback.
    * @param [in] lines a pointer to the vector that received the input.
    * @param [in] args the args associated with the handler.
    */
    static BOOL CreateHandler(Socket* sock, TEXTBLOCKCB cb, std::vector<std::string>* lines, void* arg = nullptr);
};
#endif
