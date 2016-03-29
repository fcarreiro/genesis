#include <cstdlib>
#include <windows.h>
#include <string>
#include <list>
#include <gl/gl.h>
#include "glwinsys.h"
#include "glw_util.h"

//////////////////////////////////////////////////////////////////////////
// glw_window class constructor & destructor
//////////////////////////////////////////////////////////////////////////

glw_window::glw_window() : m_bVisible(true),
m_Parent(0), m_iWidth(0),
m_iHeight(0), m_bMoveable(true),
m_bDragging(false)
{
	// initialize variables to default
}

glw_window::~glw_window()
{
}

//////////////////////////////////////////////////////////////////////////
// addChild() : Adds "window" to its subwindows list
//////////////////////////////////////////////////////////////////////////

bool glw_window::addChild(glw_window *window)
{
	// have we been provided a correct pointer?
	if(!window) return false;

	// check if we already have that window among our children
	std::list<glw_window*>::const_iterator it;
	it = std::find(m_Children.begin(), m_Children.end(), window);
	if(it != m_Children.end()) return false;

	// add the window, welcome!
	window->setParent(this);
	m_Children.push_back(window);

	// no problem
	return true;
}

//////////////////////////////////////////////////////////////////////////
// removeChild() : Removes window from its subwindows
//////////////////////////////////////////////////////////////////////////

bool glw_window::removeChild(glw_window *window)
{
	// have we been provided a correct pointer?
	if(!window) return false;

	// check if we already have that window among our children
	std::list<glw_window*>::iterator it;
	it = std::find(m_Children.begin(), m_Children.end(), window);
	if(it == m_Children.end()) return false;

	// remove the window
	window->setParent(0);
	m_Children.erase(it);

	// no problem
	return true;
}

//////////////////////////////////////////////////////////////////////////
// bringToTop() : Sets this window on top of all the others
//////////////////////////////////////////////////////////////////////////

void glw_window::bringToTop()
{
	if(m_Parent)
	{
		// save parent
		glw_window *parent = m_Parent;

		// then add and remove so we finish on top
		// of the list's z-order
		parent->removeChild(this);
		parent->addChild(this);
	}
}

//////////////////////////////////////////////////////////////////////////
// isActive() : returns true if we are the topmost window
//////////////////////////////////////////////////////////////////////////

