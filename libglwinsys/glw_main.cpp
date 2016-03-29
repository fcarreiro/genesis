#include <cstdlib>
#include <windows.h>
#include <string>
#include <list>
#include <gl/gl.h>
#include "glwinsys.h"

//////////////////////////////////////////////////////////////////////////
// glWinsys class constructor & destructor
//////////////////////////////////////////////////////////////////////////

glWinsys::glWinsys() : m_Desktop(0),
m_bMouseBoundaryCheck(false), m_uiCursorTexture(0),
m_iCursorWidth(32), m_iCursorHeight(32),
m_CursorAlpha(255), m_bHotSpotDebug(false)
{
	// initialize variables
}

glWinsys::~glWinsys()
{
	// destroy windows and uninitialze if needed
	free();
}

//////////////////////////////////////////////////////////////////////////
// initialize() : Initializes virtual desktop
//////////////////////////////////////////////////////////////////////////

bool glWinsys::initialize(int desk_width, int desk_height)
{
	// previous instance?
	if(m_Desktop) return false;

	// create desktop
	m_Desktop = new glw_window();
	if(!m_Desktop) return false;

	// set desktop size
	m_Desktop->setSize(desk_width, desk_height);

	// reset key states
	resetKeyStates();

	// reset mouse position (center)
	m_MousePosition.x = desk_width / 2;
	m_MousePosition.y = desk_height / 2;

	// reset boundary check
	m_bMouseBoundaryCheck = false;

	// no error
	return true;
}

//////////////////////////////////////////////////////////////////////////
// free() : Destroys all the windows
//////////////////////////////////////////////////////////////////////////

void glWinsys::free()
{
	// destroy desktop
	delete m_Desktop;
	m_Desktop = 0;
}

//////////////////////////////////////////////////////////////////////////
// render() : Renders the whole window scene
// this functions is just a renderall shortcut
//////////////////////////////////////////////////////////////////////////

void glWinsys::render() const {	render(GLWL_ALL); }

//////////////////////////////////////////////////////////////////////////
// render() : Renders the selected layers
//////////////////////////////////////////////////////////////////////////

void glWinsys::render(unsigned char layers) const
{
	// if we haven't been initialized we return
	if(!m_Desktop) return;

	// save our 3d matrices
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// start 2d mode
	glOrtho(0, m_Desktop->getWidth(), m_Desktop->getHeight(), 0, 0, 1);

	// save modified bits
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT);

	// setup new state
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	// blending options (translcency)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// render windows
	if(layers & GLWL_WINDOWS)
	{
		m_Desktop->renderChildren();
	}

	// render mouse
	if(layers & GLWL_MOUSE)
	{
		// draw cursor with a texture if we have it
		if(m_uiCursorTexture)
		{
			glBindTexture(GL_TEXTURE_2D, m_uiCursorTexture);
			glEnable(GL_TEXTURE_2D);
		}
		// or else draw a white rectangle
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

		// blend modes and color
		glColor4ub(255, 255, 255, m_CursorAlpha);

		// draw it
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, 1);
			glVertex2i(m_MousePosition.x,
				       m_MousePosition.y);

			glTexCoord2i(0, 0);
			glVertex2i(m_MousePosition.x,
				       m_MousePosition.y + m_iCursorHeight);

			glTexCoord2i(1, 1);
			glVertex2i(m_MousePosition.x + m_iCursorWidth,
				       m_MousePosition.y);

			glTexCoord2i(1, 0);
			glVertex2i(m_MousePosition.x + m_iCursorWidth,
				       m_MousePosition.y + m_iCursorHeight);
		glEnd();

		// hot spot debug
		if(m_bHotSpotDebug)
		{
			// draw one white point with 4 black corners
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_POINTS);
				glColor4ub(255, 255, 255, 255);
				glVertex2i(m_MousePosition.x + m_CursorHotSpot.x,
						   m_MousePosition.y + m_CursorHotSpot.y);

				glColor4ub(0, 0, 0, 255);
				glVertex2i(m_MousePosition.x + m_CursorHotSpot.x - 1,
						   m_MousePosition.y + m_CursorHotSpot.y - 1);

				glVertex2i(m_MousePosition.x + m_CursorHotSpot.x + 1,
						   m_MousePosition.y + m_CursorHotSpot.y + 1);

				glVertex2i(m_MousePosition.x + m_CursorHotSpot.x - 1,
						   m_MousePosition.y + m_CursorHotSpot.y + 1);

				glVertex2i(m_MousePosition.x + m_CursorHotSpot.x + 1,
						   m_MousePosition.y + m_CursorHotSpot.y - 1);
			glEnd();
		}
	}

	// restore bits
	glPopAttrib();

	// back to our 3d matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////
// resetKeyStates() : Resets mouse and keyboard key states
//////////////////////////////////////////////////////////////////////////

void glWinsys::resetKeyStates()
{
	// clear arrays
	memset(m_Keys, 0, sizeof(m_Keys));
	memset(m_MouseKeys, 0, sizeof(m_MouseKeys));
}

//////////////////////////////////////////////////////////////////////////
// setMouseBoundCheck() : This function activates/deactivates the off-window 
// mouse boundary check
//////////////////////////////////////////////////////////////////////////

void glWinsys::setMouseBoundCheck(bool check)
{
	m_bMouseBoundaryCheck = check;
}

//////////////////////////////////////////////////////////////////////////
// setMousePos() : Sets winsys mouse position
//////////////////////////////////////////////////////////////////////////

void glWinsys::setMousePos(int x, int y)
{
	// check if it goes out of bounds
	if(m_bMouseBoundaryCheck && m_Desktop)
	{
		// x check
		if(x < 0) x = 0;
		else if(x > m_Desktop->getWidth()) x = m_Desktop->getWidth();

		// y check
		if(y < 0) y = 0;
		else if(y > m_Desktop->getHeight()) y = m_Desktop->getHeight();
	}

	// set mouse position
	m_MousePosition.x = x;
	m_MousePosition.y = y;
}

//////////////////////////////////////////////////////////////////////////
// setMouseRelPos() : Sets winsys mouse relative position
//////////////////////////////////////////////////////////////////////////

void glWinsys::setMouseRelPos(int x, int y)
{
	// save old position
	glw_point old_position = m_MousePosition;

	// set mouse new position
	m_MousePosition.x += x;
	m_MousePosition.y += y;

	// check if it goes out of bounds
	if(m_bMouseBoundaryCheck && m_Desktop)
	{
		// x check
		if(m_MousePosition.x < 0)
		{
			m_MousePosition.x = 0;
		}
		else if(m_MousePosition.x > m_Desktop->getWidth())
		{
			m_MousePosition.x = m_Desktop->getWidth();
		}

		// y check
		if(m_MousePosition.y < 0)
		{
			m_MousePosition.y = 0;
		}
		else if(m_MousePosition.y > m_Desktop->getHeight())
		{
			m_MousePosition.y = m_Desktop->getHeight();
		}
	}

	// find window at position before moving
	glw_window *topmost;
	topmost = m_Desktop->getTopmostAt(old_position.x + m_CursorHotSpot.x,
		old_position.y + m_CursorHotSpot.y);
	if(!topmost) return;

	// prepare message
	glw_point relative(m_MousePosition.x - old_position.x,
					   m_MousePosition.y - old_position.y);

	// send the message
	topmost->sendMessage(GLWM_MOUSEMOVE, (glw_param) &m_MousePosition,
		(glw_param) &relative);
}

//////////////////////////////////////////////////////////////////////////
// getMousePos() : Queries winsys mouse position
//////////////////////////////////////////////////////////////////////////

void glWinsys::getMousePos(int *x, int *y) const
{
	// pass mouse position
	if(x) *x = m_MousePosition.x;
	if(y) *y = m_MousePosition.y;
}

//////////////////////////////////////////////////////////////////////////
// setDesktopSize() : Sets virtual desktop size
//////////////////////////////////////////////////////////////////////////

void glWinsys::setDesktopSize(int width, int height)
{
	// set new size
	if(m_Desktop) m_Desktop->setSize(width, height);
}

//////////////////////////////////////////////////////////////////////////
// getDesktopSize() : Gets virtual desktop size
//////////////////////////////////////////////////////////////////////////

void glWinsys::getDesktopSize(int *width, int *height) const
{
	// if we haven't initialized return
	if(!m_Desktop) return;

	// pass desktop size
	if(width) *width = m_Desktop->getWidth();
	if(height) *height = m_Desktop->getHeight();
}

