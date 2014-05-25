#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <iostream>
#include "mud.h"
#include "conf.h"
#include "socket.h"
#include "server.h"
#include "log.h"
#include "player.h"
#include "calloutManager.h"
#include "modules.h"
#ifdef MODULE_SCRIPTING
#include "scripts/scripts.h"
#endif
#include "zone.h"
#include "components.h"
#include "constants.h"
#ifdef OLC
#include "olc.h"
#endif
#include "socials.h"
#include "world.h"

//prototypes:
//recovers from a copyover
static void CopyoverRecover();
//the main game loop function
static void GameLoop();
//our signal callback
void sig(int sig);
//check directories:
static bool InitializeDirectories();

int main(int argc, const char** argv)
{
    World* world = NULL;
    BOOL copyover=false; //are we rebooting for copyover?
    int listener=0; //the socket to listen on when recovering from copyover
    int port = 0;

    if (getuid() == 0)
        {
            std::cerr << "You should not be running as root, exiting." << std::endl;
            return EXIT_FAILURE;
        }

//this needs to be done before world is forcefully created.
    CalloutManager::Initialize();

#ifdef ELAPSED_STARTUP
    timeval start, end;
    std::stringstream st;
    float elapsed;

//we start the counter for elapsed here:
    gettimeofday(&start, NULL);
#endif

    if (!InitializeDirectories())
        {
            std::cerr << "Error: could not create directories." << std::endl;
            return EXIT_FAILURE;
        }

    InitializeConstants();
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
#ifdef MODULE_SCRIPTING
    if (!InitializeScript())
        {
            world->WriteLog("Could not initialize scripting.", ERR);
            return EXIT_FAILURE;
        }
#endif

#ifdef OLC
    if(!InitializeOlc())
        {
            world->WriteLog("Could not initialize OLC.", ERR);
            return EXIT_FAILURE;
        }
#endif

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

    if (!InitializeSocials())
        {
            world->WriteLog("Could not initialize the socials system.", ERR);
            return EXIT_FAILURE;
        }

    CreateComponents();
    world->SetRealUptime(time(NULL));
    world->SetCopyoverUptime(time(NULL));
    srand(time(NULL));

//make the server listen:
    world->WriteLog("Attempting to establish listening point.");
    if (copyover)
        {
//set the listening socket to the descripter specified
            world->GetServer()->Recover(listener);
//load all saved connections
            CopyoverRecover();
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

#ifdef ELAPSED_STARTUP
//and we're finished
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) *1000;
    elapsed += (float)(end.tv_usec-start.tv_usec)/1000;
    st << "Startup elapsed time: " << elapsed << " ms." << std::endl;
    world->WriteLog(st.str());
#endif
//start the game loop:
    world->WriteLog("Entering game loop.");
    GameLoop();
    CalloutManager::Release();
    world->WriteLog("Game loop finished, exiting.");
    delete world;
    return 0;
}

static void CopyoverRecover()
{
    World* world = World::GetPtr();

    Player* person = NULL;
    sockaddr_in* saddr = NULL;
    FILE* recover = NULL;
    const short family = PF_INET;
    unsigned short port = 0;
    unsigned long addr = 0;
    char *name=new char[15];
    char* host=new char[256];
    int desc = 0;
    int cuptime = 0;
    int ruptime = 0;

    world->WriteLog("Starting copyover recovery");
    recover=fopen(COPYOVER_FILE,"rb");
    if (recover==NULL)
        {
            world->WriteLog("There was an error opening the copyover recovery file, now exiting.", ERR);
            exit(EXIT_FAILURE);
        }

    fscanf(recover, "%d %d\n", &cuptime, &ruptime);

    world->SetRealUptime((time_t)ruptime);
    world->SetCopyoverUptime((time_t)cuptime);

    while (1)
        {
            memset(name, 0, 15);
            memset(host, 0, 256);
            fscanf(recover,"%d %s %hu %lu %s\n",
                   &desc,name, &port,&addr, host);
            if (desc==-1)
                {
                    break;
                }
            Socket* sock=new Socket(desc);
            saddr = sock->GetAddr();
            saddr->sin_family=family;
            saddr->sin_port=port;
            saddr->sin_addr.s_addr=addr;
            sock->SetHost(host);
            sock->AllocatePlayer();
            world->WriteLog("Loading "+std::string(name)+".");
            person = sock->GetPlayer();
            person->SetName(name);
            person->Load();
            sock->SetConnectionType(ConnectionType::Game);
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
    if (DirectoryExists("data") && DirectoryExists("data/players") && DirectoryExists("data/state"))
        {
            return true;
        }
    if (DirectoryExists("logs"))
        {
            return true;
        }

    return false;
}
#endif

//we can init defaults.
//we just create the dirs here, if they already exists the calls will fail and noone cares.
mkdir("data", 0);
mkdir("data/players", 0);
mkdir("data/state", 0);

return true;
}
