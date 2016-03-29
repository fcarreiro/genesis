#include "precompiled/stdafx.h"
#include "main.h"
#include "socket/socket.h"
#include "server/server.h"

//////////////////////////////////////////////////////////////////////////
// "main" entry point
//////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	bool bRet;
	CServer server;

	// initialize server
	std::cout << "Initializing server...";
	bRet = server.Initialize();
	if(!bRet)
	{
		std::cout << "ERROR!" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "OK!" << std::endl;
	}

	// initialize server
	std::cout << "Running server..." << std::endl;
	bRet = server.Run();

	// shutdown
	std::cout << "Cleaning..." << std::endl;
	server.Shutdown();

	// no errors
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
