#include "precompiled/stdafx.h"
#include "engine/base.h"

//////////////////////////////////////////////////////////////////////////
// Program entry point
//////////////////////////////////////////////////////////////////////////

// main entry point
int main(int argc, char *argv[])
{
#ifdef _DEBUG
	// check for memmory leaks
	leak_check_start();
#endif

	int iRet;
	CEngine logEngine;

	// prepare engine
    iRet = logEngine.Startup("Genesis: The Book of Firsts");
	if(iRet != ERR_OK) return iRet;

	// run engine
	iRet = logEngine.Run();

	// free engine
	logEngine.Close();

#ifdef _DEBUG
	// dump leak info
	leak_check_end();
#endif

	// no errors
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
