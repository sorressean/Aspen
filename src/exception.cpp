#include "exception.h"

Exception::~Exception() throw()
{
}

const std::string Exception::GetMessage() const
{
    return _message;
}

const char* Exception::what() const throw()
{
    return _message.c_str();
}
