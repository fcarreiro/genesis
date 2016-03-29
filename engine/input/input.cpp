#include "../precompiled/stdafx.h"
#include "input.h"

//////////////////////////////////////////////////////////////////////////
// CInputSystem default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CInputSystem::CInputSystem() : m_iMouseX(0),
m_iMouseY(0), m_bSmooth(false)
{
	// arrays
	memset(m_bKeyStates, 0, sizeof(m_bKeyStates));
	memset(m_bMouseStates, 0, sizeof(m_bMouseStates));
}

CInputSystem::~CInputSystem()
{
	// free any leftovers
	Free();
}

//////////////////////////////////////////////////////////////////////////
// Init() : Initializes the system
//////////////////////////////////////////////////////////////////////////

bool CInputSystem::Init()
{
	// enable unicode translation
	SDL_EnableUNICODE(1);

	// return ok
	return true;
}

////////////////////////////////////////////////////////////////////////
// Free() : Shuts down the system
//////////////////////////////////////////////////////////////////////////

void CInputSystem::Free()
{
	// arrays
	memset(m_bKeyStates, 0, sizeof(m_bKeyStates));
	memset(m_bMouseStates, 0, sizeof(m_bMouseStates));

	// variables
	m_iMouseX = 0;
	m_iMouseY = 0;

	// clear queue
	m_KeyQueue.clear();
}

//////////////////////////////////////////////////////////////////////////
// Update() : Updates the system for the next frame
//////////////////////////////////////////////////////////////////////////

void CInputSystem::Update()
{
	Uint8 *keys;
	Uint8 buttons;
	int iMouseX, iMouseY;

	// removes all the old keys from the queue
	ClearKeyQueue();

	// mouse movement
	buttons = SDL_GetRelativeMouseState(&iMouseX, &iMouseY);

	// smooth last two frames
	if(m_bSmooth)
	{
		m_iMouseX = (m_iMouseX + iMouseX) / 2;
		m_iMouseY = (m_iMouseY + iMouseY) / 2;
	}
	else
	{
		m_iMouseX = iMouseX;
		m_iMouseY = iMouseY;
	}

	// left buttons state
	if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
		m_bMouseStates[SDL_BUTTON_LEFT] = true;
	else
		m_bMouseStates[SDL_BUTTON_LEFT] = false;

	// middle button state
	if(buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		m_bMouseStates[SDL_BUTTON_MIDDLE] = true;
	else
		m_bMouseStates[SDL_BUTTON_MIDDLE] = false;

	// right button state
	if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
		m_bMouseStates[SDL_BUTTON_RIGHT] = true;
	else
		m_bMouseStates[SDL_BUTTON_RIGHT] = false;

	// keyboard snapshot
	keys = SDL_GetKeyState(NULL);

	// check for keypress
	for(int i = 0; i < SDLK_LAST; ++i)
	{
		if(m_bKeyStates[i] && !keys[i])
			m_KeyQueue.push_back(static_cast<SDLKey> (i));
	}

	// copy (renew) state
	memcpy(m_bKeyStates, keys, sizeof(m_bKeyStates));
}

//////////////////////////////////////////////////////////////////////////
// GetKeyHit() : Checks if that key has been pressed
//////////////////////////////////////////////////////////////////////////

bool CInputSystem::GetKeyHit(SDLKey key)
{
	std::list<SDLKey>::const_iterator it;

	// look for the key
	it = std::find(m_KeyQueue.begin(), m_KeyQueue.end(), key);

	// if we find it return ok
	if(it != m_KeyQueue.end()) return true;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
// GetQueuedKey() : Gets first key pressed from queue
//////////////////////////////////////////////////////////////////////////

SDLKey CInputSystem::GetQueuedKey()
{
	// check buffer
	if(m_KeyQueue.size() > 0)
	{
		// get last key
		SDLKey key = m_KeyQueue.front();
		m_KeyQueue.pop_front();

		// return key
		return key;
	}
	else
	{
		// buffer empty
		return (SDLKey) 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// GetMouseMotion() : Returns the relative x-y movement within the last frame
//////////////////////////////////////////////////////////////////////////

void CInputSystem::GetMouseMotion(int *x, int *y)
{
	// x mouse movement
	if(x) *x = m_iMouseX;

	// y mouse movement
	if(y) *y = m_iMouseY;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
