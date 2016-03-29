#include "../precompiled/stdafx.h"
#include "../main.h"
#include "socket.h"
#include "listener.h"
#include "../server/server.h"

//////////////////////////////////////////////////////////////////////////
// CListener default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CListener::CListener(unsigned int port) : m_uiPort(port)
{
	// setup binding
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_uiPort);
	addr.sin_addr.s_addr = htons(INADDR_ANY);

	// bind to local socket
	bind(m_iSocket, (struct sockaddr *) &addr, sizeof(addr));
	listen(m_iSocket, 5);
}

CListener::~CListener()
{
}

//////////////////////////////////////////////////////////////////////////
// DispatchRead() : Opens a new connection
//////////////////////////////////////////////////////////////////////////

void CListener::DispatchRead()
{
	struct sockaddr addr;
	socklen_t slen = sizeof(struct sockaddr);
	int newsock = accept(m_iSocket, &addr, &slen);

	std::cout << "new connection!" << std::endl;
	CSocket *sock = new CDesc_Login(newsock);
	g_Sockets->push_back(sock);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
