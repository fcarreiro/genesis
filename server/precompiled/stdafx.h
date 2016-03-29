#ifndef __STDAFX_H__
#define __STDAFX_H__

//////////////////////////////////////////////////////////////////////////
// Hardly changed headers
//////////////////////////////////////////////////////////////////////////

// standard C headers
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

// standard C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <map>

// OS specific headers
#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
