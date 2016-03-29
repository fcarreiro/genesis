#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// Extension procs
//////////////////////////////////////////////////////////////////////////

PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB	 = NULL;
PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB = NULL;
PFNGLFOGCOORDFEXTPROC		glFogCoordfEXT		 = NULL;

//////////////////////////////////////////////////////////////////////////
// COpenGL default constructor & destructor
//////////////////////////////////////////////////////////////////////////

COpenGL::COpenGL() : m_szExtensions(NULL), m_iFadeBufferLength(0)
{
	// initialize variables
}

COpenGL::~COpenGL()
{
	// destroy opengl window
	KillWindow();
}

//////////////////////////////////////////////////////////////////////////
// InitGL() : Initializes OpenGL specific stuff
//////////////////////////////////////////////////////////////////////////

bool COpenGL::InitGL()
{
	// save extensions
	m_szExtensions = (const char *) glGetString(GL_EXTENSIONS);

	// set fade buffer length
	m_iFadeBufferLength = ext::get_option_int("high_quality_textures") ? 512 : 256;

	// try to get gl extension pointers
	glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) SDL_GL_GetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) SDL_GL_GetProcAddress("glFogCoordfEXT");

	// OPENGL-->shading
	glShadeModel(GL_SMOOTH);

	// OPENGL-->lighting
	glEnable(GL_LIGHTING);

	// OPENGL-->zbuffering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// OPENGL-->blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// OPENGL-->culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// OPENGL-->arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// OPENGL-->fog
	glFogi(GL_FOG_MODE, GL_LINEAR);

	// OPENGL-->hints
	glHint(GL_FOG_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// GetExtension() : Returns true if your card has the extension
//////////////////////////////////////////////////////////////////////////

bool COpenGL::GetExtension(const char *szExtension) const
{
	// return true if we have the extension in the string
	if(m_szExtensions)
	{
		return (strstr(m_szExtensions, szExtension) != NULL);
	}

	// no extensions pointer
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ReSizeScene() : If the window is resized we change the scene size too
//////////////////////////////////////////////////////////////////////////

void COpenGL::ReSizeScene(int iWidth, int iHeight) const
{
	// prevent div by 0 error
	if(iHeight == 0) iHeight = 1;

	// reset the view port
	glViewport(0, 0, iWidth, iHeight);

	// reset the projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set the perspective
	gluPerspective(45.0f, (GLfloat)iWidth/(GLfloat)iHeight, 1.0f, 300.0f);

	// reset modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//////////////////////////////////////////////////////////////////////////
// InitWindow() : Creates the main opengl window and initializes it
//////////////////////////////////////////////////////////////////////////

bool COpenGL::InitWindow(const std::string & strTitle, int iWidth, int iHeight,
						 int iBits, bool bFullscreen)
{
	// initialize
	SDL_InitSubSystem(SDL_INIT_VIDEO);

#ifndef WIN32
	// get x11's desktop bpp and set it
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	iBits = info->vfmt->BitsPerPixel;
#endif

	// set flags
	Uint32 iFlags = SDL_OPENGL | SDL_NOFRAME | (bFullscreen ? SDL_FULLSCREEN : 0);

	// sdl opengl attributes
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// set video mode
	if(!SDL_SetVideoMode(iWidth, iHeight, iBits, iFlags)) return false;

	// initialize opengl
	if(!InitGL()) return false;

	// set scene size
	ReSizeScene(iWidth, iHeight);

	// if fullscreen do not show the cursor
	if(bFullscreen) SDL_ShowCursor(0);

	// set window title
	SDL_WM_SetCaption(strTitle.c_str(), NULL);

	// success
	return true;
}

//////////////////////////////////////////////////////////////////////////
// KillWindow() : Destroys the window
//////////////////////////////////////////////////////////////////////////

void COpenGL::KillWindow() const
{
	// show cursor if hidden
	SDL_ShowCursor(1);

	// shut down video
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

//////////////////////////////////////////////////////////////////////////
// OverlayBegin() : Starts 2D mode
//////////////////////////////////////////////////////////////////////////

void COpenGL::OverlayBegin(unsigned int uiWidth, unsigned int uiHeight, 
						   bool bDirection) const
{
	// save our 3d matrices
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// upside down?
	if(!bDirection)
	{
		glOrtho(0, uiWidth, 0, uiHeight, 0, 1);
	}
	else
	{
		glOrtho(0, uiWidth, uiHeight, 0, 0, 1);
	}

	// we dont need these
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
}

//////////////////////////////////////////////////////////////////////////
// OverlayEnd() : Ends 2D mode
//////////////////////////////////////////////////////////////////////////

void COpenGL::OverlayEnd() const
{
	// we need them back
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	
	// back to our 3d matrices
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////
// SaveScreenshot() : Saves the actual image to a file
//////////////////////////////////////////////////////////////////////////

bool COpenGL::SaveScreenshot() const
{
	bool ret;
	CImage pixels;
	static int n = 0;
	static const int width = ext::get_option_int("screen_width");
	static const int height = ext::get_option_int("screen_height");

	// jpg or png?
	static const FREE_IMAGE_FORMAT fif =
		ext::get_option_int("screenshot_jpg") ? FIF_JPEG : FIF_PNG;

	// create image
	if(!pixels.Create(width, height, 24)) return false;

	// set filename
	std::ostringstream filename;
	filename << "screenshot" << n++ << (fif == FIF_JPEG ? ".jpg" : ".png");

	// read framebuffer
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.GetData());

	// save screenshot
	ret = pixels.Save(filename.str(), fif);

	// free image
	pixels.Free();

	// return what savejpg returned
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// FadeToBlack() : Fades the screen to black in "frames" frames
//////////////////////////////////////////////////////////////////////////

void COpenGL::FadeToBlack(unsigned long frames) const
{
	CImage img;
	CTexture fade_buffer;
	float fAlpha = 1.0f;

	// get values
	static const int width = ext::get_option_int("screen_width");
	static const int height = ext::get_option_int("screen_height");
	static const int bpp = ext::get_option_int("screen_depth");

	// allocate 1 screen buffer
	if(!img.Create(width, height, 24)) return;

	// read actual screen & resize
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, img.GetData());
	img.Resize(m_iFadeBufferLength, m_iFadeBufferLength);

	// copy image to texture
	fade_buffer.CreateFromIMG(&img, false);
	img.Free();

	// activate blending
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// activate texture
	fade_buffer.Use();

	// fade loop
	for(int i = 0; i < frames; ++i)
	{
		fAlpha -= (float)(1.0f / frames);

		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);

		// 2d start
		OverlayBegin(width, height, true);
		
		// black square alpha channel
		glColor4f(1.0f, 1.0f, 1.0f, fAlpha);

		// draw black square
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, 0);
			glVertex2i(0, height);

			glTexCoord2i(0, 1);
			glVertex2i(0, 0);

			glTexCoord2i(1, 0);
			glVertex2i(width, height);

			glTexCoord2i(1, 1);
			glVertex2i(width, 0);
		glEnd();

		// 2d end
		OverlayEnd();

		// swap :..)
		SwapBuffers();
	}
	
	// disable blending
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////////
// SphericalBillboardBegin() : Starts x-y-z billboard
//////////////////////////////////////////////////////////////////////////

void COpenGL::SphericalBillboardBegin() const
{
	int i,j;
	static float fModelview[16];

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, fModelview);

	// undo all rotations & scaling
	for(i = 0; i < 3; ++i) 
	{
	    for(j = 0; j < 3; ++j)
		{
			if (i == j)
				fModelview[i * 4 + j] = 1.0f;
			else
				fModelview[i * 4 + j] = 0.0f;
	    }
	}

	// set the modelview with no rotations
	glLoadMatrixf(fModelview);
}

//////////////////////////////////////////////////////////////////////////
// CilindricalBillboardBegin() : Starts x-z billboard
//////////////////////////////////////////////////////////////////////////

void COpenGL::CilindricalBillboardBegin() const
{
	int i, j;
	static float fModelview[16];

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, fModelview);

	// undo all rotations & scaling
	for(i = 0; i < 3; i += 2) 
	{
	    for(j = 0; j < 3; ++j)
		{
			if (i == j)
				fModelview[i * 4 + j] = 1.0f;
			else
				fModelview[i * 4 + j] = 0.0f;
	    }
	}

	// set the modelview with no rotations
	glLoadMatrixf(fModelview);
}

//////////////////////////////////////////////////////////////////////////
// BillboardEnd() : Ends all kinds of billboards
//////////////////////////////////////////////////////////////////////////

void COpenGL::BillboardEnd() const
{
	// restore the previously stored modelview matrix
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
