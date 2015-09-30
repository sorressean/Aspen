/**
* Contains: BaseSocket
* The BaseSocket is a socket class which other classes may inherit.
* It does not contain any mud-specific telnet logic, etc.
*/
#pragma once
#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "mud.h"
#include "conf.h"

/**
* This class is a basic socket.
* It contains the underlying logic for the telnet parser.
*/
class BaseSocket
{
protected:
    int _control;
    sockaddr_in _addr;
    std::string _inBuffer;
    std::string          _outBuffer;
public:
    BaseSocket();
    /**
    * Constructs a basic socket with a descriptor.
    *
    * @param [in] desc The FD to assign to the socket.
    */
    BaseSocket  (int desc );
    virtual ~BaseSocket();

    /**
    * Returns the socket's fd.
    * @return the fd associated with the socket.
    */
    virtual int                  GetControl     ( void ) const;
    /**
    * Reads the pending data and places it in the sockets input buffer.
    *
    * @return True on success, false on failure.
    */
    virtual bool                 Read           ();
    /**
    * Appends the specified to the output buffer.
    *
    * @param [in] txt the data to append to the output buffer.
    */
    virtual void                 Write(const std::string &txt );
    virtual size_t Write(const unsigned char* data);
    /**
    * Flushes the current buffer and writes len bytes, stored in buffer.
    *
    * @param [in] buffer a pointer to the buffer to write.
    * @param [in] count the number of bytes to write.
    * @return The number of bytes written.
    */
    virtual size_t Write(const void* buffer, size_t count);
    /**
    * Flushes the sockets output buffer to the client.
    *
    * @return true on success, false on failure.
    */
    virtual bool                 Flush          ( void );
    /**
    * Returns the input buffer.
    */
    virtual std::string          GetInBuffer    ( void );
    /**
    * Clears the input buffer.
    */
    virtual void                 ClrInBuffer    ( void );
    /**
    * Determines whether there is input pending in the buffer.
    * @return True if input is pending, false otherwise.
    */
    virtual BOOL InputPending() const;
    /**
    * Returns the address of the socket.
    */
    virtual sockaddr_in* GetAddr();
    /**
    * Coppies the address to the socket's address buffer.
    *
    * @param [in] addr a pointer to a sockaddr_in structure.
    */
    virtual void SetAddr(sockaddr_in* addr);
    /**
    * Closes the socket, if one is actually open.
    *
    *@return True if the socket could be closed, false otherwise.
    */
    BOOL Close();
    /**
    * Creates a socket and connects it to the specified address.
    *
    * @param [in] address the address to connect to.
    * @param [in] port the port to connect to.
    * @return True if the socket could be created and connected, false otherwise.
    */
    BOOL Connect(const char* address, unsigned short port);
};
#endif
