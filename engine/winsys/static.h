#ifndef __STATIC_H__
#define __STATIC_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CStatic : public CWindow
{
public:
	CStatic();
	virtual ~CStatic();

	// functions
	virtual bool Render();

	virtual bool wm_lbuttondown(int a_x, int a_y) { return true; }
	virtual bool wm_lbuttonup(int a_x, int a_y) { return true; }
	virtual bool wm_keydown(char key) { return true; }

	// STATIC's
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b) { m_colR = r; m_colG = g; m_colB = b; }
	virtual void SetFont(const char *szFontname) { if(szFontname) strcpy(m_szFontname, szFontname); else memset(m_szFontname, 0, sizeof(m_szFontname)); }

private:
	char m_szFontname[60];

	unsigned char m_colR;
	unsigned char m_colG;
	unsigned char m_colB;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif