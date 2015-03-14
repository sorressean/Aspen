#pragma once
#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H
#include "mud.h"
#include "conf.h"
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

class BaseSocket
{
protected:
    int _control;
    sockaddr_in          *_addr;
    std::string _inBuffer;
    std::string          _outBuffer;
public:
    BaseSocket();
    /*
    *Constructer
    *Param: [in] the FD to assign to the socket.
    */
    BaseSocket  (const int desc );
    virtual ~BaseSocket();
    /*
    *Returns the "control" fd.
    *Return: the fd associated with the socket.
    */
    virtual int                  GetControl     ( void ) const;
    /*
    *Reads the pending data and places it in the sockets input buffer.
    *Return: True on success, false on failure.
    */
    virtual bool                 Read           ();
    /*
    *Appends the text to the output buffer.
    *Param: [in] the data to append to the output buffer.
    */
    virtual void                 Write(const std::string &txt );
    virtual size_t Write(const unsigned char* data);
    /*
    *Flushes the current buffer and writes len bytes, stored in buffer.
    *Param: [in] a pointer to the buffer to write.
    [in] the number of bytes to write.
    [Return: The number of bytes written.
    */
    virtual size_t Write(const void* buffer, size_t count);
    /*
    *Flushes the sockets output buffer to the client.
    *Return: true on success, false on failure.
    */
    virtual bool                 Flush          ( void );
    /*
    *Returns the input buffer.
    *Return: The input buffer of the socket.
    */
    virtual std::string          GetInBuffer    ( void );
    /*
    *Clears the input buffer.
    */
    virtual void                 ClrInBuffer    ( void );
    virtual BOOL InputPending() const;
    /*
    *Returns the address of the socket.
    *Return: The socket's addr struct.
    */
    virtual sockaddr_in* GetAddr() const;
    /*
    *coppies the address to the socket's address buffer.
    *Param: [in] a pointer to a sockaddr_in structure.
    */
    virtual void SetAddr(sockaddr_in* addr);
    /*
    *Closes the socket, if one is actually open.
    *Return: True if the socket could be closed, false otherwise.
    */
    BOOL Close();
    /*
    *Creates a socket and connects it to the specified address.
    *Param: [in] the address to connect to.
    *Param: [in] the port to connect to.
    *Return: True if the socket could be created and connected, false otherwise.
    */
    BOOL Connect(const char* address, unsigned short port);
};
#endif
