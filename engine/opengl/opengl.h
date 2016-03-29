#ifndef __OPENGL_H__
#define __OPENGL_H__

//////////////////////////////////////////////////////////////////////////
// COpenGL class
//////////////////////////////////////////////////////////////////////////

class COpenGL
{
public:
	// contructor & destructor
	COpenGL();
	~COpenGL();

	// initializes gl parameters
	bool InitGL();

	// copies from the buffer to the screen
	void SwapBuffers() const { SDL_GL_SwapBuffers(); }

	// creates the window
	bool InitWindow(const std::string & strTitle, int iWidth, int iHeight,
		int iBits, bool bFullscreen);

	// destroys the window
	void KillWindow() const;

	// resizes the window (scene)
	void ReSizeScene(int iWidth, int iHeight) const;

	// returns true if your card supports the requiered extension
	bool GetExtension(const char *szExtension) const;

	// starts 2D mode
	void OverlayBegin(unsigned int uiWidth,
		unsigned int uiHeight, bool bDirection) const;

	// ends 2D mode
	void OverlayEnd() const;

	// starts spherical billboard
	void SphericalBillboardBegin() const;

	// starts cilindrical billboard
	void CilindricalBillboardBegin() const;

	// ends any type of billboard
	void BillboardEnd() const;

	// fades the screen to black in "dwFrames" frames
	void FadeToBlack(unsigned long dwFrames) const;

	// saves the screen to a file
	bool SaveScreenshot() const;

private:
	// pointer to supported gl extensions
	const char *m_szExtensions;

	// size of the fade buffer
	int	m_iFadeBufferLength;
};

//////////////////////////////////////////////////////////////////////////
// Multitexture GL definitions
//////////////////////////////////////////////////////////////////////////

#define GL_TEXTURE0_ARB		0x84C0
#define GL_TEXTURE1_ARB		0x84C1
#define GL_COMBINE_ARB		0x8570
#define GL_RGB_SCALE_ARB	0x8573

//////////////////////////////////////////////////////////////////////////
// Multitexture GL prototypes
//////////////////////////////////////////////////////////////////////////

typedef void (APIENTRY *PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY *PFNGLACTIVETEXTUREARBPROC) (GLenum target);

//////////////////////////////////////////////////////////////////////////
// Multitexture GL exports
//////////////////////////////////////////////////////////////////////////

extern PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB;

//////////////////////////////////////////////////////////////////////////
// Fog extension GL definitions
//////////////////////////////////////////////////////////////////////////

#define GL_FOG_COORDINATE_SOURCE_EXT	0x8450
#define GL_FOG_COORDINATE_EXT			0x8451

//////////////////////////////////////////////////////////////////////////
// Fog extension GL prototypes
//////////////////////////////////////////////////////////////////////////

typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);

//////////////////////////////////////////////////////////////////////////
// Fog extension GL exports
//////////////////////////////////////////////////////////////////////////

extern PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;

//////////////////////////////////////////////////////////////////////////
// end
//////////////////////////////////////////////////////////////////////////

#endif
