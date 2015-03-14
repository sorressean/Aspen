#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include "socket.h"
#include "utils.h"
#include "player.h"
#include "world.h"
#include "telnet.h"

InputHandle::InputHandle()
{
    _sock = NULL;
}
InputHandle::~InputHandle()
{
}
void InputHandle::Attach(Socket* sock)
{
    _sock = sock;
}
void InputHandle::Active(in_data* in)
{
}
void InputHandle::Input(void* arg, const std::string &input)
{
}

Socket::Socket(const int desc):
    BaseSocket(desc)
{
    _termtype = "unknown";
    _mobile=NULL;
    _Close=false;
    _input = new std::stack<in_data*>();
    _lastInput = time(NULL);
    _compressing = false;
    _port = -1;
    _totalReceived = 0;
    _totalSent = 0;
    _compressedSent = 0;
    _winsize.width = 80;
    _winsize.height = 23;
}
Socket::~Socket()
{
    in_data* data = NULL;
    if (_mobile)
        {
            delete _mobile;
            _mobile=NULL;
        }

    while (!_input->empty())
        {
            data = _input->top();
            _input->pop();
            if (data->handle)
                {
                    delete data->handle;
                }
            delete data;
        }
    delete _input;
}

void Socket::Linkdeath()
{
    if (_mobile)
        {
            _mobile->SetSocket(NULL);
            _mobile = NULL;
            _Close = true;
        }
}

bool Socket::Read()
{
    char temp[4096 + 2];
    int size = 0;
    int k = 0;
    char* newbuff = nullptr;
    size_t nlpos = 0;
    std::string line;

    while (true)
        {
            size = read(_control, temp, 4096);
            if (size == -1) //error
                {
//these are ok, we just continue on.
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            return true;
                        }
///other errors means we have a problem.
                    else
                        {
                            return false;
                        }
                }
//linkdead
            if (size == 0)
                {
                    return false;
                }
            /*
                  else if (errno == EAGAIN || size == 4096)
                    {
                      break;
                    }
            */
//we have something to read
            _totalReceived += size; //add the number of bytes received to the counter.
            temp[size] = '\0'; //sets the last byte we received to null.
            _parser.Initialize(size);
            _parser.SetBuffer((unsigned char*)temp);
            _parser.Parse();
            newbuff = (char*)_parser.GetFinalBuffer();

            for (k=0; k<size; k++)
                {
                    _inBuffer+=temp[k];
                } //end iteration of buffer
//check for a newline character.
            nlpos = _inBuffer.find_first_of("\r\n");
            if (nlpos != std::string::npos)
                {
                    line=_inBuffer.substr(0, nlpos);
//we have a newline, push it to our command queue.
                    if (line.length())
                        {
                            _cqueue.push(line);
                        }
                    _inBuffer.clear();
                    break;
                }
        }
    _parser.Reset();
    return true;
}

//telnet handler functions
void Socket::OnOption(char option, char command)
{
//mccp2
    if (option == TELNET_WILL && command == TELNET_COMPRESS2)
        {
            if (!InitCompression())
                {
                    Kill();
                }
            Write(TELNET_COMPRESS2_STR);
            Flush();
            _compressing = true;
            return;
        }

//termtype
    if (option == TELNET_WILL && command == TELNET_TERMTYPE)
        {
            Write(TELNET_REQUEST_TERMTYPE);
            return;
        }

    return;
}
void Socket::OnCommand(char command)
{
    return;
}
void Socket::OnNegotiation(char option, const char* buf, int length)
{
//naws
    if (option == TELNET_NAWS)
        {
            unsigned short s = 0;
            char* ptr = (char*)buf;
            if (length != 5)
                {
                    return;
                }

            s = *(unsigned short*)ptr;
            _winsize.width=ntohs(s);
            s += 2;
            s = *(unsigned short*)ptr;
            _winsize.height = ntohs(s);
            return;
        }

//termtype
    if (option == TELNET_TERMTYPE && length >2 && buf[0] == TELNET_IS)
        {
            int i = 1;
            int curlength = length - 1;
            _termtype.clear();
            for (i = 1; i < curlength; ++i)
                {
                    if (!isprint(buf[i]))
                        {
                            _termtype = "unknown";
                            return;
                        }
                    _termtype += buf[i];
                }
            return;
        }

    return;
}

