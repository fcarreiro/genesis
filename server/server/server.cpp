#include "../precompiled/stdafx.h"
#include "../main.h"
#include "../socket/socket.h"
#include "server.h"

//////////////////////////////////////////////////////////////////////////
// Sockets list export
//////////////////////////////////////////////////////////////////////////

SOCKET_DB *g_Sockets;

//////////////////////////////////////////////////////////////////////////
// CServer default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CServer::CServer() : m_bRunning(false), m_Listener(NULL)
{
	// initialize variables
	g_Sockets = &m_Sockets;
}

CServer::~CServer()
{
	// just in case
	Shutdown();
}

//////////////////////////////////////////////////////////////////////////
// Initialize() : Sets up the server if needeed
//////////////////////////////////////////////////////////////////////////

bool CServer::Initialize()
{
#if defined WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(1,1), &wsa_data);
#endif

	// start database

	// load rooms

	// no error
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Shutdown() : Frees the server
//////////////////////////////////////////////////////////////////////////

void CServer::Shutdown()
{
	// close listener
	delete m_Listener;
	m_Listener = NULL;

	// unload rooms

	// stop database

#if defined WIN32
	WSACleanup();
#endif
}

//////////////////////////////////////////////////////////////////////////
// Run() : Main server loop
//////////////////////////////////////////////////////////////////////////

bool CServer::Run()
{
	// start listener(s)
	m_Listener = new CListener();

	// main loop
	int selectreturn;
	unsigned int maxfd;
	struct timeval waitcycle;
	SOCKET_DB::iterator it;
	fd_set insockets, outsockets, errsockets;

	// run!
	m_bRunning = true;
	while(m_bRunning)
	{
		FD_ZERO(&insockets);
		FD_ZERO(&outsockets);
		FD_ZERO(&errsockets);

		// add listener
		maxfd = m_Listener->GetSocket();
		FD_SET(m_Listener->GetSocket(), &insockets);

		// loop through the descriptor list and add sockets to approprate lists
		for(it = m_Sockets.begin(); it != m_Sockets.end(); ++it)
		{
			// everyone gets added to in and error
			FD_SET((*it)->GetSocket(), &errsockets);
			FD_SET((*it)->GetSocket(), &insockets);

			// if we have data pending to go out
			if((*it)->IsDataPending())
			{
				FD_SET((*it)->GetSocket(), &outsockets);
			}

			// max descriptor
			SOCKET sock = (*it)->GetSocket();
			if(sock > maxfd) maxfd = sock;
		}

		// setup wait timer struct
		waitcycle.tv_sec = 0;
		waitcycle.tv_usec = 50;

		// do select
		selectreturn = select(maxfd + 1, &insockets, &outsockets,
							  &errsockets, &waitcycle);

		// error, try again
		if(selectreturn < 0) continue;

		// find activated descriptors
		if(selectreturn > 0)
		{
			// check listener
			if(FD_ISSET(m_Listener->GetSocket(), &insockets))
			{
				m_Listener->DispatchRead();
			}

			// check sockets
			for(it = m_Sockets.begin(); it != m_Sockets.end(); ++it)
			{
				// check write
				//if(FD_ISSET((*it)->GetSocket(), &outsockets))
				//{
				//	(*it)->doWrite();
				//}
				
				// check read
				if(FD_ISSET((*it)->GetSocket(), &insockets))
				{
					(*it)->DispatchRead();
				}

				// check error
				if(FD_ISSET((*it)->GetSocket(), &errsockets))
				{
					// close any sockets in error
					std::cout << "connection lost!" << std::endl;
					delete (*it);
					it = m_Sockets.erase(it) - 1;
				}
			}
		}
	}

	// no error
	return true;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
