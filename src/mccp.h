/*
*MCCP-related functions and utilities.
*/
#ifndef MCCP_H
#define MCCP_H
#include "mudh.h"
#include "conf.h"
#include <arpa/telnet.h>
const unsigned char will_compress2_str[] = { IAC, WILL, TELOPT_COMPRESS2, '\0' };
const unsigned char start_compress2_str[] = { IAC, SB, TELOPT_COMPRESS2, IAC, SE, '\0' };