BOOL Socket::InitCompression()
{
    int ret = 0;

    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;
    ret = deflateInit(&zstream, 9); //used Z_DEFAULT_COMPRESSION before.
    if (ret != Z_OK)
        {
            return false;
        }
    return true;
}

bool Socket::Flush()
{
    int b=0;
    int w=0;
    int status = Z_OK;
    int length = 0;
    int i = 0;
    World* world = World::GetPtr();

    if (!_outBuffer.length())
        {
            return true;
        }
//prepend buffer to prompt
    _totalSent += _outBuffer.length();
    if ((_mobile!=NULL)&&(_con==ConnectionType::Game))
        {
            _outBuffer+="\r\n"+world->BuildPrompt(_mobile->GetPrompt(), _mobile)+(char)TELNET_IAC+(char)TELNET_GA;
        }

    if (!_compressing)
        {
            //we are not compressing outbound data.
            while (_outBuffer.length() > 0)
                {
                    b = _outBuffer.length();
                    // any write failures ?
                    if (_control!=-1)
                        {
                            if ((w = send(_control, _outBuffer.c_str(), b, 0)) == -1)
                                {
                                    return false;
                                }
                        }
                    // move the buffer down
                    _outBuffer.erase(0, w);
                }
        } //end sending raw data
    else
        {
            //we are compressing, wheee!
            unsigned char* buff= new unsigned char[_outBuffer.length()];

            zstream.avail_in = _outBuffer.length();
            zstream.next_in = (unsigned char*)const_cast<char*>(_outBuffer.c_str());
            zstream.next_out = buff;

            while (zstream.avail_in)
                {
                    zstream.avail_out = _outBuffer.length() -(zstream.next_out-buff);
                    if (zstream.avail_out)
                        {
                            status = deflate(&zstream, Z_SYNC_FLUSH);
                            if (status != Z_OK)
                                {
                                    delete []buff;
                                    return false;
                                }
                        }
                }
            length = zstream.next_out-buff;
            if (length)
                {
                    _compressedSent += length;
                    b = 0;
                    for (i = 0; i < length; i +=b)
                        {
                            w = Min<int>(length-i, 4096);
                            b = send(_control, buff+i, w, 0);
                        }
                }
            _outBuffer.clear();
            delete [] buff;
        }

    return true;
}
std::string Socket::GetInBuffer()
{
//we need to clean up extra junk at the end
    if (_inBuffer=="")
        {
            return "";
        }
    return _inBuffer.substr(0,_inBuffer.find_first_of("\n\r"));
}

ConnectionType Socket::GetConnectionType() const
{
    return _con;
}
void Socket::SetConnectionType(const ConnectionType &s)
{
    _con=s;
}

std::string Socket::GetHost() const
{
    return _host;
}
void Socket::SetHost(const std::string &host)
{
    _host = host;
}

void Socket::AllocatePlayer()
{
    if (!_mobile)
        {
            _mobile=new Player();
        }
}
Player* Socket::GetPlayer() const
{
    return _mobile;
}

void Socket::Kill()
{
    if (!_Close)
        {
            if (_mobile)
                {
                    if (GetConnectionType()==ConnectionType::Game)
                        {
                            _mobile->LeaveGame();
                        }
                }
            _Close=true;
        }
}

short Socket::GetPort() const
{
    return _port;
}
void Socket::SetPort(short port)
{
    _port = port;
}

unsigned int Socket::GetTotalReceived() const
{
    return _totalReceived;
}
unsigned int Socket::GetTotalSent() const
{
    return _totalSent;
}
unsigned int Socket::GetTotalCompressedSent() const
{
    return _compressedSent;
}
short Socket::GetWindowWidth() const
{
    return _winsize.width;
}
short Socket::GetWindowHeight() const
{
    return _winsize.height;
}
std::string Socket::GetTermtype() const
{
    return _termtype;
}
BOOL Socket::IsCompressing() const
{
    return _compressing;
}

