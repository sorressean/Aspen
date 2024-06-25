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
#include "configuration.h"
#include "ChannelManager.h"
#include "socket.h"
#include "server.h"
#include "log.h"
#include "player.h"
#include "calloutManager.h"
#include "modules.h"
#include "zone.h"
#include "world.h"

using namespace std;

//prototypes:
//recovers from a copyover
static bool CopyoverRecover();
//the main game loop function
static void GameLoop();
//our signal callback
void sig(int sig);
//check directories:
static bool InitializeDirectories();

static void InitializeSignalCallbacks()
{
    WriteLog("Initializing signal callbacks.");
    signal(SIGTERM,sig);
    signal(SIGINT,sig);
    signal(SIGQUIT,sig);
    signal(SIGHUP,sig);
}

int main(int argc, const char** argv)
{
//are we running as root?
    if (getuid() == 0)
        {
            cerr << "You should not be running as root, exiting." << endl;
            return EXIT_FAILURE;
        }

    Log::Initialize();
//load and read configuration
    Configuration::Initialize();
    Configuration::GetPtr()->Load();

//this needs to be done before world is forcefully created.
    CalloutManager::Initialize();

//we check to see if dirs exist.
//if we can create them, we do so. Otherwise we just fail when they don't exist.
    if (!InitializeDirectories())
        {
            cerr << "Error: could not create directories." << endl;
            return EXIT_FAILURE;
        }

    auto world = World::GetPtr();
    WriteLog("Initializing "+string(MUD_NAME)+".");

    int port = 0;
    int listener = 0;
    bool copyover = false;
    if (argc == 2)
        {
            port = atoi(argv[1]);
            if ((port < 1024)||(port>65535))
                {
                    WriteLog(SeverityLevel::Fatal, "Invalid port specified, program will now exit.");
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
            WriteLog(SeverityLevel::Fatal, "Could not load mud text files.");
            return EXIT_FAILURE;
        }

//game initialization calls
    if (!InitializeCommands())
        {
            WriteLog(SeverityLevel::Fatal, "Could not initialize commands.");
            return EXIT_FAILURE;
        }
    ChannelManager::Initialize();
    InitializeChannels();
    if (!InitializeModules())
        {
            WriteLog(SeverityLevel::Fatal, "Could not initialize modules.");
            return EXIT_FAILURE;
        }
    if (!InitializeZones())
        {
            WriteLog(SeverityLevel::Fatal, "could not initialize zones.");
            return EXIT_FAILURE;
        }
    world->SetRealUptime(time(NULL));
    world->SetCopyoverUptime(0);
    srand(time(NULL));

//make the server listen:
    WriteLog("Attempting to establish listening point.");
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

    InitializeSignalCallbacks();
//load state:
    world->LoadState();

//start the game loop:
    WriteLog("Entering game loop.");
    GameLoop();
    CalloutManager::Release();
    Configuration::GetPtr()->Save();
    Configuration::Release();
    ChannelManager::Release();
    WriteLog("Game loop finished, exiting.");
    delete world;
    return EXIT_SUCCESS;
}

static bool CopyoverRecover()
{
    World* world = World::GetPtr();
    Player* person = nullptr;
    sockaddr_in* saddr = nullptr;
    unsigned short port = 0;
    unsigned long addr = 0;
    int desc = 0;

    WriteLog("Starting copyover recovery");
    FILE* recover=fopen(COPYOVER_FILE,"rb");
    if (recover == nullptr)
        {
            WriteLog(SeverityLevel::Fatal, "There was an error opening the copyover recovery file, now exiting.");
            return false;
        }

    int ruptime = 0;
    fscanf(recover, "%d\n", &ruptime);
    world->SetRealUptime((time_t)ruptime);
    world->SetCopyoverUptime(time(NULL));

    while (1)
        {

            char host[256];
            char name[15];

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
            WriteLog("Loading "+std::string(name)+".");
            person = sock->GetPlayer();
            person->SetName(name);
            person->Load();
            sock->SetConnectionType(CON_Game);
            world->GetServer()->AddSock(sock);
            person->SetSocket(sock);
            person->EnterGame(true);
            sock->Write("Copyover complete.\n");
        }

    fclose(recover);
    remove(COPYOVER_FILE);
    WriteLog("Copyover completed.");
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
    WriteLog("Caught signal, cleaning up.");
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
