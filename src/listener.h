/*
*Contains: Listeners
*
* Listeners enable external modules to create their own servers.
* This might be needed to implement a webserver, for example.
*/
#ifndef LISTENER_H
#define LISTENER_H
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <list>
#include "mud.h"
#include "eventTable.h"
#include "baseSocket.h"
#include "event.h"

/**
* Listeners are endpoints and servers.
*/
class Listener
{
    /**
    * Accepts any incoming connections.
    */
    void Accept();
    /**
    * Closes a socket.
    *
    * @todo clean this up.
    */
    std::list<BaseSocket*>::iterator CloseSocket(std::list<BaseSocket*>::iterator &it);

    std::list<BaseSocket*> _sockets;
    fd_set fset, rset;
    sockaddr_in _addr;
    int _control;
public:
    EventManager events;

    Listener();
    ~Listener() = default;

    /**
    * Establishes the listener on the specified port.
    *
    * @param [in] port the port to listen for connections on.
    */
    BOOL Listen(int port);
    /**
    * Update the listener.
    */
    void Poll();
};
#endif
