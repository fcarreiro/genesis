#ifndef __BITMAPFONT_H__
#define __BITMAPFONT_H__

//////////////////////////////////////////////////////////////////////////
// CBitmapFont class
//////////////////////////////////////////////////////////////////////////

class CBitmapFont : public CFontBase
{
public:
	// constructor & destrutor
	CBitmapFont();
	virtual ~CBitmapFont();

	// creates the font
	virtual bool Create(const std::string & strFontname, int iHeight, int iWeight);

	// destroys the font
	virtual void Destroy();

	// prints text in the screen
	virtual void Print(float x, float y, const std::string & text);

	// is the font loaded?
	virtual bool IsLoaded() { return (m_uiBase != 0); }

private:
	// opengl's list base
	GLuint m_uiBase;
};

////////////////////////////////////////////////////////////////////////
// End
////////////////////////////////////////////////////////////////////////

#endif