//////////////////////////////////////////////////////////////////////////
// setCursorAlpha() : Sets mouse cursor alpha intensity
//////////////////////////////////////////////////////////////////////////

void glWinsys::setCursorAlpha(unsigned char alpha)
{
	m_CursorAlpha = alpha;
}

//////////////////////////////////////////////////////////////////////////
// setCursorSize() : Sets mouse cursor size
//////////////////////////////////////////////////////////////////////////

void glWinsys::setCursorSize(int width, int height)
{
	// check
	if(width <= 0 || height <= 0) return;

	// set
	m_iCursorWidth = width;
	m_iCursorHeight = height;
}

//////////////////////////////////////////////////////////////////////////
// setHotSpotDebug() : If enabled shows a point at the cursor's hot spot
//////////////////////////////////////////////////////////////////////////

void glWinsys::setHotSpotDebug(bool state)
{
	m_bHotSpotDebug = state;
}

//////////////////////////////////////////////////////////////////////////
// setCursorHotSpot() : Sets mouse cursor click point
//////////////////////////////////////////////////////////////////////////

void glWinsys::setCursorHotSpot(int x, int y)
{
	// check
	if(x <= 0 || y <= 0 ||
	   x > m_iCursorWidth ||
	   y > m_iCursorHeight) return;

	// set
	m_CursorHotSpot.x = x;
	m_CursorHotSpot.y = y;
}

//////////////////////////////////////////////////////////////////////////
// setCursorTexture() : Sets mouse cursor OpenGL texture
//////////////////////////////////////////////////////////////////////////

void glWinsys::setCursorTexture(GLuint texture)
{
	// if we provide a valid texture we set it
	if(texture > 0) m_uiCursorTexture = texture;
}

//////////////////////////////////////////////////////////////////////////
// setKeyPress() : Sets key state to active
//////////////////////////////////////////////////////////////////////////

void glWinsys::setKeyPress(glw_key key)
{
	m_Keys[key] = true;
}

//////////////////////////////////////////////////////////////////////////
// setKeyRelease() : Sets key state to inactive
//////////////////////////////////////////////////////////////////////////

void glWinsys::setKeyRelease(glw_key key)
{
	m_Keys[key] = false;
}

//////////////////////////////////////////////////////////////////////////
// getKeyState() : Returns key state (pressed/unpressed)
//////////////////////////////////////////////////////////////////////////

bool glWinsys::getKeyState(glw_key key) const
{
	return m_Keys[key];
}

//////////////////////////////////////////////////////////////////////////
// setMousePress() : Sets key state to active
//////////////////////////////////////////////////////////////////////////

void glWinsys::setMousePress(glw_mousekey key)
{
	// we havent been initialized
	if(!m_Desktop) return;

	// set pressed key flag
	m_MouseKeys[key] = true;

	// find picked window (at mouse + hotspot)
	glw_window *topmost;
	topmost = m_Desktop->getTopmostAt(m_MousePosition.x + m_CursorHotSpot.x,
		m_MousePosition.y + m_CursorHotSpot.y);
	if(!topmost) return;

	// send picked window the message
	topmost->sendMessage(GLWM_MOUSEDOWN, (glw_param) key,
		(glw_param) &m_MousePosition);
}

//////////////////////////////////////////////////////////////////////////
// setMouseRelease() : Sets key state to inactive
//////////////////////////////////////////////////////////////////////////

void glWinsys::setMouseRelease(glw_mousekey key) 
{
	// set released key flag
	m_MouseKeys[key] = false;

	// get topmost window
	glw_window *topmost;
	topmost = m_Desktop->getTopmost();
	if(!topmost) return;

	// send picked window the message
	topmost->sendMessage(GLWM_MOUSEUP, (glw_param) key,
		(glw_param) &m_MousePosition);
}

//////////////////////////////////////////////////////////////////////////
// getMouseState() : Returns key state (pressed/unpressed)
//////////////////////////////////////////////////////////////////////////

bool glWinsys::getMouseState(glw_mousekey key) const
{
	return m_MouseKeys[key];
}

//////////////////////////////////////////////////////////////////////////
// getDesktop() : Returns the root window
//////////////////////////////////////////////////////////////////////////

glw_window *glWinsys::getDesktop() const
{
	return m_Desktop;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
