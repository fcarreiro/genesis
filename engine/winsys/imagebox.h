#ifndef __IMAGEBOX_H__
#define __IMAGEBOX_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CImageBox : public CWindow
{
public:
	CImageBox();
	virtual ~CImageBox();

	// functions
	virtual bool Render();

	virtual bool wm_lbuttondown(int a_x, int a_y) { return true; }
	virtual bool wm_lbuttonup(int a_x, int a_y) { return true; }
	virtual bool wm_keydown(char key) { return true; }

	// IMAGEBOX's
	virtual void SetTexture(CTexture *tex) { img = tex; }

private:
	CTexture *img;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif