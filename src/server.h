/*
*Main server class
*Handles incoming connections, and passes info to the parser.
*/
#ifndef SERVER_H
#define SERVER_H
#include "mud.h"
#include "conf.h"
#include "socket.h"
#include "serializer.h"
#include "banList.h"
#include "callout.h"

class Player;

struct LinkdeathNode
{
    time_t added;
    Player* mobile;
};

class Server
{
    /*
    *Accepts all incoming connections
    */
    void                 Accept           ();
    /*
    *Closes a specified socket and remove it from the list
    */
    std::list<Socket*>::iterator CloseSocket      (std::list<Socket*>::iterator &it);
    std::list<Socket*>   socketList;
    std::list<LinkdeathNode*> _linkdead;
    int control;
    fd_set               fSet;
    fd_set               rSet;
    sockaddr_in          my_addr;
    timeval       lastSleep;
    BanList* blist;
public:
    Server  ();
    ~Server ();
    /**
    *Establishes a listening point
    *param [in] The port to open as a listening poDWORD.
    *Returns True on success, false on failure.
    */
    BOOL                 Listen          (const int port );
    /*
    *Returns the listening socket
    */
    int GetListener() const;
    /**
    *Flushes all sockets in the list
    *Sends the data in the input buffer (if any) to the socket and clears it.
    */
    void                 FlushSockets     ();
    /*
    *Checks to make sure that no incoming connections need to be accepted, reads and handles data from sockets.
    *returns True on success, false on failure.
    */
    BOOL PollSockets(void );
    /**
    *sleeps for the remainder of the pulses that haven't been used up.
    */
    int Sleep            ( DWORD pps );
    /**
    *returns A list of pointers to the currently connected sockets.
    */
    std::list<Socket*>   GetSocketList    ();
//Sets the specified socket to the listener socket and put it in the descriptor set
    void Recover(int listener);
//Adds the specified socket to the socket list and adds it to the descriptor set:
    void AddSock(Socket* sock);
//Retrieves a pointer to the banlist container object.
    BanList* GetBanList() const;

    /**
    A callout, used to periodically check linkdead players.
    Will disconnect player if they have been linkdead to long.
    */
    void CheckLinkdeaths(Callout* cb);
    bool IsUserLinkdead(const std::string& name) const;
};
#endif
