#include "../precompiled/stdafx.h"
#include "../engine/base.h"

/////////////////////////////////////////////////////////////

CTextBox::CTextBox()
{
	bIsPassword = false;
	iMaxLen = 0;
}

CTextBox::~CTextBox()
{
}

/////////////////////////////////////////////////////////////

bool CTextBox::wm_lbuttondown(int a_x, int a_y)
{
	if(!IsVisible()) return true;

	BringToTop();

	return true;
}

bool CTextBox::wm_keydown(char key)
{
	if(key > 31)
	{
		if(iMaxLen)
		{
			if(strCaption.length() >= iMaxLen) return true;
		}

		strCaption += key;
	}
	else
	{
		if(key == 8 && strCaption.length() > 0) strCaption.erase(strCaption.end() - 1);
	}

	return true;
}

/////////////////////////////////////////////////////////////

bool CTextBox::Render()
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

	// draw background
	if(IsActive()) glColor3ub(0, 0, 0);
	else glColor3ub(255, 255, 255);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(px + x, py + y);
		glVertex2i(px + x, py + y + height);
		glVertex2i(px + x + width, py + y);
		glVertex2i(px + x + width, py + y + height);
	glEnd();

	// draw borderlines
	glBegin(GL_LINE_STRIP);
		glColor3ub(120, 120, 120);

		glVertex2i(px + x, py + y + height);
		glVertex2i(px + x, py + y);
		glVertex2i(px + x + width, py + y);

		glColor3ub(255, 255, 255);

		glVertex2i(px + x + width, py + y + height);
		glVertex2i(px + x, py + y + height);
	glEnd();

	// draw text
	if(strCaption.length() == 0) return true;

	// set color
	if(IsActive()) glColor3ub(255, 255, 255);
	else glColor3ub(0, 0, 0);

	// line pointer
	std::string strLine;

	// *'s or text ?
	if(bIsPassword)
	{
		for(int i = 0; i < strCaption.length(); ++i) strLine += '*';
	}
	else
	{
		strLine = strCaption;
	}

	int linewidth = g_Winsys->fonFont.LineWidth(strLine.c_str());
	int n = 0;

	// check if the line goes out of the textbox (horizontal)
	if(linewidth > width - 10)
	{
		for(n = 0; n < strLine.length(); ++n)
		{
			linewidth = g_Winsys->fonFont.LineWidth(strLine.c_str() + n);
			if(linewidth < width - 10) break;
		}
	}

	g_Winsys->fonFont.Print(px + x + 5, py + y + height / 2 + 3, strLine.c_str() + n);

	// no errors
	return true;
}

/////////////////////////////////////////////////////////////