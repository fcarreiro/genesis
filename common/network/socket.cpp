#ifdef WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <cstdlib>
#include <string>
#include <sdl/sdl.h>
#include "socket.h"

//////////////////////////////////////////////////////////////////////////
// CSocket constructor & destructor
//////////////////////////////////////////////////////////////////////////

CSocket::CSocket() : m_iSocket(0),
m_bConnected(false), m_bListening(false),
m_dwTick(0)
{
	// create new socket
	m_iSocket = Create();
}

CSocket::CSocket(SOCKET former_sock) : m_iSocket(former_sock),
m_bConnected(false), m_bListening(false),
m_dwTick(0)
{
	// initialize socket with precreated os socket
}

CSocket::~CSocket()
{
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates a new socket and returns it if possible
//////////////////////////////////////////////////////////////////////////

SOCKET CSocket::Create()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return (s == INVALID_SOCKET) ? 0 : s;
}

//////////////////////////////////////////////////////////////////////////
// Connect() : Connects socket to a host
//////////////////////////////////////////////////////////////////////////

bool CSocket::Connect(const std::string & address, unsigned short port)
{
	// you have to disconnect first
	if(m_bConnected || m_bListening) return false;

	// try again
	if(!m_iSocket)
	{
		// create a socket if sth strange had happened
		m_iSocket = Create();
		if(!m_iSocket) return false;
	}

	// get host structure
	LPHOSTENT hostEntry = gethostbyname(address.c_str());
	if(!hostEntry) return false;

	// fill socket_address with host info
	SOCKADDR_IN hostInfo;
	hostInfo.sin_family = AF_INET;
	hostInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	hostInfo.sin_port = htons(port);

	// connect
	int nRet = connect(m_iSocket, (sockaddr *)&hostInfo, sizeof(struct sockaddr));
	if(nRet == SOCKET_ERROR) return false;

	// we are connected now, set internal variables
	m_bConnected = true;
	m_dwTick = SDL_GetTicks();
	m_strHostName = address;
	m_strHostIP = inet_ntoa(hostInfo.sin_addr);
}

//////////////////////////////////////////////////////////////////////////
// Disconnect() : Disconnects the socket
//////////////////////////////////////////////////////////////////////////

void CSocket::Disconnect()
{
	// close and destroy socket
	closesocket(m_iSocket);

	// reset variables
	m_iSocket = 0;
	m_dwTick = 0;
	m_bConnected = false;
	m_bListening = false;
	m_strHostName = "";
	m_strHostIP = "";
}

//////////////////////////////////////////////////////////////////////////
// Listen() : Listens for incoming connections
//////////////////////////////////////////////////////////////////////////

bool CSocket::Listen(unsigned short port, int queue)
{
	int nRet;

	// you have to disconnect first
	if(m_bConnected || m_bListening) return false;

	// try again
	if(!m_iSocket)
	{
		// create a socket if sth strange had happened
		m_iSocket = Create();
		if(!m_iSocket) return false;
	}

	// setup binding
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htons(INADDR_ANY);

	// bind to local socket
	nRet = bind(m_iSocket, (struct sockaddr *) &addr, sizeof(addr));
	if(nRet == SOCKET_ERROR) return false;

	// start listening
	nRet = listen(m_iSocket, queue);
	if(nRet == SOCKET_ERROR)
	{
		closesocket(m_iSocket);
		return false;
	}

	// listening, update local vars
	m_bListening = true;
	m_dwTick = SDL_GetTicks();
}

//////////////////////////////////////////////////////////////////////////
// Send() : Send data through the socket
//////////////////////////////////////////////////////////////////////////

int CSocket::Send(const char *buf, int len)
{
	int ret = send(m_iSocket, buf, len, 0);
	if(ret) m_dwTick = SDL_GetTicks();
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Recieve() : Recieves data from the other side
//////////////////////////////////////////////////////////////////////////

int CSocket::Recieve(char *buf, int len)
{
	int ret = recv(m_iSocket, buf, len, 0);
	if(ret) m_dwTick = SDL_GetTicks();
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Accept() : Accepts incoming connection and returns new socket object
//////////////////////////////////////////////////////////////////////////

CSocket *CSocket::Accept() const
{
	SOCKET s;
	CSocket *obj;

	// accept new connection
	s = accept(m_iSocket, NULL, NULL);

	// if we have a connetion create a new socket and return it
	if(s != INVALID_SOCKET)
	{
		// create and check
		obj = new CSocket(s);
		if(obj) return obj;
	}

	// error
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// IncomingConnection() : Returns true if there is an incoming connection
//////////////////////////////////////////////////////////////////////////

bool CSocket::IncomingConnection() const
{
	// if there is incoming data and we are listening then there
	// is an incoming connection
	return (m_bListening && IncomingData());
}

//////////////////////////////////////////////////////////////////////////
// IncomingData() : Returns true if there is incoming data
//////////////////////////////////////////////////////////////////////////

bool CSocket::IncomingData() const
{
	// we need a purpose
	if(!m_bConnected && !m_bListening) return false;

	// variables for select
	int selectreturn;
	static struct timeval waitcycle;
	static fd_set insocket;

	// reset descriptors & add socket
	FD_ZERO(&insocket);
	FD_SET(m_iSocket, &insocket);

	// setup wait timer struct
	waitcycle.tv_sec = 0;
	waitcycle.tv_usec = 50;

	// do select
	selectreturn = select(m_iSocket + 1, &insocket, NULL,
						  NULL, &waitcycle);

	// find activated descriptors
	if(selectreturn < 0) return false;
	else if(FD_ISSET(m_iSocket, &insocket)) return true;

	// deactivated
	return false;
}

//////////////////////////////////////////////////////////////////////////
// IsError() : Returns true if there was some kind of error
//////////////////////////////////////////////////////////////////////////

bool CSocket::IsError() const
{
	// we need a purpose
	if(!m_bConnected && !m_bListening) return true;

	// variables for select
	int selectreturn;
	static struct timeval waitcycle;
	static fd_set errsocket;

	// reset descriptors & add socket
	FD_ZERO(&errsocket);
	FD_SET(m_iSocket, &errsocket);

	// setup wait timer struct
	waitcycle.tv_sec = 0;
	waitcycle.tv_usec = 50;

	// do select
	selectreturn = select(m_iSocket + 1, NULL, NULL,
						  &errsocket, &waitcycle);

	// find activated descriptors
	if(selectreturn < 0) return false;
	else if(FD_ISSET(m_iSocket, &errsocket)) return true;

	// deactivated
	return false;
}

//////////////////////////////////////////////////////////////////////////
// IsWriteable() : Returns true if we can write to the socket
//////////////////////////////////////////////////////////////////////////

bool CSocket::IsWriteable() const
{
	// we need a purpose
	if(!m_bConnected && !m_bListening) return false;

	// variables for select
	int selectreturn;
	static struct timeval waitcycle;
	static fd_set outsocket;

	// reset descriptors & add socket
	FD_ZERO(&outsocket);
	FD_SET(m_iSocket, &outsocket);

	// setup wait timer struct
	waitcycle.tv_sec = 0;
	waitcycle.tv_usec = 50;

	// do select
	selectreturn = select(m_iSocket + 1, NULL, &outsocket,
						  NULL, &waitcycle);

	// find activated descriptors
	if(selectreturn < 0) return false;
	else if(FD_ISSET(m_iSocket, &outsocket)) return true;

	// deactivated
	return false;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
