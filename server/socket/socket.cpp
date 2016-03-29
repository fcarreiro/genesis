#include "../precompiled/stdafx.h"
#include "../main.h"
#include "socket.h"

//////////////////////////////////////////////////////////////////////////
// CSocket default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CSocket::CSocket(SOCKET sock) : m_iSocket(sock)
{
	// if we didnt specify a socket we create a new one
	if(m_iSocket == 0)
	{
		struct protoent *protoentry = NULL;
		protoentry = getprotobyname("tcp");
		m_iSocket = socket(AF_INET, SOCK_STREAM, protoentry->p_proto);
	}

#ifndef WIN32
	// set linger
	struct linger ling;
	ling.l_onoff = 0;
	ling.l_linger = 0;
	setsockopt(m_iSocket, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

	// set non blocking
	int sockflags;
	int result;
	sockflags = fcntl(m_iSocket, F_GETFL, 0);
	sockflags |= O_NONBLOCK;
	result = fcntl(m_iSocket, F_SETFL, sockflags);

	// set reuse address
	int optval = 1;
	setsockopt(m_iSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif
}

CSocket::~CSocket()
{
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
