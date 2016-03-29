#ifndef __IMAGE_H__
#define __IMAGE_H__

//////////////////////////////////////////////////////////////////////////
// CImage class
//////////////////////////////////////////////////////////////////////////

class CImage
{
public:
	// constructor & destructor
	CImage();
	~CImage();
	
	// creates an image and loads it
	bool Create(const std::string & strImageName, FREE_IMAGE_FORMAT fif);

	// reserves memory for an image
	bool Create(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiBpp);

	// resizes the image
	bool Resize(unsigned int uiWidth, unsigned int uiHeight);

	// saves the image
	bool Save(const std::string & strImageName, FREE_IMAGE_FORMAT fif);

	// frees the image
	void Free();

	// returns true if we have an image
	bool IsLoaded() const { return (m_pData != NULL); }

	// returns the number of bits per pixel
	unsigned int GetBPP() const { return m_uiBpp; }

	// returns the image width
	unsigned int GetWidth() const { return m_uiWidth; }

	// returns the image height
	unsigned int GetHeight() const { return m_uiHeight; }

	// returns the amount of memory needeed to allocate one scanline
	unsigned int GetStride() const { return m_uiStride; }

	// returns the total amount of memory allocated for the image
	unsigned int GetImageSize() const { return m_uiData; }

	// returns a pointer to the data
	void* GetData() { return m_pData; }

private:
	// holds the number of bits per pixel
	unsigned int m_uiBpp;

	// holds the width of the image in pixels
	unsigned int m_uiWidth;

	// holds the height of the image in pixels
	unsigned int m_uiHeight;

	// holds the memory needeed to allocate one scanline
	unsigned int m_uiStride;

	// holds the total amount of memory allocated for the image
	unsigned int m_uiData;

	// pointer to the image data
	void *m_pData;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif