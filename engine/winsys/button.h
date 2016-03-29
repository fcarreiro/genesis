#ifndef __BUTTON_H__
#define __BUTTON_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CButton : public CWindow
{
public:
	CButton();
	virtual ~CButton();

	// FUNCTIONS
	virtual bool Render();

	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_lbuttonup(int a_x, int a_y);
	virtual bool wm_keydown(char key) { return true; }

	// BUTTON's
	virtual void SetOnClick(fncCallback c) { OnClick = c; }

	// STATIC's
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b) { colR = r; colG = g; colB = b; }

private:
	bool bPushed;
	unsigned char colR;
	unsigned char colG;
	unsigned char colB;
	fncCallback OnClick;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif