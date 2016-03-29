#include "../precompiled/stdafx.h"
#include "network.h"

//////////////////////////////////////////////////////////////////////////
// Network constants
//////////////////////////////////////////////////////////////////////////

#define SERVER	"localhost"
#define PORT	1798

//////////////////////////////////////////////////////////////////////////
// CNetworkSystem default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CNetworkSystem::CNetworkSystem()
{
	socket = 0;
	buffer = NULL;
}

CNetworkSystem::~CNetworkSystem()
{
	Shutdown();
}

//////////////////////////////////////////////////////////////////////////
// Startup() : Initializes network
//////////////////////////////////////////////////////////////////////////

bool CNetworkSystem::Startup()
{
#ifdef WIN32
	WORD version = MAKEWORD(1,1);
	WSADATA wsaData;

	// initialize winsock
	if(WSAStartup(version, &wsaData) != 0)
	{
		return false;
	}
#endif

	// no error
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Shutdown() : Frees network resources
//////////////////////////////////////////////////////////////////////////

void CNetworkSystem::Shutdown()
{
	// free packet buffer
	delete buffer;

	// disconnect
	if(IsConnected()) Disconnect();

#ifdef WIN32
	// winsock cleanup
	WSACleanup();
#endif
}

//////////////////////////////////////////////////////////////////////////
// Connect() : Connects to a server
//////////////////////////////////////////////////////////////////////////

bool CNetworkSystem::Connect(const char *host, unsigned short port)
{
/*	int nRet;
	LPHOSTENT hostEntry;

	hostEntry = gethostbyname(host);
	if (!hostEntry) return false;

	CNetworkSystem::socket = ::socket(AF_INET,	// Go over TCP/IP
						  SOCK_STREAM,			// This is a stream-oriented socket
						 IPPROTO_TCP);			// Use TCP rather than UDP

	if(socket == INVALID_SOCKET) return false;

	// Fill a SOCKADDR_IN struct with address information
	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	serverInfo.sin_port = htons(port);

	// Connect to the server
	nRet = connect(socket, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));

	if (nRet == SOCKET_ERROR)
	{
		closesocket(socket);
		socket = 0;

		return false;
	}

	// Send version
	pkVersion ver;

	memset(&ver, 0, sizeof(pkVersion));
//	ver.ucLower = CLIENT_VERSION_LOWER;
//	ver.ucUpper = CLIENT_VERSION_UPPER;

	nRet = SendPacket(OC_VERSION, &ver, sizeof(pkVersion));
	if(!nRet) return false;

	// check for success
	void *fail = NULL;

	nRet = RecievePacket((void**)&fail);
	if(nRet != OC_OK)
	{
		// free pointer
		delete fail;

		// error!
		return false;
	}
*/
	// no error
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Disconnect() : Disconnet from server
//////////////////////////////////////////////////////////////////////////

void CNetworkSystem::Disconnect()
{
	if(socket)
	{
		// tell the server i'm logging out
//		SendPacket(OC_LOGOUT, NULL, 0);

		// cerrar socket
		closesocket(socket);
	}

	socket = 0;
}

//////////////////////////////////////////////////////////////////////////
// RecievePacket() : Reads a packet from the server
//////////////////////////////////////////////////////////////////////////

unsigned char CNetworkSystem::RecievePacket(void **buf)
{
/*	int nRet;
	int size = 0;
	unsigned char packetID = 0;

	// retrieve packet ID
	recv(socket, (char*)&packetID, 1, 0);

	// free buffer
	*buf = NULL;

	// switch packet type & create buffer
	switch(packetID)
	{
	case OC_JOIN:
		*buf = new pkJoin;
		size = sizeof(pkJoin);
		break;

	case OC_PART:
		*buf = new pkPart;
		size = sizeof(pkPart);
		break;

	case OC_FAILED:
		*buf = new pkFailed;
		size = sizeof(pkFailed);
		break;

	case OC_VERSION:
		*buf = new pkVersion;
		size = sizeof(pkVersion);
		break;

//	case OC_PLAYER_INFO:
//		*buf = new pkPlayerInfo;
//		size = sizeof(pkPlayerInfo);
//		break;

	case OC_MOVE:
		*buf = new pkMove;
		size = sizeof(pkMove);
		break;

	case OC_TALK:
		*buf = new pkTalk;
		size = sizeof(pkTalk);
		break;

	default:
		// opcode without body
		*buf = NULL;
		break;
	}

	// retrieve packet body
	if(*buf)
	{
		nRet = recv(socket, (char*) *buf, size, 0);

		if(nRet == 0) return OC_ERROR;
	}
	else
	{
		// if we dont have a buffer but we want to retrieve there is an error
		if(size) return OC_ERROR;
	}

	// return opcode
	return packetID;*/
	return 1;
}

//////////////////////////////////////////////////////////////////////////
// SendPacket() : Send a packet
//////////////////////////////////////////////////////////////////////////

bool CNetworkSystem::SendPacket(unsigned char packetID, void *buffer, int size)
{
	// send packet header (id)
	send(socket, (char*)&packetID, 1, 0);

	// send packet body
	if(buffer != NULL && size != 0)
	{
		send(socket, (char*)buffer, size, 0);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// IncomingData() : Returns true if there is data pending to be read
//////////////////////////////////////////////////////////////////////////

bool CNetworkSystem::IncomingData()
{
	timeval timeout = {0, 0};
	int s;						// s is where the data is stored from the select function
	int nfds;					// This is used for Compatibility
	fd_set conn;				// Setup the read variable for the Select function

	FD_ZERO(&conn);				// Set the data in conn to nothing
	FD_SET(socket, &conn);		// Tell it to get the data from the Socket
	nfds = socket + 1;
	s = select(nfds, &conn, NULL, NULL, &timeout); // Is there any data coming in?

	if(s > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// CNetworkSystem default constructor & destructor
//////////////////////////////////////////////////////////////////////////

void CNetworkSystem::Control()
{
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
