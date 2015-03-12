#pragma once
#ifndef TELNET_H
#define TELNET_H
#include "conf.h"
#include "mud.h"
#define TELNET_IAC 0xFF
#define TELNET_WILL 0xFB
#define TELNET_WONT 0xFC
#define TELNET_DO 0xFD
#define TELNET_DONT 0xFE
#define TELNET_SB 0xFA
#define TELNET_SE 0xF0
#define TELNET_GA 0xF9
//options
#define TELNET_ECHO 0x01
#define TELNET_NOP 0xF1
#define TELNET_AYT 0xF6
#define  TELNET_TERMTYPE      0x18
#define TELNET_NAWS 0x1f
#define TELNET_COMPRESS2 0x56
#define TELNET_SEND 0x01
#define TELNET_IS 0x0

const unsigned char TELNET_COMPRESS2_STR []= {TELNET_IAC,TELNET_SB,TELNET_COMPRESS2,TELNET_IAC,TELNET_SE, '\0'};
const unsigned char TELNET_ECHO_OFF[] = {TELNET_IAC, TELNET_WILL, TELNET_ECHO, '\0'};
const unsigned char TELNET_ECHO_ON[] = {TELNET_IAC, TELNET_WONT, TELNET_ECHO, '\0'};
const unsigned char TELNET_DO_TERMTYPE[]= {TELNET_IAC, TELNET_DO, TELNET_TERMTYPE, '\0'};
const unsigned char TELNET_DO_COMPRESS2[] = {TELNET_IAC, TELNET_DO, TELNET_COMPRESS2, '\0'};
const unsigned char TELNET_DO_NAWS[] = {TELNET_IAC, TELNET_DO, TELNET_NAWS, '\0'};
const unsigned char TELNET_REQUEST_TERMTYPE []= {TELNET_IAC, TELNET_SB, TELNET_TERMTYPE, TELNET_SEND, TELNET_IAC, TELNET_SE, '\0'};

#endif
