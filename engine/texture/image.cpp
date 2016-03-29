#include "../precompiled/stdafx.h"
#include "../file_io/file_base.h"
#include "../file_io/package.h"
#include "../file_io/standard.h"
#include "../file_io/file.h"
#include "image.h"

//////////////////////////////////////////////////////////////////////////
// Memory-Load FreeImageIO Functions
//////////////////////////////////////////////////////////////////////////

// temporary image handle holder
fi_handle image_load_address;

// reads from memory
unsigned _stdcall image_read_mem(void *buffer, unsigned size,
								 unsigned count, fi_handle handle);

// seeks in memory
int _stdcall image_seek_mem(fi_handle handle, long offset, int origin);

// tells posision in memory
long _stdcall image_tell_mem(fi_handle handle);

//////////////////////////////////////////////////////////////////////////
// CImage default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CImage::CImage()
{
	// set variables to 0
	m_uiBpp = 0;
	m_uiWidth = 0;
	m_uiHeight = 0;
	m_uiStride = 0;
	m_uiData = 0;
	m_pData = NULL;
}

CImage::~CImage()
{
	// just in case
	Free();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates an image and loads it
//////////////////////////////////////////////////////////////////////////

bool CImage::Create(const std::string & strImageName, FREE_IMAGE_FORMAT fif)
{
	CFile image;
	std::vector<char> file_buffer;

	// check for error
	if(strImageName.length() <= 0) return false;

	// PHASE 0: try to see if we support the file by the file name
	if(fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(strImageName.c_str());
	}

	// if we couldn't recognize it return
	if(fif == FIF_UNKNOWN) return false;

	// PHASE 1: create file stream
	image.Open(strImageName, FILE_AUTO, FILE_BINARY);
	if(!image.IsOpen()) return false;

	// allocate mem for the file
	file_buffer.resize(image.Size());
	if(file_buffer.empty()) return false;

	// read file into mem
	image.Read(&file_buffer[0], image.Size());

	// clean file stream
	image.Close();

	// PHASE 2: set io procs
	FreeImageIO io;
	io.read_proc = image_read_mem;
	io.write_proc = NULL;
	io.seek_proc = image_seek_mem;
	io.tell_proc = image_tell_mem;

	// load from memory
	image_load_address = &file_buffer[0];
	FIBITMAP *dib = FreeImage_LoadFromHandle(fif, &io, (fi_handle) &file_buffer[0]);
	if(!dib) return false;

	// free mem
	file_buffer.clear();

	// PHASE 3: create image
	if(!Create(FreeImage_GetWidth(dib), FreeImage_GetHeight(dib),
		FreeImage_GetBPP(dib)))
	{
		FreeImage_Unload(dib);
		return false;
	}

	// copy the image
	memcpy(m_pData, FreeImage_GetBits(dib), m_uiData);
	
	// unload from memory
	FreeImage_Unload(dib);

	// PHASE 4: reorder BGR to RGB
	if(m_uiBpp == 24 || m_uiBpp == 32)
	{
		unsigned char r;
		unsigned char b;
		unsigned char *p;
		const int alignment = m_uiBpp / 8;

		// reorder data
		for(unsigned long i = 0; i < m_uiData; i += alignment)
		{
			// pointer to actual pixel
			p = (unsigned char *) m_pData + i;

			// save old red & blue
			r = p[0];
			b = p[2];

			// switch values
			p[0] = b;
			p[2] = r;
		}
	}

	// success!
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Create() : Reserves memory for an image
//////////////////////////////////////////////////////////////////////////

bool CImage::Create(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiBpp)
{
	// nuke any old image data.
	Free();

	// check for errors
	if(uiWidth == 0 || uiHeight == 0 || uiBpp == 0) return false;

	// fill properties
	m_uiWidth = uiWidth;
	m_uiStride = uiWidth * ((uiBpp + 7) / 8);
	m_uiHeight = uiHeight;
	m_uiBpp = uiBpp;

	// allocate memory for the image data
	m_uiData = m_uiStride * m_uiHeight;
	m_pData = new unsigned char [m_uiData];
	if(!m_pData) return false;

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Save() : Saves the image
//////////////////////////////////////////////////////////////////////////

bool CImage::Save(const std::string & strImageName, FREE_IMAGE_FORMAT fif)
{
	FIBITMAP *dib;

	// check
	if(!IsLoaded()) return false;

	// allocate dib
	dib = FreeImage_Allocate(m_uiWidth, m_uiHeight, m_uiBpp);
	if(!dib) return false;

	// copy image data
	memcpy(FreeImage_GetBits(dib), m_pData, m_uiData);

	// reorder RGB to BGR
	if(m_uiBpp == 24 || m_uiBpp == 32)
	{
		unsigned char r;
		unsigned char b;
		unsigned char *p;
		const int alignment = m_uiBpp / 8;

		// reorder data
		for(unsigned long i = 0; i < m_uiData; i += alignment)
		{
			// pointer to actual pixel
			p = (unsigned char *) FreeImage_GetBits(dib) + i;

			// save old red & blue
			r = p[0];
			b = p[2];

			// switch values
			p[0] = b;
			p[2] = r;
		}
	}

	// save dib
	FreeImage_Save(fif, dib, strImageName.c_str());

	// delete dib
	FreeImage_Unload(dib);

	// no save yet
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Free() : Frees the image, unloads all the data
//////////////////////////////////////////////////////////////////////////

void CImage::Free()
{
	// reset variables
	m_uiBpp = 0;
	m_uiWidth = 0;
	m_uiHeight = 0;
	m_uiStride = 0;
	m_uiData = 0;

	// free data
	delete [] m_pData;
	m_pData = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Resize() : Resizes the image
//////////////////////////////////////////////////////////////////////////

bool CImage::Resize(unsigned int uiWidth, unsigned int uiHeight)
{
	unsigned char *newbuffer;

	// save some time, no resizing needed
	if(uiWidth == m_uiWidth && uiHeight == m_uiHeight) return true;

	// allocate new buffer
	newbuffer = new unsigned char [uiWidth * uiHeight * ((m_uiBpp + 7) / 8)];
	if(!newbuffer) return false;

	// scale
	switch(m_uiBpp)
	{
	case 8:
		gluScaleImage(GL_LUMINANCE, m_uiWidth, m_uiHeight, GL_UNSIGNED_BYTE, m_pData,
					  uiWidth, uiHeight, GL_UNSIGNED_BYTE, newbuffer);
		break;

	case 24:
		gluScaleImage(GL_RGB, m_uiWidth, m_uiHeight, GL_UNSIGNED_BYTE, m_pData,
					  uiWidth, uiHeight, GL_UNSIGNED_BYTE, newbuffer);
		break;

	case 32:
		gluScaleImage(GL_RGBA, m_uiWidth, m_uiHeight, GL_UNSIGNED_BYTE, m_pData,
					  uiWidth, uiHeight, GL_UNSIGNED_BYTE, newbuffer);
		break;
	}

	// free old buffer
	delete [] m_pData;

	// set new buffer
	m_pData = newbuffer;

	// set new sizes
	m_uiWidth = uiWidth;
	m_uiStride = uiWidth * ((m_uiBpp + 7) / 8);
	m_uiHeight = uiHeight;
	m_uiData = m_uiStride * m_uiHeight;

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Memory-Load FreeImageIO Functions
//////////////////////////////////////////////////////////////////////////

inline unsigned _stdcall image_read_mem(void *buffer, unsigned size,
										unsigned count, fi_handle handle)
{
	// temp pointer
	unsigned char *tmp = (unsigned char *)buffer;

	// loop "count" times
	for (unsigned c = 0; c < count; c++)
	{
		memcpy(tmp, image_load_address, size);
		image_load_address = (unsigned char *) image_load_address + size;
		tmp += size;
	}

	// return read records
	return count;
}

inline int _stdcall image_seek_mem(fi_handle handle, long offset, int origin)
{
	if (origin == SEEK_SET)
	{
		image_load_address = (unsigned char *) handle + offset;
	}
	else
	{
		image_load_address = (unsigned char *) image_load_address + offset;
	}

	return 0;
}

inline long _stdcall image_tell_mem(fi_handle handle)
{
	return ((int) image_load_address - (int) handle);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
