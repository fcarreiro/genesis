#include "../precompiled/stdafx.h"
#include "../engine/base.h"

/////////////////////////////////////////////////////////////

CCheckBox::CCheckBox()
{
	bState = false;
}

CCheckBox::~CCheckBox()
{
}

/////////////////////////////////////////////////////////////

bool CCheckBox::Render()
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
	glColor3ub(255, 255, 255);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(px + x, py + y);
		glVertex2i(px + x, py + y + height);
		glVertex2i(px + x + width, py + y);
		glVertex2i(px + x + width, py + y + height);
	glEnd();

	// if our state is pressed draw the circle
	if(bState)
	{
		glPointSize(width / 2);
		glColor3ub(0, 0, 0);

		glBegin(GL_POINTS);
			glVertex2i(px + x + (width / 2), py + y + (height / 2));
		glEnd();
	}

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

	// no errors
	return true;
}

/////////////////////////////////////////////////////////////

bool CCheckBox::wm_lbuttondown(int a_x, int a_y)
{
	// focus please...
	BringToTop();

	// push me!
	bState = !bState;

	// ok, ok, continue
	return true;
}

/////////////////////////////////////////////////////////////