#ifndef __INPUT_H__
#define __INPUT_H__

//////////////////////////////////////////////////////////////////////////
// Input constants
//////////////////////////////////////////////////////////////////////////

#define INPUT_KEYBOARD		1
#define INPUT_MOUSE			2
#define INPUT_JOYSTICK		4

#define INPUT_RELEASED		0
#define INPUT_PRESSED		1

#define INPUT_OK			0
#define INPUT_NO_DEVICE		-1
#define INPUT_QUEUE_EMPTY	-2
#define INPUT_WRONG_BUTTON	-3

//////////////////////////////////////////////////////////////////////////
// CInputSystem class
//////////////////////////////////////////////////////////////////////////

class CInputSystem
{
public:
	// contructor & destructor
	CInputSystem();
	~CInputSystem();

	// initializes the system
	bool Init();

	// shuts the system down
	void Free();

	// updates the system for the next frame
	void Update();

	// gets the state of a key
	bool GetKey(SDLKey key) { return m_bKeyStates[key]; }

	// checks if that key has been pressed
	bool GetKeyHit(SDLKey key);

	// returns input smoothing state
	bool GetSmooth() const { return m_bSmooth; }

	// sets input smoothing state
	void SetSmooth(bool smooth) { m_bSmooth = smooth; }

	// gets first key pressed from queue
	SDLKey GetQueuedKey();

	// gets relative mouse movement
	void GetMouseMotion(int *x, int *y);

	// gets specific mouse button state
	bool GetMouseButton(int button) { return m_bMouseStates[button]; }

	// clears the input queue
	void ClearKeyQueue() { m_KeyQueue.clear(); }

private:
	// key queue
	std::list<SDLKey> m_KeyQueue;

	// keyboard states
	bool m_bKeyStates[SDLK_LAST];

	// mouse button states
	bool m_bMouseStates[3];

	// smooth last frames flag
	bool m_bSmooth;

	// mouse relative X movement
	int m_iMouseX;

	// mouse relative Y movement
	int m_iMouseY;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
