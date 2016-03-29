#ifndef __GLW_LABEL_H__
#define __GLW_LABEL_H__

//////////////////////////////////////////////////////////////////////////
// glw_label class
//////////////////////////////////////////////////////////////////////////

class glw_label : glw_window
{
public:
	// constructor & destructor
	glw_label();
	virtual ~glw_label();

public:
	// draws the window
	virtual void render() const;

protected:
	// mouse button was pressed
	virtual bool wm_mousedown(glw_mousekey button, int x, int y) { return true; }

	// mouse button was released
	virtual bool wm_mouseup(glw_mousekey button, int x, int y) { return true; }

	// keyboard has been pressed
	virtual bool wm_keydown(glw_key key) { return true; }

	// keyboard has been released
	virtual bool wm_keyup(glw_key key) { return true; }
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
