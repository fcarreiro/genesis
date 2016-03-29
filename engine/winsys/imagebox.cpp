#include "../precompiled/stdafx.h"
#include "../engine/base.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

CImageBox::CImageBox()
{
	img = NULL;
}

CImageBox::~CImageBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CImageBox::Render()
{
	// are we visible ?
	if(!IsVisible() || !img) return true;

	// blend enabled?
	GLboolean blend = glIsEnabled(GL_BLEND);

	// parent x and y
	int px = 0;
	int py = 0;

	if(parent)
	{
		px = parent->GetX();
		py = parent->GetY();
	}

	// enable blending
	if(!blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// draw image
	glEnable(GL_TEXTURE_2D);
	img->Use();
	glColor3ub(255, 255, 255);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 0);
		glVertex2i(px + x, py + y + height);

		glTexCoord2i(0, 1);
		glVertex2i(px + x, py + y);

		glTexCoord2i(1, 0);
		glVertex2i(px + x + width, py + y + height);

		glTexCoord2i(1, 1);
		glVertex2i(px + x + width, py + y);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// disable blending
	if(!blend)
	{
		glDisable(GL_BLEND);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////