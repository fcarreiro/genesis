#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CCheckBox : public CWindow
{
public:
	CCheckBox();
	virtual ~CCheckBox();

	// FUNCTIONS
	virtual bool Render();

	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_keydown(char key) { return true; }
	
	// CHECKBOXE's
	virtual void SetState(bool s) { bState = s; }
	virtual bool GetState() { return bState; }

private:
	bool bState;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif