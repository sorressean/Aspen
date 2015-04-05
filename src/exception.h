/*
*exception.h
*A basic exception framework that other exceptions can be built off of.
*/
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <stdexcept>
#include "mud.h"
#include "conf.h"

class Exception:public std::exception
{
    std::string _message;
public:
    Exception(const std::string &m):
        _message(m) {}
    ~Exception() throw();
    const std::string GetMessage() const;
    const char* what() const throw();
};

//some aspen-specific exceptions
class VarNotFoundException:public Exception
{
public:
    VarNotFoundException(const std::string &msg):Exception(msg) {}
};
class EventNotFoundException:public Exception
{
public:
    EventNotFoundException(const std::string &msg):Exception(msg) {}
};
#endif
