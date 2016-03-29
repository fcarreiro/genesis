#include "../precompiled/stdafx.h"
#include "../engine/base.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

CStatic::CStatic()
{
	m_colR = m_colG = m_colB = 0;
	memset(m_szFontname, 0, sizeof(m_szFontname));
}

CStatic::~CStatic()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CStatic::Render()
{
	// are we visible ?
	if(!IsVisible()) return true;

	// parent x and y
	int px = 0;
	int py = 0;

	if(parent)
	{
		px = parent->GetX();
		py = parent->GetY();
	}

	// set color
	glColor3ub(m_colR, m_colG, m_colB);

	// set string pointer
	char *pter = (char *) strCaption.c_str();
	int addy = 0;

	// choose font
	CFont *font;

	if(strlen(m_szFontname) == 0)
	{
		font = &g_Winsys->fonFont;
	}
	else
	{
		return false;
	}

	// print
	for(;;)
	{
		font->Print(px + x, py + y + addy, pter);

		if(strstr(pter, "\n"))
		{
			addy += g_Winsys->fonFont.GetHeight();
			pter = strstr(pter, "\n") + 1;
		}
		else
		{
			break;
		}
	}

	// no errors
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////