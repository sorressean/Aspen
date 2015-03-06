#pragma once
#include "mud.h"
#include "conf.h"
#include "eventManager.h"

class TelnetParser
{
    unsigned char _state;
    unsigned char* _buff;
    unsigned char* _newbuff;
public:
    EventManager events;
    TelnetParser();
    ~TelnetParser();
    void SetBuffer(unsigned char* buff);
    unsigned char* GetFinalBuffer() const;
    void Parse();
    void Reset();
    void Initialize(unsigned int size);
};
