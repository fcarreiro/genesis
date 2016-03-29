#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CTextBox : public CWindow
{
public:
	CTextBox();
	virtual ~CTextBox();

	// FUNCTIONS
	virtual bool Render();

	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_lbuttonup(int a_x, int a_y) { return true; }
	virtual bool wm_keydown(char key);

	// TEXTBOXE's
	virtual void SetMaxLength(unsigned long a_len) { iMaxLen = a_len; }
	virtual void SetPassword(bool p) { bIsPassword = p;}

private:
	bool bIsPassword;
	int iMaxLen;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif