#ifndef __FONT_BASE_H__
#define __FONT_BASE_H__

//////////////////////////////////////////////////////////////////////////
// Font type constants
//////////////////////////////////////////////////////////////////////////

enum font_type {
	FONT_AUTO,
	FONT_BITMAP,
	FONT_TEXTURE
};

//////////////////////////////////////////////////////////////////////////
// CFontBase class
//////////////////////////////////////////////////////////////////////////

class CFontBase
{
public:
	// constructor & destrutor
	CFontBase();
	virtual ~CFontBase();

	// returns the font's average width
	int GetWidth() const { return m_iAverageWidth; }

	// returns the font height
	int GetHeight() const { return m_iHeight; }

	// calculates the line width in pixels
	unsigned long LineWidth(const std::string & text) const;

	// creates the font
	virtual bool Create(const std::string & strFontname, int iHeight, int iWeight) = 0;

	// destroys the font
	virtual void Destroy();

	// prints text in the screen
	virtual void Print(float x, float y, const std::string & text) = 0;

	// is the font loaded?
	virtual bool IsLoaded() = 0;

	// proxy
	void DecCount() { m_iRefCount--; }
	void IncCount() { m_iRefCount++; }

protected:
	// how many times is this font used
	int m_iRefCount;

	// if we are are a duplicate then who's my father?
	CFontBase *m_pRealFont;

	// each font has a width
	std::vector<int> m_iWidths;

	// so there is an average
	int m_iAverageWidth;

	// but their height is all the same
	int m_iHeight;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
