#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

CWindow::CWindow()
{
	id = 0;
	parent = NULL;
	isshown = true;
	ismoving = false;
	x = 0; y = 0;
	width = 0; height = 0;
}

CWindow::~CWindow()
{
	FreeSubwins();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::Center()
{
	if(parent)
	{
		x = (parent->GetWidth() / 2) - (width / 2);
		y = (parent->GetHeight() / 2) - (height / 2);
	}
	else
	{
		x = 400 - (width / 2);
		y = 300 - (height / 2);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::FreeSubwins()
{
	for(unsigned long i = 0; i < vSubWindows.size(); ++i)
	{
		delete vSubWindows[i];
	}

	vSubWindows.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindow::BringToTop()
{
	if(IsActive()) return;

	if (parent) {
		CWindow *p = parent;
		p->RemoveWindow(this);
		p->AddWindow(this);
	}
	else
	{
		g_Winsys->RemoveWindow(this);
		g_Winsys->AddWindow(this);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::IsActive()
{
	if(parent)
	{
		return (this == parent->vSubWindows.back());
	}
	else
	{
		return (this == g_Winsys->vWindows.back());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

CWindow *CWindow::GetTopmost()
{
	if(!vSubWindows.size()) return NULL;
	return vSubWindows[vSubWindows.size() - 1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

CWindow *CWindow::GetTopmostXY(int a_x, int a_y)
{
	CWindow *r = NULL;

	for(int i = 0; i < vSubWindows.size(); ++i)
	{
		if(ext::is_between(a_x, a_y,
						   vSubWindows[i]->GetX(),
						   vSubWindows[i]->GetY(),
						   vSubWindows[i]->GetX() + vSubWindows[i]->GetWidth(),
						   vSubWindows[i]->GetY() + vSubWindows[i]->GetHeight()))
		{
			r = vSubWindows[i];
		}
	}

	return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::SendMessage(unsigned char message, unsigned long lparam, unsigned long wparam)
{
	// EXECUTE
	switch(message) {
		case MWM_KEYDOWN:
			if(!wm_keydown(lparam)) return false;
			break;
		case MWM_KILL:
			if(!wm_kill()) return false;
			break;
		case MWM_LBUTTONDOWN:
			if(!wm_lbuttondown(lparam, wparam)) return false;
			break;
		case MWM_LBUTTONUP:
			if(!wm_lbuttonup(lparam, wparam)) return false;
			break;
		default:
			// UNKNOWN MESSAGE
			return false;
	}

	// DISPATCH
	for(unsigned long i = 0; i < vSubWindows.size(); ++i)	{
		if(!vSubWindows[i]->SendMessage(message, lparam, wparam)) return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::wm_kill()
{
	if(IsActive())
	{
		parent->RemoveWindow(this);
		return false;
	}
	else
	{
		return true;
	}
}

bool CWindow::wm_lbuttondown(int a_x, int a_y)
{
	if(!IsVisible()) return true;

	if( a_x < x + width &&
		a_x > x &&
		a_y < y + height &&
		a_y > y)
	{
		if(a_y < y + 15) ismoving=true;

		BringToTop();
	}

	return true;
}

bool CWindow::wm_lbuttonup(int a_x, int a_y)
{
	ismoving = false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CWindow::Render()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////