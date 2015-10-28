/**
* Contains: exception
*
* This is a basic fraemwork that other exceptions can build on.
*/
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <stdexcept>
#include "mud.h"

/**
* Base exception.
*/
class Exception:public std::exception
{
    std::string _message;
public:
    /**
    * Creates an exception object.
    *
    * @param [in] m the message for the exception.
    */
    Exception(const std::string &m):
        _message(m) {}
    ~Exception() throw();
    const std::string GetMessage() const;
    const char* what() const throw();
};

/**
* Used to note that a property was not found.
*/
class VarNotFoundException:public Exception
{
public:
    VarNotFoundException(const std::string &msg):Exception(msg) {}
};

/**
* Used when an event could not be found to call.
*/
class EventNotFoundException:public Exception
{
public:
    EventNotFoundException(const std::string &msg):Exception(msg) {}
};
#endif
