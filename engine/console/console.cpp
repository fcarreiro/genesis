#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// CConsole default construtor & destructor
//////////////////////////////////////////////////////////////////////////

CConsole::CConsole() : m_iLineIndex(0),
m_iX(0), m_iY(0), m_iWidth(0),
m_iHeight(0), m_bActive(false),
m_bEnabled(false), m_textR(0.0f),
m_textG(0.0f), m_textB(0.0f),
m_textA(1.0f), m_bgR(1.0f),
m_bgG(1.0f), m_bgB(1.0f),
m_bgA(1.0f), m_iMaxLines(50)
{
	// initialize variables to default
}

CConsole::~CConsole()
{
	// free :-D
	Free();
}

//////////////////////////////////////////////////////////////////////////
// Initialize() : Startups the console
//////////////////////////////////////////////////////////////////////////

bool CConsole::Initialize(int x, int y, int width, int height,
						  const std::string & strFontname)
{
	// set properties
	m_iX = x;
	m_iY = y;
	m_iWidth = width;
	m_iHeight = height;

	// create font
	return m_Font.Create(strFontname, 12, FONT_BITMAP);
}

//////////////////////////////////////////////////////////////////////////
// Free() : Shuts down and frees all the data
//////////////////////////////////////////////////////////////////////////

void CConsole::Free()
{
	// free background texture
	m_Background.Delete();

	// free console font
	m_Font.Destroy();

	// delete all items
	m_Items.clear();

	// reasign
	m_iLineIndex = 0;
	m_iX = 0;
	m_iY = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_bActive = false;
	m_bEnabled = false;
	m_textR = 0.0f;
	m_textG = 0.0f;
	m_textB = 0.0f;
	m_textA = 1.0f;
	m_bgR = 1.0f;
	m_bgG = 1.0f;
	m_bgB = 1.0f;
	m_bgA = 1.0f;
	m_iMaxLines = 50;
}

//////////////////////////////////////////////////////////////////////////
// Render() : Draws the console
//////////////////////////////////////////////////////////////////////////

void CConsole::Render()
{
	// set blending mode for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// background color
	glColor4f(m_bgR, m_bgG, m_bgB, m_bgA);	

	// render background
	if(m_Background.IsLoaded())
	{
		// use background
		glEnable(GL_TEXTURE_2D);
		m_Background.Use();

		// render
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, 1);
			glVertex2i(m_iX, m_iY);

			glTexCoord2i(0, 0);
			glVertex2i(m_iX, m_iY + m_iHeight);

			glTexCoord2i(1, 1);
			glVertex2i(m_iX + m_iWidth, m_iY);

			glTexCoord2i(1, 0);
			glVertex2i(m_iX + m_iWidth, m_iY + m_iHeight);
		glEnd();
		
		// do not use textures any more
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		// render background
		glBegin(GL_TRIANGLE_STRIP);
			glVertex2i(m_iX, m_iY);
			glVertex2i(m_iX, m_iY + m_iHeight);
			glVertex2i(m_iX + m_iWidth, m_iY);
			glVertex2i(m_iX + m_iWidth, m_iY + m_iHeight);
		glEnd();
	}

	// set default text color
	glColor4f(m_textR, m_textG, m_textB, m_textA);

	// if we are writing in the console we print the command line
	if(m_bActive)
	{
		unsigned int n;

		// check if the line goes out of the console (horizontal) and trim it
		for(n = 0; m_Font.LineWidth(m_commandLine.substr(n, std::string::npos)) > m_iX + m_iWidth - 20; ++n);

		// format the commandline
		std::ostringstream o;
		o << "]" << m_commandLine.substr(n, std::string::npos);

		// print the commandline
		m_Font.Print(m_iX + 10, m_iY + m_iHeight - m_Font.GetHeight(), o.str());
	}

	// print the textbuffer
	if(m_textBuffer.size() > 0)
	{
		int pos;
		
		// leave space for the command line
		if(m_bActive)
		{
			pos = m_iY + m_iHeight - m_Font.GetHeight();
		}
		else
		{
			pos = m_iY + m_iHeight;
		}

		// start printing
		for(int i = 0; i < m_textBuffer.size(); ++i)
		{
			// did we get to the upper part?
			pos -= m_Font.GetHeight();
			if(pos < m_iY + 10) break;

			// print
			m_Font.Print(m_iX + 10, pos, m_textBuffer[m_textBuffer.size() - i - 1]);
		}
	}

	// disable blending
	glDisable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////////
