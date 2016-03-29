#include "../precompiled/stdafx.h"
#include "../engine/base.h"

//////////////////////////////////////////////////////////////////////////
// CTextureFont default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CTextureFont::CTextureFont()
{
}

CTextureFont::~CTextureFont()
{
	// free font mem
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates a textured font
//////////////////////////////////////////////////////////////////////////

bool CTextureFont::Create(const std::string & strFontname, int iHeight, int iWeight)
{
	// width's file
	CFile widths;

	// temporary path holder
	std::ostringstream path;

	// create the texture
	path << "fonts/" << strFontname << ".pak/texture.tex";
	if(!m_texFont.Create(path.str(), FIF_TARGA, true)) return false;

	// set widths path & create the stream
	path.str("");
	path << "fonts/" << strFontname << ".pak/config.dat";
	widths.Open(path.str(), FILE_PACKAGE, FILE_BINARY);
	
	// error?
	if(!widths.IsOpen())
	{
		m_texFont.Delete();
		return false;
	}

	// read widths
	unsigned char ar_widths[256] = {0};
	widths.Read(ar_widths, sizeof(ar_widths));

	// set widths
	for(int i = 0; i < 256; ++i)
		m_iWidths[i] = (int) ar_widths[i];

	// close stream
	widths.Close();

	// save height
	m_iHeight = 20;

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Free the font
//////////////////////////////////////////////////////////////////////////

void CTextureFont::Destroy()
{
	// destroy texture
	if(m_texFont.IsLoaded()) m_texFont.Delete();
}

////////////////////////////////////////////////////////////////////////
// Print() : Prints a string on the screen
////////////////////////////////////////////////////////////////////////

void CTextureFont::Print(float x, float y, const std::string & text)
{
	static int relative_x;

	// check for errors
	if(!m_texFont.IsLoaded()) return;

	// set position
	glTranslatef(x, y, 0);

	// draw it
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_texFont.Use();

	// reset relative position
	relative_x = 0;

	// print every character
	for(unsigned int i = 0; i < text.length(); ++i)
	{
		// print this char
		PrintChar(relative_x, 0, text[i]);

		// add this char width
		relative_x += (m_iHeight * m_iWidths[text[i]] / 32) + (m_iHeight / 8);
	}

	// set gl state back
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

////////////////////////////////////////////////////////////////////////
// PrintChar() : Prints ONE character on the screen
////////////////////////////////////////////////////////////////////////

inline void CTextureFont::PrintChar(int x, int y, unsigned char c) const
{
	// font cell width for opengl (1 / num_of_chars_in_a_row)
	static const float fCellWidth = 0.0625f;

	// char x
	const float fTextureX = ((float) c / 16.0f) - (c / 16);

	// char y
	const float fTextureY = 1.0f - (c / 16) * fCellWidth;

	// char width
	const float fCharWidth = m_iHeight * m_iWidths[c] / 32.0f;

	// texture char padding
	const float fPadding = ((32 - m_iWidths[c]) / 2) / 512.0f;

	// draw character
	glBegin(GL_TRIANGLE_STRIP);
		// upper left
		glTexCoord2f(fTextureX + fCellWidth - fPadding, fTextureY - fCellWidth);
		glVertex2f((float) (x + fCharWidth), (float) (y + m_iHeight));

		// lower left
		glTexCoord2f(fTextureX + fPadding, fTextureY - fCellWidth);
		glVertex2f((float) (x), (float) (y + m_iHeight));

		// upper right
		glTexCoord2f(fTextureX + fCellWidth - fPadding, fTextureY);
		glVertex2f((float) (x + fCharWidth), (float) (y));
		
		// lower right
		glTexCoord2f(fTextureX + fPadding, fTextureY);
		glVertex2f((float) (x), (float) (y));
	glEnd();
}

////////////////////////////////////////////////////////////////////////
// End
////////////////////////////////////////////////////////////////////////
