#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// Local functions
//////////////////////////////////////////////////////////////////////////

unsigned int NearestPowerOfTwo(unsigned int n);
unsigned int MajorPowerOfTwo(unsigned int n);

//////////////////////////////////////////////////////////////////////////
// CTexture default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CTexture::CTexture()
{
	// set default values
	m_nID = 0;
	m_refCount = 0;
	m_pRealTexture = NULL;
}

CTexture::~CTexture()
{
	// just in case
	Delete();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Loads texture from filename
//////////////////////////////////////////////////////////////////////////

bool CTexture::Create(const std::string & strFilename, FREE_IMAGE_FORMAT fif,
					  bool mipmap, TTexResizeFlag resize, unsigned long dwFormat)
{
	// First, check if we can find this texture in DB already...
	CTexture *tex = g_TextureMgr->Find(strFilename);

	// have we found the texture?
	if(tex)
	{
		// we found the texture in the database
		tex->IncCount();
		m_pRealTexture = tex;

		// no errors
		return true;
	}
	else
	{
		// we need to create a new image & texture
		CImage img;

		if(img.Create(strFilename, fif) == false)
			return false;

		// Mark as used
		m_refCount = 1;

		// create texture from image
		CreateFromIMG(&img, mipmap, resize, dwFormat);

		// add it to the DB
		g_TextureMgr->Add(strFilename, this);

		// no errors
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
// Delete() : Destructs the texture
//////////////////////////////////////////////////////////////////////////

void CTexture::Delete()
{
	// check loaded or not
	if(m_nID == 0 && m_pRealTexture == NULL) return;

	// Are we real or proxy
	if(m_pRealTexture == NULL)
	{
		// One less needs this one
		DecCount();

		// Last one standing?
		if (m_refCount == 0)
		{
			// delete opengl texture
			if(m_nID)
			{
				m_nID--;
				glDeleteTextures(1, &m_nID);
				m_nID = 0;
			}

			// remove the texure from database
			g_TextureMgr->Remove(this);
		}
	}
	else
	{
		// recurse to real texture
		m_pRealTexture->Delete();
		m_pRealTexture = NULL;
	}

	// set default values
	m_nID = 0;
	m_refCount = 0;
	m_pRealTexture = NULL;
}

//////////////////////////////////////////////////////////////////////////
// CreateFromIMG() : Creates a texture from a CImage
//////////////////////////////////////////////////////////////////////////

bool CTexture::CreateFromIMG(CImage *pImage, bool mipmap,
							 TTexResizeFlag resize,	unsigned long dwFormat)
{
	// generate new texture
	glGenTextures(1, &m_nID);
	m_nID++;

	// bind with the actual
	glBindTexture(GL_TEXTURE_2D, m_nID - 1);

	// set general texture options
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// get sizes
	const unsigned int oldWidth = pImage->GetWidth();
	const unsigned int oldHeight = pImage->GetHeight();
	unsigned int newWidth = NearestPowerOfTwo(pImage->GetWidth());
	unsigned int newHeight = NearestPowerOfTwo(pImage->GetHeight());

	// check sizes
	if(resize == TEX_RESIZE_AUTO)
	{
		if(oldWidth != newWidth || oldHeight != newHeight)
		{
			// more quality (major) high quality textures
			if(ext::get_option_int("high_quality_textures"))
			{
				// do we need a new width?
				newWidth = oldWidth != newWidth ?
					MajorPowerOfTwo(oldWidth) : oldWidth;

				// do we need a new height?
				newHeight = oldHeight != newHeight ?
					MajorPowerOfTwo(oldHeight) : oldHeight;
			}
		}
	}
	else if(resize == TEX_RESIZE_UPSAMPLE)
	{
		newWidth = oldWidth != newWidth ? MajorPowerOfTwo(oldWidth) : oldWidth;
		newHeight = oldHeight != newHeight ? MajorPowerOfTwo(oldHeight) : oldHeight;
	}

	// do the resizing
	if(resize != TEX_RESIZE_OFF)
	{
		if(!pImage->Resize(newWidth, newHeight))
		{
			// delete the recently allocated space
			glDeleteTextures(1, &(--m_nID));
			m_nID = 0;

			// oops
			return false;
		}
	}

	// clamp flag
	//if(dwFlags & TEX_CLAMP)
	//{
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//}
	//else
	//{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//}

	// mipmap flag
	if(mipmap)
	{
		// set mipmap options
		switch(ext::get_option_int("texture_filter"))
		{
		case 1:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
			break;

		case 2:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
			break;

		case 3:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, ext::get_option_float("texture_anisotropy"));
			break;
		}

		// create mipmap filtered texture
		switch(pImage->GetBPP())
		{
		case 8:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 1, pImage->GetWidth(),
				pImage->GetHeight(), GL_LUMINANCE, GL_UNSIGNED_BYTE, pImage->GetData());
			break;

		case 24:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->GetWidth(),
				pImage->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pImage->GetData());
			break;

		case 32:
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->GetWidth(),
				pImage->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pImage->GetData());
			break;
		}
	}
	else
	{
		// set linear parameters
		switch(ext::get_option_int("texture_filter"))
		{
		case 1:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
			break;

		case 2:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
			break;

		case 3:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, ext::get_option_float("texture_anisotropy"));
			break;
		}

		// create linear filtered texture
		switch(pImage->GetBPP())
		{
		case 8:
			glTexImage2D(GL_TEXTURE_2D, 0, 1, pImage->GetWidth(),
				pImage->GetHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pImage->GetData());
			break;

		case 24:
			glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->GetWidth(),
				pImage->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->GetData());
			break;

		case 32:
			glTexImage2D(GL_TEXTURE_2D, 0, 4, pImage->GetWidth(),
				pImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage->GetData());
			break;
		}
	}

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Use() : Tells OpenGL to use this texture
//////////////////////////////////////////////////////////////////////////

void CTexture::Use()
{ 
	// if we are a proxy...
	if(m_pRealTexture)
	{
		// call the real one
		m_pRealTexture->Use();
	}
	else
	{
		// send this texture to opengl
		if(m_nID) glBindTexture(GL_TEXTURE_2D, m_nID - 1);
	}
}

//////////////////////////////////////////////////////////////////////////
// NearestPowerOfTwo() : Returns the nearest-lower power of 2 of n
//////////////////////////////////////////////////////////////////////////

inline unsigned int NearestPowerOfTwo(unsigned int n)
{
	return pow(2, (unsigned int)(log(n) / log(2)));
}

//////////////////////////////////////////////////////////////////////////
// MajorPowerOfTwo() : Returns the nearest-upper power of 2 of n
//////////////////////////////////////////////////////////////////////////

inline unsigned int MajorPowerOfTwo(unsigned int n)
{
	return pow(2, (unsigned int)(log(n) / log(2)) + 1);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