// Print() : Prints text in the console (adds to the list)
//////////////////////////////////////////////////////////////////////////

void CConsole::Print(const std::string & strTxt)
{
	std::string strText = strTxt;
	static const int correct_width = (m_iWidth - 20);

	// cut the text in parts until it fits the console size
	while(m_Font.LineWidth(strText) > correct_width)
	{
		std::string strTemp;
		std::string::size_type index = 0;

		// until when? huh! eh! when!
		while(m_Font.LineWidth(strTemp) < correct_width)
		{
			strTemp += strText[index++];
		}

		// remove the line from the original text
		strText.erase(0, index);

		// push it
		m_textBuffer.push_back(strTemp);

		// if we are out of bounds
		if(m_textBuffer.size() > m_iMaxLines)
			m_textBuffer.erase(m_textBuffer.begin());
	}

	// push the last (or only) line
	m_textBuffer.push_back(strText);

	// if we are out of bounds
	if(m_textBuffer.size() > m_iMaxLines)
		m_textBuffer.erase(m_textBuffer.begin());
}

//////////////////////////////////////////////////////////////////////////
// ExecuteItem() : Executes a commandline
//////////////////////////////////////////////////////////////////////////

bool CConsole::ExecuteItem(const std::string & strCommand)
{
	std::ostringstream out;
	std::string::size_type index = 0;
	std::vector<std::string> arguments;
	ITEM_DB::const_iterator iter;
	std::string strCommandLine = strCommand;

	// if we dont have a "/" then its not a function nor a variable
	if(strCommandLine.find('/') == std::string::npos)
	{
		// format it
		out << "[simulacro] player: " << strCommandLine;

		// then we just print it
		Print(out.str());
		return false;
	}
	else
	{
		// parse it without the "/"
		strCommandLine.erase(strCommandLine.begin());
	}

	// add to line buffer
	m_lineBuffer.insert(m_lineBuffer.begin(), strCommandLine);
	if(m_lineBuffer.size() > 20) m_lineBuffer.pop_back();
	m_iLineIndex = -1;

	// tokenize
	while(index != std::string::npos)
	{
		// push word
		std::string::size_type next_space = strCommandLine.find(' ');
		arguments.push_back(strCommandLine.substr(index, next_space));

		// increment index
		if(next_space != std::string::npos) index = next_space + 1;
		else break;
	}

	// execute
	for(iter = m_Items.begin(); iter != m_Items.end(); ++iter)
	{
		if(iter->name == arguments[0])
		{
			switch(iter->type)
			{
			case VAR_UCHAR:
				if(arguments.size() > 2) return true;
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *((unsigned char *)(*iter).var);
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					*((unsigned char *)(*iter).var) = (unsigned char) atoi(arguments[1].c_str());
					return true;
				}
				break;

			case VAR_CHAR:
				if(arguments.size() > 2) return true;
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *((char *)(*iter).var);
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					*((char *)(*iter).var) = (char) atoi(arguments[1].c_str());
					return true;
				}
				break;

			case VAR_UINT:
				if(arguments.size() > 2) return true;
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *((unsigned int *)(*iter).var);
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					*((unsigned int *)(*iter).var) = (unsigned int) atoi(arguments[1].c_str());
					return true;
				}
				break;

			case VAR_INT:
				if(arguments.size() > 2) return true;
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *((int *)(*iter).var);
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					*((int *)(*iter).var) = (int) atoi(arguments[1].c_str());
					return true;
				}
				break;

			case VAR_FLOAT:
				if(arguments.size() > 2) return true;
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *((float *)(*iter).var);
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					*((float *)(*iter).var) = (float)atof(arguments[1].c_str());
					return true;
				}
				break;

			case VAR_CHARP:
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << (char *)(*iter).var;
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					strcpy(((char *)(*iter).var), arguments[1].c_str());
					return true;
				}
				break;

			case VAR_STRING:
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << (std::string *)(*iter).var;
					Print(out.str());
					return false;
				}
				if(arguments.size() > 1)
				{
					*((std::string *)(*iter).var) = arguments[1];
					return true;
				}
				break;

			case VAR_FUNCTION:
				Print(strCommandLine);
				(*iter).callback(arguments);
				return false;
				break;
			}
		}
	}

	out.str("");
	out << arguments[0] << " no es una funcion o variable.";
	Print(out.str());

	// just in case
	arguments.clear();

	return false;
}

