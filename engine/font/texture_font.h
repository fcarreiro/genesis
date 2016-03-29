#ifndef __CTEXTUREFONT_H__
#define __CTEXTUREFONT_H__

//////////////////////////////////////////////////////////////////////////
// CTextureFont class
//////////////////////////////////////////////////////////////////////////

class CTextureFont : public CFontBase
{
public:
	// constructor & destrutor
	CTextureFont();
	virtual ~CTextureFont();

	// creates the font
	virtual bool Create(const std::string & strFontname, int iHeight, int iWeight);

	// destroys the font
	virtual void Destroy();

	// prints text in the screen
	virtual void Print(float x, float y, const std::string & text);

	// is the font loaded?
	virtual bool IsLoaded() { return m_texFont.IsLoaded(); }

private:
	// prints ONE character in the screen
	void PrintChar(int x, int y, unsigned char c) const;

	// variables
	CTexture m_texFont;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif