#ifndef __GLWINSYS_H__
#define __GLWINSYS_H__

//////////////////////////////////////////////////////////////////////////
// Sub-Library includes
//////////////////////////////////////////////////////////////////////////

#include "glw_types.h"
#include "glw_keysym.h"
#include "glw_window.h"
#include "glw_label.h"

//////////////////////////////////////////////////////////////////////////
// Render Layers
//////////////////////////////////////////////////////////////////////////

#define GLWL_WINDOWS	0x01
#define GLWL_MOUSE		0x02
#define GLWL_ALL		(GLWL_WINDOWS | GLWL_MOUSE)

//////////////////////////////////////////////////////////////////////////
// glWinsys class
//////////////////////////////////////////////////////////////////////////

class glWinsys
{
public:
	// constructor & destructor
	glWinsys();
	~glWinsys();

public:
	// initializes virtual desktop
	bool initialize(int desk_width, int desk_height);

	// destroys all the windows
	void free();

	// renders the selected layers
	void render(unsigned char layers) const;

	// renders the whole window scene
	void render() const;

	// sets key state to active
	void setKeyPress(glw_key key);

	// sets key state to inactive
	void setKeyRelease(glw_key key);

	// returns key state (pressed/unpressed)
	bool getKeyState(glw_key key) const;

	// sets key state to active
	void setMousePress(glw_mousekey key);

	// sets key state to inactive
	void setMouseRelease(glw_mousekey key);

	// this function activates/deactivates the off-window 
	// mouse boundary check
	void setMouseBoundCheck(bool check);

	// returns key state (pressed/unpressed)
	bool getMouseState(glw_mousekey key) const;

	// sets winsys mouse position
	void setMousePos(int x, int y);

	// sets winsys mouse relative position
	void setMouseRelPos(int x, int y);

	// queries winsys mouse position
	void getMousePos(int *x, int *y) const;

	// sets mouse cursor alpha intensity
	void setCursorAlpha(unsigned char alpha);

	// sets mouse cursor size
	void setCursorSize(int width, int height);

	// sets mouse cursor click point
	void setCursorHotSpot(int x, int y);

	// sets mouse cursor OpenGL texture
	void setCursorTexture(GLuint texture);

	// if enabled shows a point at the cursor's hot spot
	void setHotSpotDebug(bool state);

	// returns the root window
	glw_window *getDesktop() const;

	// sets virtual desktop size
	void setDesktopSize(int width, int height);

	// gets virtual desktop size
	void getDesktopSize(int *width, int *height) const;

	// resets mouse and keyboard key states
	void resetKeyStates();

private:
	// desktop window (root window)
	glw_window *m_Desktop;

	// do we check if the mouse goes out of bounds?
	bool m_bMouseBoundaryCheck;

	// mouse position
	glw_point m_MousePosition;

	// mouse hot spot (click center)
	bool m_bHotSpotDebug;
	glw_point m_CursorHotSpot;

	// mouse cursor alpha intensity
	unsigned char m_CursorAlpha;

	// mouse cursor texture id
	GLuint m_uiCursorTexture;

	// cursor texture size
	int m_iCursorWidth;
	int m_iCursorHeight;

	// key's states list
	bool m_Keys[GLWK_LAST];
	bool m_MouseKeys[GLWMK_LAST];

	// windows list
	std::list<glw_window*> m_Windows;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
