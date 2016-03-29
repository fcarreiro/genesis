#ifndef __STDAFX_H__
#define __STDAFX_H__

//////////////////////////////////////////////////////////////////////////
// Files changed infrequently
//////////////////////////////////////////////////////////////////////////

// OS-Specific includes
#ifdef WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#else
#endif

// C STD includes
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <cfloat>

// C++ STD includes
#include <algorithm>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>

// OpenGL
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

// OpenAL
//#include <al/al.h>
//#include <al/alut.h>

// SDL
#include <sdl/sdl.h>

// libxml2
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <libxml/parser.h>

// FreeImage
#include <freeimage.h>

// Zlib
#ifdef WIN32
#ifndef _WINDOWS
#define _WINDOWS
#endif
#endif
#ifndef ZLIB_DLL
#define ZLIB_DLL
#endif
#include <zlib.h>
#include <unzip.h>

// Memory Manager
//#ifdef _DEBUG
//#include "../../common/utility/memmgr.h"
//#endif

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
