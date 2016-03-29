#ifndef __GLW_WINDOW_H__
#define __GLW_WINDOW_H__

//////////////////////////////////////////////////////////////////////////
// glw_window class
//////////////////////////////////////////////////////////////////////////

class glw_window
{
public:
	// constructor & destructor
	glw_window();
	virtual ~glw_window();

public:
	// adds "window" to its subwindows list
	bool addChild(glw_window *window);

	// removes window from its subwindows
	bool removeChild(glw_window *window);

	// shows (sets visible state) of the window (will be rendered)
	// so will be its children
	void show();

	// hides (sets invisible state) of the window (will NOT be rendered)
	// so will be its children
	void hide();

	// sets this window on top of all the others
	void bringToTop();

	// returns true if we are the topmost window
	bool isActive() const;

	// changes/sets the window's parent window
	void setParent(glw_window *window);

	// returns the window's parent
	glw_window *getParent() const;

	// sets window's position
	void setPosition(int x, int y);

	// queries window's position
	void getPosition(int *x, int *y) const;

	// returns the window's x coord
	int getX() const;

	// returns the window's y coord
	int getY() const;

	// sets window's dimensions
	void setSize(int width, int height);

	// queries window's size
	void getSize(int *width, int *height) const;

	// returns the window's width
	int getWidth() const;

	// returns the window's height
	int getHeight() const;

	// sets window position and size
	void setBounds(int x, int y, int width, int height);

	// true if the window's visibility flag is on (will be rendered)
	bool isVisible() const;

	// returns the first visible window (topmost)
	glw_window *getTopmost() const;

	// returns the first visible window (topmost) at a specific coord
	glw_window *getTopmostAt(int x, int y) const;

	// sets window's title/text/caption
	void setCaption(const std::string & caption);

	// returns window's title/text/caption
	std::string getCaption();

	// sends and executes message
	bool sendMessage(glw_message message,
		glw_param lparam, glw_param wparam);

	// draws this window's children
	void renderChildren() const;

public:
	// draws the window & its children
	virtual void render() const;

protected:
	// kills the window and its subwindows
	bool wm_kill();

	// mouse button was pressed
	virtual bool wm_mousedown(glw_mousekey button, int x, int y);

	// mouse button was released
	virtual bool wm_mouseup(glw_mousekey button, int x, int y);

	// mouse was moved
	virtual bool wm_mousemove(int x, int y, int rx, int ry);

	// keyboard has been pressed
	virtual bool wm_keydown(glw_key key);

	// keyboard has been released
	virtual bool wm_keyup(glw_key key);

private:
	// children window's list
	std::list<glw_window*> m_Children;

protected:
	// visibility flag
	bool m_bVisible;

	// can we move this window with the mouse?
	bool m_bMoveable;

	// are we dragging the window?
	bool m_bDragging;

	// pointer to parent window
	glw_window *m_Parent;

	// window's coordinate
	glw_point m_Position;

	// window's size
	int m_iWidth;
	int m_iHeight;

	// holds window's title/caption/text
	std::string m_strCaption;

	// windows color (r, g, b, a)
	glw_color4 m_Color;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
