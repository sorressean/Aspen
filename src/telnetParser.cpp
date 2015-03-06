#include <string>
#include "mud.h"
#include "conf.h"
#include "telnetParser.h"
#include "telnetParserEvents.hpp"
#include "telnet.h"
#include "eventManager.h"
#include "event.h"

TelnetParser::TelnetParser()
{
    Reset();
    events.RegisterEvent("OnNegotiation", new Event());
    events.RegisterEvent("OnOption", new Event());
}
TelnetParser::~TelnetParser()
{
    if (_newbuff != nullptr)
        {
            delete []_newbuf;
        }
}

void TelnetParser::SetBuffer(unsigned char* buff)
{
    _buff = buff;
}
unsigned char* TelnetParser::GetFinalBuffer() const
{
    return _newbuff;
}
void TelnetParser::Parse()
{
    if (_buff == nullptr)
        {
            return;
        }

    unsigned char* r = _buff;
    unsigned char* w = _newbuff;
    unsigned char cur;
    unsigned char option = 0;
    unsigned char* start = nullptr;
    unsigned char* end = nullptr;

    for (; *r; ++r)
        {
            cur = *r;
//possible: iac iac or iac option.
            if (cur == TELNET_IAC)
                {
//iac escape (iac iac)
                    if (_state == TELNET_IAC)
                        {
                            *r = TELNET_IAC;
                            r++;
                        }
//beginning of sequence or end of negotiation
                    if (_state == 0 || _state == TELNET_SB)
                        {
                            _state = TELNET_IAC;
                        }
                }
//option negotiation
            else if (cur == TELNET_DO || cur == TELNET_DONT || cur == TELNET_WILL || cur == TELNET_WONT)
                {
                    if (_state == TELNET_IAC)
                        {
                            _state = cur;
                        }
                }
//option
//we've already seen will|wont|do|dont.
            else if (_state == TELNET_WILL || _state == TELNET_WONT || _state == TELNET_DO || _state == TELNET_DONT)
                {
                    OptionEventArgs args(cur);
                    events.CallEvent("OnOption", &args, static_cast<void*>(this));
                    _state = 0;
                }
//sequence begin
            else if(cur == TELNET_SB)
                {
                    if (_state == TELNET_IAC)
                        {
                            _state = TELNET_SB;
                        }
                }
            else if (_state == TELNET_SB && !option)
                {
                    option = cur;
                    start = r;
                    for (end = start; *end; ++end)
                        {
                            if (*end == TELNET_SE)
                                {
                                    _state = TELNET_SE;
                                    break;
                                }
                        }
                    start++; //move it past option.
                    end--; //move it to the last bite of the sequence.
                }
//sequence end.
            else if(cur == TELNET_SE)
                {
                    if (_state == TELNET_IAC && start != end && option)
                        {
                            NegotiationEventArgs args(option, start, (unsigned int)(end-start));
                            events.CallEvent("OnNegotiation", &args, static_cast<void*>(this));
                            _state = 0;
                            option = 0;
                            start = end = nullptr;
                        }
                }
            else
                {
                    *w = cur;
                    w++;
                }
        }
}

void TelnetParser::Reset()
{
    _state = 0;
    _buff = nullptr;
    if (_newbuff)
        {
            delete []_newbuff;
            _newbuff = nullptr;
        }
}
void TelnetParser::Initialize(unsigned int size)
{
    if (_newbuff)
        {
            delete []_newbuff;
        }
    _newbuff = new unsigned char[size];
}