bool glw_window::isActive() const
{
	// if we dont have a parent we must be the one and only
	if(!m_Parent) return true;

	// are we our parent's topmost?
	if(m_Parent->getTopmost() == this) return true;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
// render() : Draws the window
//////////////////////////////////////////////////////////////////////////

void glw_window::render() const
{
	if(m_bVisible)
	{
		// get absolute window position
		int window_x = (m_Parent ? m_Parent->getX() : 0) + m_Position.x;
		int window_y = (m_Parent ? m_Parent->getY() : 0) + m_Position.y;

		// setup for window drawing
		glDisable(GL_TEXTURE_2D);

		// draw
		glColor4ub(255, 255, 255, 255);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex2i(window_x, window_y);
			glVertex2i(window_x, window_y + m_iHeight);
			glVertex2i(window_x + m_iWidth, window_y);
			glVertex2i(window_x + m_iWidth, window_y + m_iHeight);
		glEnd();

		glColor4ub(0, 0, 0, 255);
		glBegin(GL_LINE_STRIP);
			glVertex2i(window_x, window_y);
			glVertex2i(window_x, window_y + m_iHeight);
			glVertex2i(window_x + m_iWidth, window_y + m_iHeight);
			glVertex2i(window_x + m_iWidth, window_y);
			glVertex2i(window_x, window_y);
		glEnd();

		// continue rendering
		renderChildren();
	}
}

//////////////////////////////////////////////////////////////////////////
// renderChildren() : Draws this window's children
//////////////////////////////////////////////////////////////////////////

void glw_window::renderChildren() const
{
	// we won't render if we have no children
	if(m_Children.size() == 0) return;

	// create const iterator for the children list
	std::list<glw_window*>::const_iterator it;

	// recurse through the children and draw them
	for(it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		reinterpret_cast<const glw_window *>(*it)->render();
	}
}

//////////////////////////////////////////////////////////////////////////
// getTopmost() : Returns the first visible window (topmost)
//////////////////////////////////////////////////////////////////////////

glw_window *glw_window::getTopmost() const
{
	// if we have no children there is no topmost window
	if(m_Children.size() == 0)
	{
		return 0;
	}
	// if we do have then it's the last in the list
	else
	{
		return m_Children.back();
	}
}

//////////////////////////////////////////////////////////////////////////
// getTopmostAt() : Returns the first visible window (topmost)
// at a specific coord
//////////////////////////////////////////////////////////////////////////

glw_window *glw_window::getTopmostAt(int x, int y) const
{
	// if we have no children there is no topmost window
	if(m_Children.size() == 0)
	{
		return 0;
	}
	// if we do have then it's the last in the list
	else
	{
		// temporary window container
		glw_window *topmost = 0;

		// create const iterator for the children list
		std::list<glw_window*>::const_iterator it;

		// recurse through the children and draw them
		for(it = m_Children.begin(); it != m_Children.end(); ++it)
		{
			if(isInBounds(x, y,
						 (*it)->getX(),
						 (*it)->getY(),
						 (*it)->getX() + (*it)->getWidth(),
						 (*it)->getY() + (*it)->getHeight()))
			{
				topmost = reinterpret_cast<glw_window *>(*it);
			}
		}

		// returns the topmost or NULL (0)
		return topmost;
	}
}

//////////////////////////////////////////////////////////////////////////
// setSize() : Sets window's dimensions
//////////////////////////////////////////////////////////////////////////

void glw_window::setSize(int width, int height)
{
	// check
	if(width <= 0) width = 1;
	if(height <= 0) height = 1;

	// set
	m_iWidth = width;
	m_iHeight = height;
}

//////////////////////////////////////////////////////////////////////////
// getSize() : Queries window's size
//////////////////////////////////////////////////////////////////////////

void glw_window::getSize(int *width, int *height) const
{
	if(width) *width = m_iWidth;
	if(height) *height = m_iHeight;
}

//////////////////////////////////////////////////////////////////////////
// setBounds() : Sets window position and size
//////////////////////////////////////////////////////////////////////////

void glw_window::setBounds(int x, int y, int width, int height)
{
	// check size
	if(width <= 0) width = 1;
	if(height <= 0) height = 1;

	// set bounds
	m_Position.x = x;
	m_Position.y = y;
	m_iWidth = width;
	m_iHeight = height;
}

//////////////////////////////////////////////////////////////////////////
// show() : Shows (sets visible state) of the window (will be rendered)
// so will be its children
//////////////////////////////////////////////////////////////////////////

void glw_window::show() { m_bVisible = true; }

//////////////////////////////////////////////////////////////////////////
// hide() : hides (sets invisible state) of the window 
// (will NOT be rendered) so will be its children
//////////////////////////////////////////////////////////////////////////

void glw_window::hide() { m_bVisible = false; }

//////////////////////////////////////////////////////////////////////////
// setParent() : Changes/sets the window's parent window
//////////////////////////////////////////////////////////////////////////

void glw_window::setParent(glw_window *window) { m_Parent = window; }

//////////////////////////////////////////////////////////////////////////
// getParent() : Returns the window's parent
//////////////////////////////////////////////////////////////////////////

glw_window *glw_window::getParent() const { return m_Parent; }

//////////////////////////////////////////////////////////////////////////
// setPosition() : Sets window's position
//////////////////////////////////////////////////////////////////////////

void glw_window::setPosition(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
}

//////////////////////////////////////////////////////////////////////////
// getX() : returns the window's x coord
//////////////////////////////////////////////////////////////////////////

int glw_window::getX() const { return m_Position.x; }

//////////////////////////////////////////////////////////////////////////
// getY() : returns the window's y coord
//////////////////////////////////////////////////////////////////////////

int glw_window::getY() const { return m_Position.y; }

//////////////////////////////////////////////////////////////////////////
// getWidth() : Returns the window's width
//////////////////////////////////////////////////////////////////////////

int glw_window::getWidth() const { return m_iWidth; }

//////////////////////////////////////////////////////////////////////////
// getHeight() : Returns the window's height
//////////////////////////////////////////////////////////////////////////

int glw_window::getHeight() const { return m_iHeight; }

//////////////////////////////////////////////////////////////////////////
// isVisible() : True if the window's visibility flag is on
// (will be rendered)
//////////////////////////////////////////////////////////////////////////

bool glw_window::isVisible() const { return m_bVisible; }

//////////////////////////////////////////////////////////////////////////
// setCaption() : Sets window's title/text/caption
//////////////////////////////////////////////////////////////////////////

void glw_window::setCaption(const std::string & caption)
{
	m_strCaption = caption;
}

//////////////////////////////////////////////////////////////////////////
// getCaption() : Rreturns window's title/text/caption
//////////////////////////////////////////////////////////////////////////

std::string glw_window::getCaption()
{
	return m_strCaption;
}

//////////////////////////////////////////////////////////////////////////
// sendMessage() : Sends and executes message
//////////////////////////////////////////////////////////////////////////

bool glw_window::sendMessage(glw_message message,
	glw_param lparam, glw_param wparam)
{
	// if we are invisible we dont respond to messages
	if(!m_bVisible) return true;

	// check message and execute the function
	switch(message)
	{
		case GLWM_MOUSEMOVE:
			return wm_mousemove(((glw_point*)lparam)->x,
				((glw_point*)lparam)->y,
				((glw_point*)wparam)->x,
				((glw_point*)wparam)->y);
			break;

		case GLWM_MOUSEDOWN:
			return wm_mousedown((glw_mousekey) lparam,
				((glw_point*)wparam)->x,
				((glw_point*)wparam)->y);
			break;

		case GLWM_MOUSEUP:
			return wm_mouseup((glw_mousekey) lparam,
				((glw_point*)wparam)->x,
				((glw_point*)wparam)->y);
			break;

		default:
			return true;
	}
}

//////////////////////////////////////////////////////////////////////////
// wm_mousemove() : Mouse was moved
//////////////////////////////////////////////////////////////////////////

bool glw_window::wm_mousemove(int x, int y, int rx, int ry)
{
	// we may be moving the window
	if(m_bDragging)
	{
		m_Position.x += rx;
		m_Position.y += ry;
	}

	// window's still alive
	return true;
}

//////////////////////////////////////////////////////////////////////////
// wm_mousedown() : Mouse button was pressed
//////////////////////////////////////////////////////////////////////////

bool glw_window::wm_mousedown(glw_mousekey button, int x, int y)
{
	// yes! i'm the one!
	if(!isActive())	bringToTop();

	// if we pressed we are dragging
	m_bDragging = true;

	// window's still alive
	return true;
}

//////////////////////////////////////////////////////////////////////////
// wm_mouseup() : Mouse button was released
//////////////////////////////////////////////////////////////////////////

bool glw_window::wm_mouseup(glw_mousekey button, int x, int y)
{
	// we stopped clicking, drag no more
	m_bDragging = false;

	// window's still alive
	return true;
}

//////////////////////////////////////////////////////////////////////////
// wm_keydown() : Keyboard has been pressed
//////////////////////////////////////////////////////////////////////////

bool glw_window::wm_keydown(glw_key key)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// wm_keyup() : Keyboard has been released
//////////////////////////////////////////////////////////////////////////

bool glw_window::wm_keyup(glw_key key)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