//////////////////////////////////////////////////////////////////////////
// AddItem() : Adds a variable or function to the console
//////////////////////////////////////////////////////////////////////////

void CConsole::AddItem(const std::string & strName,
					   void *pointer, console_item_type_t type)
{
	console_item_t it;

	// fill item properties
	it.name = strName;
	it.type = type;

	// address
	if(type != VAR_FUNCTION)
	{
		it.var = pointer;
	}
	else
	{
		it.callback = (console_fnc) pointer;
	}

	// add item
	m_Items.push_back(it);
}

//////////////////////////////////////////////////////////////////////////
// DeleteItem() : Deletes a variable or function from the console
//////////////////////////////////////////////////////////////////////////

void CConsole::DeleteItem(const std::string & strName)
{
	ITEM_DB::iterator iter;
	
	// find item
	for (iter = m_Items.begin(); iter != m_Items.end(); ++iter)
	{
		if (iter->name == strName)
		{
			m_Items.erase(iter);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// PassKey() : Processes console keypress
//////////////////////////////////////////////////////////////////////////

void CConsole::PassKey(char c)
{
	// if we can print the char
	if(c > 31)
	{
		m_commandLine += c;
	}
	else
	{
		switch(c)
		{
		case 1:	// up arrow
			History(-1);
			break;

		case 2:	// down arrow
			History(+1);
			break;

		case 8: // backspace
			if(m_commandLine.length() > 0)
			{
				m_commandLine.erase(m_commandLine.end() - 1);
			}
			break;

		case 10:
		case 13: // enter
			if(m_commandLine.length() > 0)
			{
				if(ExecuteItem(m_commandLine)) Print(m_commandLine);
				m_commandLine = "";
			}
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// SetBackground() : Processes console keypress
//////////////////////////////////////////////////////////////////////////

void CConsole::SetBackground(const std::string & strImage)
{
	m_Background.Delete();
	m_Background.Create(strImage, FIF_UNKNOWN, false);
}

//////////////////////////////////////////////////////////////////////////
// SetBufferSize() : Ssets line buffer size
//////////////////////////////////////////////////////////////////////////

void CConsole::SetBufferSize(int size)
{
	// set
	m_iMaxLines = size;

	// remove any extra line
	if(m_textBuffer.size() > m_iMaxLines)
	{
		std::vector<std::string>::iterator it;
		it = m_textBuffer.end() - m_iMaxLines;
		m_textBuffer.erase(m_textBuffer.begin(), it);
	}
}

//////////////////////////////////////////////////////////////////////////
// Switch() : Switches between active/inactive
//////////////////////////////////////////////////////////////////////////

void CConsole::Switch()
{
	m_bActive = !m_bActive;
}

//////////////////////////////////////////////////////////////////////////
// History() : Changes actual commandline into some line printed before
//////////////////////////////////////////////////////////////////////////

void CConsole::History(int i)
{
	// increment line index
	m_iLineIndex += i;

	// error correct line index
	if(m_iLineIndex < 0) m_iLineIndex = 0;
	else if(m_iLineIndex >= m_lineBuffer.size())
		m_iLineIndex = m_lineBuffer.size() - 1;

	// set command line
	if(m_lineBuffer.size() > 0)
	{
		m_commandLine = m_lineBuffer.at(m_iLineIndex);
	}
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
