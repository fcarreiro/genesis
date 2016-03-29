#ifndef __WINSYS_H__
#define __WINSYS_H__

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define BG_TEXTURE	0
#define BG_IMAGE	1

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CWindowSystem
{
public:
	// friends
	friend class CWindow;
	friend class CStatic;
	friend class CButton;
	friend class CTextBox;

	// constructor & destructor
	CWindowSystem();
	~CWindowSystem();

	// functions
	bool Startup();
	void Free();

	void Update();
	bool Render() { return RenderEx(R_BACKGROUND | R_WINDOWS | R_MOUSE); }
	bool RenderEx(unsigned char a_flags);

	void SetBackground(CTexture *tex) { lpBackground = tex; ucBackgroundType = BG_TEXTURE; }
	void SetBackground(CImage *img) { lpBackground = img; ucBackgroundType = BG_IMAGE; }
	void FreeBackground() { lpBackground = NULL; }

	void PassKey(char key);

	void Enable(bool b) { bEnabled = b; }
	void EnableMouse(bool b) { bMouseEnabled = b; }
	bool IsEnabled() { return bEnabled; }
	bool IsMouseEnabled() { return bMouseEnabled; }

	CWindow* FindWindow(unsigned long id);
	CWindow* FindWindow(const char *caption);

	void AddWindow(CWindow *win);
	void RemoveWindow(CWindow *win);
	void ClearWindows() { vWindows.clear(); }

private:
	int iMouseX;
	int iMouseY;
	unsigned char ucMouseButtons[4];

	bool bEnabled;
	bool bMouseEnabled;
	CFont fonFont;
	unsigned char ucBackgroundType;
	void *lpBackground;
	CTexture texMouse;

	std::vector<CWindow*> vWindows;
	std::map<unsigned long, CWindow*> mRefTable;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif
