#ifndef __CONSOLE_H__
#define __CONSOLE_H__

//////////////////////////////////////////////////////////////////////////
// Console item type constants
//////////////////////////////////////////////////////////////////////////

enum console_item_type_t
{
	VAR_UCHAR,
	VAR_CHAR,
	VAR_CHARP,
	VAR_UINT,
	VAR_INT,
	VAR_FLOAT,
	VAR_STRING,
	VAR_FUNCTION
};

//////////////////////////////////////////////////////////////////////////
// Console function pointer
//////////////////////////////////////////////////////////////////////////

typedef void (*console_fnc)(const std::vector<std::string> &);

//////////////////////////////////////////////////////////////////////////
// Item structure
//////////////////////////////////////////////////////////////////////////

typedef struct {
	// item name
	std::string name;

	// item type
	console_item_type_t type;

	// function callback or pointer to the value
	union
	{
		console_fnc callback;
		void *var;
	};
} console_item_t;

//////////////////////////////////////////////////////////////////////////
// CConsole class
//////////////////////////////////////////////////////////////////////////

class CConsole
{
public:
	// constructor & destructor
	CConsole();
	~CConsole();

public:
	// creates a console in the given coords
	bool Initialize(int x, int y, int width, int height,
		const std::string & strFontname);

	// adds an item to the console
	void AddItem(const std::string & strName,
		void *pointer, console_item_type_t type);

	// deletes an item from the console
	void DeleteItem(const std::string & strName);

	// executes an item in the console
	bool ExecuteItem(const std::string & strCommand);

	// free and destroy the console
	void Free();

	// draw the console
	void Render();
	
	// activates the console
	void Active(bool bActive) { m_bActive = bActive; }

	// enables the console
	void Enable(bool bEnabled) { m_bEnabled = bEnabled; }

	// switches enabled state
	void Switch();

	// sets the new console position
	void SetPosition(int x, int y) { m_iX = x; m_iY = y; }

	// sets new console size
	void SetSize(int iWidth, int iHeight) {
		m_iWidth = iWidth; m_iHeight = iHeight; }

	// sets console background color
	void SetBackColor(float r, float g, float b, float a) {
		m_bgR = r; m_bgG = g; m_bgB = b; m_bgA = a; }

	// sets the console's background
	void SetBackground(const std::string & strImage);

	// sets default text color
	void SetTextColor(float r, float g, float b, float a) {
		m_textR = r; m_textG = g; m_textB = b; m_textA = a; }

	// sets line buffer size
	void SetBufferSize(int size);

	// clears the console's text buffer
	void Clear() { m_textBuffer.clear(); }

	// tells the console that a key has been pressed
	void PassKey(char c);

	// returns true if the console is active
	bool IsActive() const { return m_bActive; }

	// returns true if the console is enabled
	bool IsEnabled() const { return m_bEnabled; }
	
	// prints text on the console
	void Print(const std::string & strTxt);

private:
	// define item database type
	typedef std::vector<console_item_t> ITEM_DB;

private:
	// changes the text written to a previous one
	void History(int i);

private:
	// holds the text buffer
	std::vector<std::string> m_textBuffer;

	// holds the buffer for the entered commands
	std::vector<std::string> m_lineBuffer;

	// holds the command line
	std::string	m_commandLine;

	// holds the list of items
	ITEM_DB m_Items;

	// the background (if needed)
	CTexture m_Background;

	// the font
	CFont m_Font;

	// active state
	bool m_bActive;

	// enabled state
	bool m_bEnabled;

	// actual line index (in history)
	int	m_iLineIndex;

	// console background color
	float m_bgR, m_bgG, m_bgB, m_bgA;

	// default text color
	float m_textR, m_textG, m_textB, m_textA;

	// console's upper cords
	int	m_iX, m_iY;

	// size
	int	m_iWidth, m_iHeight;

	// how many lines can we store
	int	m_iMaxLines;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
