#include "../precompiled/stdafx.h"
#include "../engine/base.h"

//////////////////////////////////////////////////////////////////////////
// CPlayer default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	// destroy player
	CPlayer::Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Frees all the player's data
//////////////////////////////////////////////////////////////////////////

void CPlayer::Destroy()
{
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
