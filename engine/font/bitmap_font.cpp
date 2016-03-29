#include "../precompiled/stdafx.h"
#include "../engine/base.h"

// we just process this file under win32
#if BITMAP_FONT_AVAILABLE == 1

//////////////////////////////////////////////////////////////////////////
// CBitmapFont default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CBitmapFont::CBitmapFont() : m_uiBase(0)
{
	// set variales to zero
}

CBitmapFont::~CBitmapFont()
{
	// destroy the font
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates a bitmap font (just under win32)
//////////////////////////////////////////////////////////////////////////

bool CBitmapFont::Create(const std::string & strFontname, int iHeight, int iWeight)
{
	HFONT oldfont;
	HFONT font;

	// create base for 256 chars
	m_uiBase = glGenLists(256);

	// create font
	font = CreateFont( -iHeight,
							  0,
							  0,
							  0,
					    iWeight,
						  false,
						  false,
						  false,
				   ANSI_CHARSET,
				  OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
	FF_DONTCARE | DEFAULT_PITCH,
		   strFontname.c_str());

	// font?
	if(!font)
	{
		Destroy();
		return false;
	}

	// build it
	ABC abcMetrics[256];
	HDC hDC = GetDC(GetForegroundWindow());
	oldfont = (HFONT) SelectObject(hDC, font);

	// use fonts as bitmaps
	wglUseFontBitmaps(hDC, 0, 255, m_uiBase);

	// get each font's width
	GetCharABCWidths(hDC, 0, 255, (ABC *) abcMetrics);

	// optimize width's access
	for(int i = 0; i < 256; ++i)
	{
		m_iWidths[i] += abcMetrics[i].abcA;
		m_iWidths[i] += abcMetrics[i].abcB;
		m_iWidths[i] += abcMetrics[i].abcC;

		m_iAverageWidth += m_iWidths[i];
		m_iAverageWidth /= 2;
	}

	// set back last object & destroy font
	SelectObject(hDC, oldfont);
	DeleteObject(font);
	
	// set attributes
	m_iHeight = abs(iHeight);

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Destroys the font
//////////////////////////////////////////////////////////////////////////

void CBitmapFont::Destroy()
{
	// delete opengl's lists
	if(m_uiBase)
	{
		glDeleteLists(m_uiBase, 256);
	}

	// reset variables
	m_uiBase = 0;
}

//////////////////////////////////////////////////////////////////////////
// Print() : Prints one string in the screen
//////////////////////////////////////////////////////////////////////////

void CBitmapFont::Print(float x, float y, const std::string & text)
{
	static size_t len;
	static size_t newline;

	// set position
	glRasterPos2f(x, y);

	// draw
	len = text.length();
	newline = text.find("\n");

	// set font list
	glPushAttrib(GL_LIST_BIT);
	glListBase(m_uiBase);

	// print character string
	glCallLists(newline != std::string::npos ? newline : len,
		GL_UNSIGNED_BYTE, text.c_str());

	// restore old list
	glPopAttrib();
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