BOOL Socket::ShouldClose()
{
    return (_Close? true : false);
}

BOOL Socket::HasHandle() const
{
    return (_input->empty()==true?false:true);
}
BOOL Socket::HandleInput()
{
    if (HasHandle())
        {
            in_data* data = _input->top();
            data->handle->Input(_input->top()->args, PopCommand());
            ClrInBuffer();
            return true;
        }

    return false;
}
void Socket::ClearInput()
{
    if (HasHandle())
        {
            in_data* in = _input->top();
            _input->pop();
            if (in)
                {
                    if (in->handle)
                        {
                            delete in->handle;
                        }
                    delete in;
                }
            if (HasHandle())
                {
                    in = _input->top();
                    in->handle->Active(in);
                }
        }
}
BOOL Socket::SetInput(in_data* data)
{
    if (data->handle)
        {
            data->handle->Attach(this);
        }
    _input->push(data);
    data->handle->Active(data);
    ClrInBuffer();
    return true;
}

void Socket::UpdateLastCommand()
{
    _lastInput = time(NULL);
}
time_t Socket::GetLastCommand()
{
    return _lastInput;
}
Player* Socket::GetMobile()
{
    return _mobile;
}
BOOL Socket::CommandPending() const
{
    return (_cqueue.size()==0?false:true);
}
std::string Socket::PopCommand()
{
    if (!CommandPending())
        {
            return "";
        }
    std::string ret = _cqueue.front();
    _cqueue.pop();

    return ret;
}
void Socket::AddCommand(const std::string &input)
{
    _cqueue.push(input);
}

bool Socket::HandleGameInput()
{
    World* world = World::GetPtr();
    Player* mob = GetPlayer();
    std::string input;

//check to see if an input handler was associated with the socket before we pass to command parsing
    if (HasHandle())
        {
            HandleInput();
            return true;
        }

//No handle was found, pass on to command parsing
    input=PopCommand();
    if (!input.length())
        {
            return true;
        }
    if (!world->DoCommand(mob, input))
        {
            mob->Message(MSG_ERROR, "I did not understand that.");
            return false;
        }

    return true;
}
bool Socket::HandleNameInput()
{
    Player* mob = nullptr;
    std::string input;
    World* world = World::GetPtr();
    Server* server = world->GetServer();
    input = PopCommand();

//new player:
    if (input=="new")
        {
            Write("Welcome, what name would you like?\n");
            AllocatePlayer();
            mob = GetPlayer();
            SetConnectionType(ConnectionType::Newname);
            return true;
        }

//checks to see if the username is valid
    if (!IsValidUserName(input))
        {
            Write("Invalid name, try again.\n");
            return true;
        }

//check to see if the player exists
    if (!PlayerExists(input))
        {
            Write("That player doesn't exist.\nWhat is your name? Type new for a new character.\n");
            return true;
        }
    mob = server->GetLinkdeadUser(input);
    if (mob)
        {
            _mobile = mob;
            Write(TELNET_ECHO_OFF);
            Write("\n");
            Write("Password?\n");
            SetConnectionType(ConnectionType::Password);
            return true;
        }

    AllocatePlayer();
    mob = GetPlayer();
//set the players name to the one specified and try to load the file.
    mob->SetName(input);
    mob->Load();

    Write(TELNET_ECHO_OFF);
    Write("\n");
    Write("Password?\n");
    SetConnectionType(ConnectionType::Password);
    return true;
}
bool Socket::HandlePasswordInput()
{
    World* world = World::GetPtr();
    Server* server = world->GetServer();
    Player* mobile = GetPlayer();
    std::string input;

    input = PopCommand();
    mobile->SetTempPassword(input);

    if (!mobile->ComparePassword())
        {
            Write("That password isn't valid!\n");
            mobile->IncInvalidPassword();
            return false;
        }

    Write(TELNET_ECHO_ON);
    SetConnectionType(ConnectionType::Game);
    mobile->SetSocket(this);
    if (server->GetLinkdeadUser(mobile->GetName()))
        {
            server->RemoveLinkdeadUser(mobile->GetName());
            Write("Reconnected.\n");
            return true;
        }

    mobile->SetLastLogin((UINT)time(NULL));
    mobile->EnterGame(false);
    return true;
}
bool Socket::HandleNewnameInput()
{
    std::string input;
    Player* mobile = GetPlayer();
    input = PopCommand();

    if (!IsValidUserName(input))
        {
            Write("That is not a valid username. Usernames must contain 4-12 characters.\nWhat name would you like?\n");
            return true;
        }
    if (PlayerExists(input))
        {
            Write("That player already exists, please try again.\nWhat name would you like?\n");
            return true;
        }

    Write("Please choose a password. Please make your password between 5 and 20 characters long.\nStrong passwords contain both lower and uppercase letters, numbers, letters and a dash ('-').\nEnter your new password?\n");
    Write(TELNET_ECHO_OFF);
//name was valid, set it in the player class.
    mobile->SetName(input);
    SetConnectionType(ConnectionType::Newpass);
    return true;
}
bool Socket::HandleNewpassInput()
{
    std::string input;
    Player* mob = GetPlayer();

    input=PopCommand();
    if (!IsValidPassword(input))
        {
            Write("That password isn't valid, please try again.\n");
            return true;
        }

//transfer control to password verification
    Write("Please re-enter your password for varification.\n");
    mob->SetPassword(input);
    SetConnectionType(ConnectionType::Verpass);

    return true;
}
bool Socket::HandleVerpassInput()
{
    Player* mob = GetPlayer();
    std::string input = PopCommand();

    if (!IsValidPassword(input))
        {
            Write("That password isn't valid, please try again.\n");
            return true;
        }

    mob->SetTempPassword(input);
//passwords  did not match, transfer control back to new password.
    if (!mob->ComparePassword())
        {
            Write("That password isn't valid, please try again.\n");
            SetConnectionType(ConnectionType::Newpass);
            return true;
        }

    Write(TELNET_ECHO_OFF);
    Write("What is your gender? Please enter male or female.\n");
    SetConnectionType(ConnectionType::Gender);

    return true;
}
bool Socket::HandleGenderInput()
{
    bool gf = false;
    Player* mob = GetPlayer();
    std::string input = PopCommand();
    Lower(input);

    if (input.length() >= 1)
        {
            if (input[0] == 'm')
                {
                    mob->SetGender(Gender::Male);
                    gf = true;
                }
            if (input[0] == 'f')
                {
                    mob->SetGender(Gender::Female);
                    gf = true;
                }
        }

    if (gf)
        {
            InitializeNewPlayer();
        }

    return true;
}

void Socket::InitializeNewPlayer()
{
    Player* mob = GetPlayer();
    World* world = World::GetPtr();

    mob->Initialize();
//passwords matched, see if the player is the first user. If so, make it a god.
    if (IsFirstUser())
        {
            mob->SetRank(RANK_PLAYER|RANK_PLAYTESTER|RANK_NEWBIEHELPER|RANK_BUILDER|RANK_ADMIN|RANK_GOD);
            Write("You are the first player to create, rank set to God.\n");
        }

    mob->SetFirstLogin((UINT)time(NULL));
//Set the connection type to game and enter the player.
    SetConnectionType(ConnectionType::Game);
    mob->SetSocket(this);
//call the Create event:
    mob->EnterGame(false);
    world->events.CallEvent("PlayerCreated", NULL, (void*)mob);
}

BOOL Socket::HandleCommand()
{
    switch (GetConnectionType())
        {
//handles all in-game connections that aren't handled with a function
        case ConnectionType::Disconnected:
        case ConnectionType::Game:
            return HandleGameInput();
        case ConnectionType::Name:
            return HandleNameInput();
//login password prompt
        case ConnectionType::Password:
            return HandlePasswordInput();
//login new username
        case ConnectionType::Newname:
            return HandleNewnameInput();
        //login new password
        case ConnectionType::Newpass:
            return HandleNewpassInput();
//login verify password
        case ConnectionType::Verpass:
            return HandleVerpassInput();
        case ConnectionType::Gender:
            return HandleGenderInput();
        default:
            return false;
        }

    return true;
}
