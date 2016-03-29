#ifndef __FONT_H__
#define __FONT_H__

//////////////////////////////////////////////////////////////////////////
// CFont class
//////////////////////////////////////////////////////////////////////////

class CFont
{
public:
	// constructor & destrutor
	CFont();
	virtual ~CFont();

	// overloaded creation constructor
	CFont(const std::string & strFontname, int height,
		font_type type = FONT_AUTO, int weight = 400);

public:
	// creates the font
	bool Create(const std::string & strFontname, int height,
		font_type type = FONT_AUTO, int weight = 400);

	// destroys the font
	void Destroy();

	// returns the font's average width
	int GetWidth() { if(m_Font) return m_Font->GetWidth(); else return 0; }

	// returns the font height
	int GetHeight() { if(m_Font) return m_Font->GetHeight(); else return 0; }

	// calculates the line width in pixels
	unsigned long LineWidth(const std::string & text) {
		if(m_Font) return m_Font->LineWidth(text); else return 0; }

	// prints text in the screen
	void Print(float x, float y, const std::string & text) {
		if(m_Font) m_Font->Print(x, y ,text); }

	// is the font loaded?
	bool IsLoaded() { if(m_Font) return m_Font->IsLoaded(); else return false; }

	// proxy
	void DecCount() { if(m_Font) m_Font->DecCount(); }
	void IncCount() { if(m_Font) m_Font->IncCount(); }

private:
	// real font
	CFontBase *m_Font;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
