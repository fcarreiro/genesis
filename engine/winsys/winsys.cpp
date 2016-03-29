#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

#define s_width 800
#define s_height 600

/////////////////////////////////////////////////////////////////////

CWindowSystem::CWindowSystem()
{
	iMouseX = 0;
	iMouseY = 0;
	bEnabled = false;
	bMouseEnabled = false;
	lpBackground = NULL;
	ucBackgroundType = 0;
	memset(ucMouseButtons, 0, 4);
}

CWindowSystem::~CWindowSystem()
{
	Free();
}

/////////////////////////////////////////////////////////////////////

bool CWindowSystem::Startup()
{
	bool bRet;

	// create font
	bRet = fonFont.Create("arial", 12, FONT_BITMAP);
	if(!bRet) return false;

	// load mouse texture
	bRet = texMouse.Create("gfx/gui.pak/cursor.png", FIF_PNG, false);
	if(!bRet) return false;

	// center mouse
	iMouseX = 400;//g_Engine->GetScreenWidth() / 2;
	iMouseY = 300;//g_Engine->GetScreenHeight() / 2;

	// no errors
	return true;
}

/////////////////////////////////////////////////////////////////////

void CWindowSystem::Free()
{
	// free windows
	vWindows.clear();
	mRefTable.clear();

	// clear background
	FreeBackground();

	// free
	fonFont.Destroy();
	texMouse.Delete();

	// reset variables
	iMouseX = 0;
	iMouseY = 0;
	bEnabled = false;
	lpBackground = NULL;
	ucBackgroundType = 0;
	memset(ucMouseButtons, 0, 4);
}

/////////////////////////////////////////////////////////////////////

void CWindowSystem::Update()
{
	static int lastml = 0;

	// find special windows
	CWindow *topmostXY = NULL;
	CWindow *topmost = NULL;

	// set topmost window
	if(!vWindows.empty()) topmost = vWindows.back();

	// do we need the mouse?
	if(bMouseEnabled)
	{
		// get mouse information
		int mx, my;
		bool mouseleft = g_Input->GetMouseButton(SDL_BUTTON_LEFT);
		g_Input->GetMouseMotion(&mx, &my);

		// find topmost at mouse position
		for(int i = 0; i < vWindows.size(); ++i)
		{
			if(ext::is_between(iMouseX,
							   iMouseY,
							   vWindows[i]->GetX(),
							   vWindows[i]->GetY(),
							   vWindows[i]->GetX() + vWindows[i]->GetWidth(),
							   vWindows[i]->GetY() + vWindows[i]->GetHeight()))
			{
				topmostXY = vWindows[i];
			}
		}

		// if we are over a window check for click events
		if(topmostXY || topmost)
		{
			// check for left mouse down
			if(!lastml && mouseleft)
			{
				if(topmostXY) topmostXY->SendMessage(MWM_LBUTTONDOWN, iMouseX, iMouseY);
			}

			// check for left mouse up
			if(lastml && !mouseleft)
			{
				if(topmost) topmost->SendMessage(MWM_LBUTTONUP, iMouseX, iMouseY);
			}
		}

		// update mouse information
		iMouseX += mx;
		iMouseY += my;
		lastml = mouseleft;

		// mouse boundaries
		if(iMouseX < 0) iMouseX = 0;
		if(iMouseY < 0) iMouseY = 0;
		if(iMouseX > s_width) iMouseX = s_width;
		if(iMouseY > s_height) iMouseY = s_height;
	}

	// check for keyboard events
	char c = (char) g_Input->GetQueuedKey(); // !!!FIXME!!!
	if(c && topmost) topmost->SendMessage(MWM_KEYDOWN, c, 0);
}

/////////////////////////////////////////////////////////////////////

bool CWindowSystem::RenderEx(unsigned char a_flags)
{
	// render background ?
	if((a_flags & R_BACKGROUND) && lpBackground)
	{
		CTexture *bg = (CTexture*)lpBackground;

		if(ucBackgroundType == BG_TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);
			bg->Use();
			glColor3ub(255,255,255);

			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2i(0, 0);
				glVertex2i(0, s_height);

				glTexCoord2i(0, 1);
				glVertex2i(0, 0);

				glTexCoord2i(1, 0);
				glVertex2i(s_width, s_height);

				glTexCoord2i(1, 1);
				glVertex2i(s_width, 0);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
		else if(ucBackgroundType == BG_IMAGE)
		{
			CImage *bg = (CImage*)lpBackground;

			// start from s_width, s_height
			glRasterPos4i(0, s_height, 0, 1);

			switch(bg->GetBPP())
			{
			case 8:
				glDrawPixels(bg->GetWidth(), bg->GetHeight(), GL_LUMINANCE, GL_UNSIGNED_BYTE, bg->GetData());
				break;

			case 24:
				glDrawPixels(bg->GetWidth(), bg->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, bg->GetData());
				break;

			case 32:
				glDrawPixels(bg->GetWidth(), bg->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, bg->GetData());
				break;
			}
		}
	}

	// render windows ?
	if(a_flags & R_WINDOWS)
	{
		for(int i = 0; i < vWindows.size(); ++i)
		{
			vWindows[i]->Render();
		}
	}

	// render mouse ?
	if(a_flags & R_MOUSE && bMouseEnabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		glEnable(GL_TEXTURE_2D);
		texMouse.Use();
		glColor3ub(255,255,255);

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, 0);
			glVertex2i(iMouseX, iMouseY + 32);

			glTexCoord2i(0, 1);
			glVertex2i(iMouseX, iMouseY);

			glTexCoord2i(1, 0);
			glVertex2i(iMouseX + 32, iMouseY + 32);

			glTexCoord2i(1, 1);
			glVertex2i(iMouseX + 32, iMouseY);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	// no errors
	return true;
}

/////////////////////////////////////////////////////////////////////

void CWindowSystem::AddWindow(CWindow *win)
{
	// add id reference
	if(win->GetID() != 0)
	{
		mRefTable.insert(std::make_pair(win->GetID(), win));
	}

	// add window
	vWindows.push_back(win);
}

/////////////////////////////////////////////////////////////////////

void CWindowSystem::RemoveWindow(CWindow *win)
{
	if(win->GetID() != 0)
	{
		mRefTable.erase(win->GetID());
	}

	ext::remove(vWindows, win);
}

/////////////////////////////////////////////////////////////////////

CWindow* CWindowSystem::FindWindow(unsigned long id)
{
	if(mRefTable.find(id) != mRefTable.end())
	{
		return mRefTable[id];
	}
	else
	{
		return NULL;
	}
}

/////////////////////////////////////////////////////////////////////

CWindow* CWindowSystem::FindWindow(const char *caption)
{
	for(int i = 0; i < vWindows.size(); ++i)
	{
		if(strcmp(vWindows[i]->GetText(), caption) == NULL) return vWindows[i];
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
