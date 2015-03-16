#pragma once
#include "mud.h"
#include "conf.h"
#include "event.h"
#include "eventargs.h"

class OptionEventArgs:public EventArgs
{
    unsigned char _option;
    unsigned char _command;
public:
    OptionEventArgs(unsigned char option, unsigned char command) : _option(option), _command(command)
    {
    }
    unsigned char GetOption() const
    {
        return _option;
    }
    unsigned char GetCommand() const
    {
        return _command;
    }
};

class NegotiationEventArgs:public EventArgs
{
    unsigned char _option;
    unsigned char* _data;
    unsigned int _size;
public:
    NegotiationEventArgs(unsigned char option, unsigned char* data, unsigned int size):
        _option(option), _data(data), _size(size)
    {
    }

    unsigned char GetOption() const
    {
        return _option;
    }
    unsigned char* GetData() const
    {
        return _data;
    }
    unsigned int GetSize() const
    {
        return _size;
    }
};
