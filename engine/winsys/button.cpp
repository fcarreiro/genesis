#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

/////////////////////////////////////////////////////////////

CButton::CButton()
{
	bPushed = false;
	OnClick = NULL;
	colR = colG = colB = 255;
}

CButton::~CButton()
{
}

/////////////////////////////////////////////////////////////

bool CButton::Render()
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
	if(!bPushed) glColor3ub(0, 0, 0);
	else glColor3ub(170, 170, 170);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(px + x, py + y);
		glVertex2i(px + x, py + y + height);
		glVertex2i(px + x + width, py + y);
		glVertex2i(px + x + width, py + y + height);
	glEnd();

	// draw borderlines
	glBegin(GL_LINE_STRIP);
		if(!bPushed) glColor3ub(255, 255, 255);
		else glColor3ub(120, 120, 120);

		glVertex2i(px + x, py + y + height);
		glVertex2i(px + x, py + y);
		glVertex2i(px + x + width, py + y);

		if(!bPushed) glColor3ub(120, 120, 120);
		else glColor3ub(255, 255, 255);

		glVertex2i(px + x + width, py + y + height);
		glVertex2i(px + x, py + y + height);
	glEnd();

	// draw text
	int linewidth = g_Winsys->fonFont.LineWidth(strCaption.c_str());

	glColor3ub(colR, colG, colB);

	g_Winsys->fonFont.Print(px + x + width / 2 - linewidth / 2, py + y + height / 2 + 3, strCaption);

	// no errors
	return true;
}

/////////////////////////////////////////////////////////////

bool CButton::wm_lbuttondown(int a_x, int a_y)
{
	// focus please...
	BringToTop();

	// push me!
	bPushed = true;

	// play "tick" sound
	//g_Sound->PlaySound("tick.wav");

	// ok, ok, continue
	return true;
}

bool CButton::wm_lbuttonup(int a_x, int a_y)
{
	// push me not!
	bPushed = false;

	// parent x and y
	int px = 0;
	int py = 0;

	if(parent)
	{
		px = parent->GetX();
		py = parent->GetY();
	}

	// inside the button? execute handler!
	if(OnClick &&
		ext::is_between(a_x, a_y,
						px + x,
						py + y,
						px + x + width,
						py + y + height))
	{
		OnClick();
	}

	// all right
	return true;
}

/////////////////////////////////////////////////////////////