#ifndef __MAIN_H__
#define __MAIN_H__

//////////////////////////////////////////////////////////////////////////
// Definitions for portability
//////////////////////////////////////////////////////////////////////////

// OS specific winsock stuff
#ifdef WIN32
typedef int socklen_t;
#else
typedef	int SOCKET;
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define closesocket(s)	close(s);
#endif

// server version number
#define VERSION			"0.1"
#define VERSION_UPPER	0
#define VERSION_LOWER	1

// client version number
#define CLIENT_SUPPORTED_VERSION			"0.1"
#define CLIENT_SUPPORTED_VERSION_UPPER		0
#define CLIENT_SUPPORTED_VERSION_LOWER		1

// system definition
#ifdef WIN32
#define	SYSTEM	"win32"
#else
#define	SYSTEM	"linux"
#endif

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
