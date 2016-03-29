#ifndef __TEXTURE_H__
#define __TEXTURE_H__

//////////////////////////////////////////////////////////////////////////
// Texture constants
//////////////////////////////////////////////////////////////////////////

enum TTexResizeFlag
{
	TEX_RESIZE_OFF,
	TEX_RESIZE_AUTO,
	TEX_RESIZE_UPSAMPLE,
	TEX_RESIZE_DOWNSAMPLE
};

//////////////////////////////////////////////////////////////////////////
// Unknown classes
//////////////////////////////////////////////////////////////////////////

class CImage;

//////////////////////////////////////////////////////////////////////////
// CTexture class
//////////////////////////////////////////////////////////////////////////

class CTexture
{
public:
	// constructor & destructor
	CTexture();
	~CTexture();

public:
	// creates a texture given a filename and a loader
	bool Create(const std::string & strFilename, FREE_IMAGE_FORMAT fif,
		bool mipmap, TTexResizeFlag resize = TEX_RESIZE_AUTO,
		unsigned long dwFormat = 0);

	// creates a texture given an image
	bool CreateFromIMG(CImage *pImage, bool mipmap,
		TTexResizeFlag resize = TEX_RESIZE_AUTO,
		unsigned long dwFormat = 0);

	// frees the texture
	void Delete();

	// passes the texture to opengl
	void Use();

	// returns true if the texture is loaded
	bool IsLoaded() { return (m_nID != 0 || m_pRealTexture != NULL); }

	// proxy
	void DecCount() { m_refCount--; }
	void IncCount() { m_refCount++; }

private:
	// holds the number of times that this teture is duplicated
	int	m_refCount;

	// holds the opengl id for this texture
	unsigned int m_nID;

	// pointer to the real texture if we are just a proxy
	CTexture *m_pRealTexture;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
