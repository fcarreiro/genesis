#include "../precompiled/stdafx.h"
#include "../engine/base.h"

//////////////////////////////////////////////////////////////////////////
// CFont default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CFont::CFont() : m_Font(NULL)
{
	// set variables to zero
}

CFont::~CFont()
{
	// destroy the font
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// CFont overloaded creation constructor
//////////////////////////////////////////////////////////////////////////

CFont::CFont(const std::string & strFontname, int height,
			 font_type type, int weight)
{
	// set variables to zero
	m_Font = NULL;

	// route to creation
	Create(strFontname, height, type, weight);
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates the font
//////////////////////////////////////////////////////////////////////////

bool CFont::Create(const std::string & strFontname, int height,
				   font_type type, int weight)
{
	// if we have one then its an error
	if(m_Font) return false;

	// bitmap font
	#if BITMAP_FONT_AVAILABLE == 1
	if(type == FONT_BITMAP)
	{
		// allocate bitmap font
		m_Font = new CBitmapFont();
		if(!m_Font) return false;

		// create bitmap font
		return m_Font->Create(strFontname, height, weight);
	}
	#endif

	// else create texture font
	m_Font = new CTextureFont();
	if(!m_Font) return false;

	// create bitmap font
	return m_Font->Create(strFontname, height, 0);
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Destroys the font
//////////////////////////////////////////////////////////////////////////

void CFont::Destroy()
{
	// destroy real font
	delete m_Font;
	m_Font = NULL;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
