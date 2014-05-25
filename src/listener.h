/*
*This class is used so that external modules can create their own listeners.
*When we receive data, we will pass it on to the listener to handle.
*/
#ifndef LISTENER_H
#define LISTENER_H
#include <list>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include "mud.h"
#include "conf.h"
#include "eventManager.h"
#include "baseSocket.h"
#include "event.h"

class Listener
{
    void Accept();
    std::list<BaseSocket*>::iterator CloseSocket(std::list<BaseSocket*>::iterator &it);
    std::list<BaseSocket*> _sockets;
    fd_set fset, rset;
    sockaddr_in _addr;
    int _control;
public:
    EventManager events;

    Listener();
    ~Listener();
    BOOL Listen(int port);
    void Poll();
    void ReceiveText();
};
#endif
