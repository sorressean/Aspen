#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "mud.h"
#include "conf.h"
#include "socket.h"
#include "server.h"
#include "log.h"
#include "player.h"
#include "calloutManager.h"
#include "modules.h"
#include "zone.h"
#include "components.h"
#include "world.h"

//prototypes:
//recovers from a copyover
static bool CopyoverRecover();
//the main game loop function
static void GameLoop();
//our signal callback
void sig(int sig);
//check directories:
static bool InitializeDirectories();

int main(int argc, const char** argv)
{
    World* world = nullptr;
    BOOL copyover=false; //are we rebooting for copyover?
    int listener=0; //the socket to listen on when recovering from copyover
    int port = 0;

//are we running as root?
    if (getuid() == 0)
        {
            std::cerr << "You should not be running as root, exiting." << std::endl;
            return EXIT_FAILURE;
        }

//this needs to be done before world is forcefully created.
    CalloutManager::Initialize();

//we check to see if dirs exist.
//if we can create them, we do so. Otherwise we just fail when they don't exist.
    if (!InitializeDirectories())
        {
            std::cerr << "Error: could not create directories." << std::endl;
            return EXIT_FAILURE;
        }

    world = World::GetPtr();
    world->WriteLog("Initializing "+std::string(MUD_NAME)+".");

//initialize the server class:
//determine if a port was specified. If not, use default.
    if (argc == 2)
        {
            port = atoi(argv[1]);
            if ((port < 1024)||(port>65535))
                {
                    world->WriteLog("Invalid port specified, program will now exit.", ERR);
                    return EXIT_FAILURE;
                }
        }
    else if (argc == 3)
        {
            if ((!strcmp(argv[1],"-c"))&&(atoi(argv[2])>0))
                {
                    copyover=true;
                    listener=atoi(argv[2]);
                }
        }
    else
        {
            port=DEFAULT_PORT;
        }
    world->InitializeServer();

    if (!world->InitializeFiles())
        {
            world->WriteLog("Could not load mud text files.", ERR);
            return EXIT_FAILURE;
        }

//game initialization calls
    if (!InitializeCommands())
        {
            world->WriteLog("Could not initialize commands.", ERR);
            return EXIT_FAILURE;
        }
    InitializeChannels();
    if (!InitializeModules())
        {
            world->WriteLog("Could not initialize modules.", ERR);
            return EXIT_FAILURE;
        }
    if (!InitializeZones())
        {
            world->WriteLog("could not initialize zones.", ERR);
            return EXIT_FAILURE;
        }

    CreateComponents();
    world->SetRealUptime(time(NULL));
    world->SetCopyoverUptime(0);
    srand(time(NULL));

//make the server listen:
    world->WriteLog("Attempting to establish listening point.");
    if (copyover)
        {
//set the listening socket to the descripter specified
            world->GetServer()->Recover(listener);
//load all saved connections
            if (!CopyoverRecover())
                {
                    return EXIT_FAILURE;
                }
        }
    else
        {
            if (!world->GetServer()->Listen(port))
                {
                    return EXIT_FAILURE;
                }
        }

//initialize signal callbacks
    world->WriteLog("Initializing signal callbacks.");
    signal(SIGTERM,sig);
    signal(SIGINT,sig);
    signal(SIGQUIT,sig);
    signal(SIGHUP,sig);

//load state:
    world->LoadState();

//start the game loop:
    world->WriteLog("Entering game loop.");
    GameLoop();
    CalloutManager::Release();
    world->WriteLog("Game loop finished, exiting.");
    delete world;
    return EXIT_SUCCESS;
}

static bool CopyoverRecover()
{
    World* world = World::GetPtr();
    Player* person = nullptr;
    sockaddr_in* saddr = nullptr;
    FILE* recover = nullptr;
    unsigned short port = 0;
    unsigned long addr = 0;
    char *name=new char[15];
    char* host=new char[256];
    int desc = 0;
    int ruptime = 0;

    world->WriteLog("Starting copyover recovery");
    recover=fopen(COPYOVER_FILE,"rb");
    if (recover == nullptr)
        {
            world->WriteLog("There was an error opening the copyover recovery file, now exiting.", ERR);
            return false;
        }

    fscanf(recover, "%d\n", &ruptime);

    world->SetRealUptime((time_t)ruptime);
    world->SetCopyoverUptime(time(NULL));

    while (1)
        {
            fscanf(recover,"%d %s %hu %lu %s\n",
                   &desc,name, &port,&addr, host);
            if (desc==-1)
                {
                    break;
                }
            Socket* sock=new Socket(desc);
            saddr = sock->GetAddr();
            saddr->sin_family = PF_INET;
            saddr->sin_port=port;
            saddr->sin_addr.s_addr=addr;
            sock->SetHost(host);
            sock->AllocatePlayer();
            world->WriteLog("Loading "+std::string(name)+".");
            person = sock->GetPlayer();
            person->SetName(name);
            person->Load();
            sock->SetConnectionType(CON_Game);
            world->GetServer()->AddSock(sock);
            person->SetSocket(sock);
            person->EnterGame(true);
            sock->Write("Copyover complete.\n");
        }

    delete []name;
    delete []host;
    fclose(recover);
    remove(COPYOVER_FILE);
    world->WriteLog("Copyover completed.");
    return true;
}

static void GameLoop()
{
    World* world = World::GetPtr();
    while (world->IsRunning())
        {
//update our world:
            world->Update();
        }
//the game loop finished, shutdown world.
    world->Shutdown();
    world->Update();
}

void sig(int sig)
{
    World* world = World::GetPtr();
    world->WriteLog("Caught signal, cleaning up.");
    world->SetRunning(false);
}

bool InitializeDirectories()
{
//we shouldn't initialize, but we need to die if the directories don't exist already.
//otherwise we just get errors later.
#ifdef NO_INIT_DEFAULTS
    if (DirectoryExists("../data") && DirectoryExists("../data/players") && DirectoryExists("../data/state") && DirectoryExists("../data/areas"))
        {
            return true;
        }
    if (DirectoryExists("../logs"))
        {
            return true;
        }

    return false;
}
#endif

//we can init defaults.
//we just create the dirs here, if they already exists the calls will fail and noone cares.
mkdir("../data", 0700);
mkdir("../data/players", 0700);
mkdir("../data/state", 0700);
mkdir("../data/areas", 0700);
mkdir("../logs", 0700);
return true;
}
