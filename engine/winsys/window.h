#ifndef __WINDOW_H__
#define __WINDOW_H__

////////////////////////////////////////////////////////////////////////////////////////////////////

class CWindow {
public:
	CWindow();
	virtual ~CWindow();

	// FUNCTIONS
	void AddWindow(CWindow *w) { vSubWindows.push_back(w); }
	void RemoveWindow(CWindow *w) { ext::remove(vSubWindows, w); }
	void FreeSubwins();

	virtual bool Render();

	void Show() { isshown = true; }
	void Hide() { isshown = false; }
	void BringToTop();
	
	void Center();

	unsigned long GetID() { return id; }
	void SetID(unsigned long a_id) { id = a_id; }

	bool IsActive();
	bool IsVisible() { return isshown; }
	
	CWindow *GetParent() { return parent; }
	CWindow *GetTopmost();
	CWindow *GetTopmostXY(int a_x, int a_y);

	void SetParent(CWindow *w) { parent = w; }
	void SetCaption(const char *a_caption) { strCaption = a_caption; }
	void SetPos(int a_x, int a_y) { x = a_x; y = a_y; }
	void SetSize(unsigned long a_width, unsigned long a_height) { width = a_width; height = a_height; }

	const char *GetText() { return strCaption.c_str(); }
	int GetX() { return x; }
	int GetY() { return y; }
	unsigned long GetWidth() { return width; }
	unsigned long GetHeight() { return height; }

	bool SendMessage(unsigned char message, unsigned long lparam, unsigned long wparam);
	bool wm_kill();
	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_lbuttonup(int a_x, int a_y);
	virtual bool wm_keydown(char key) { return true; }

	// STATIC's
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b) { }
	virtual void SetFont(const char *szFontname) { }

	// BUTTON's
	virtual void SetOnClick(fncCallback c) {}

	// TEXTBOXE's
	virtual void SetMaxLength(unsigned long a_len) {}
	virtual void SetPassword(bool p) {}

	// CHECKBOXE's
	virtual void SetState(bool s) {}
	virtual bool GetState() { return false; }

	// IMAGEBOX's
	virtual void SetTexture(CTexture *tex) {}

	// VARIABLES
private:
	std::vector<CWindow*> vSubWindows;
	bool ismoving;

protected:
	CWindow *parent;
	std::string strCaption;
	bool isshown;
	int x;
	int y;
	unsigned long width;
	unsigned long height;
	unsigned long id;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif