#include "../precompiled/stdafx.h"
#include "../engine/base.h"

//////////////////////////////////////////////////////////////////////////
// CFontBase default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CFontBase::CFontBase() : m_iAverageWidth(0),
m_iHeight(0), m_iRefCount(0), m_pRealFont(NULL),
m_iWidths(256, 0)
{
	// initialize variables
}

CFontBase::~CFontBase()
{
	// destroy the font
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// LineWidth() : Calculates the line width in pixels
//////////////////////////////////////////////////////////////////////////

unsigned long CFontBase::LineWidth(const std::string & text) const
{
	unsigned long width = 0;

	// get width
	for(unsigned int i = 0; i < text.length(); ++i)
	{
		width += m_iWidths[text[i]];
	}

	// return width
	return width;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Destroys the font
//////////////////////////////////////////////////////////////////////////

void CFontBase::Destroy()
{
	// remove from database
	g_FontMgr->Remove(this);

	// reset variables
	m_iAverageWidth = 0;
	m_iHeight = 0;
	m_iRefCount = 0;
	m_pRealFont = NULL;
	
	// reset vector
	std::fill(m_iWidths.begin(), m_iWidths.end(), 0);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
