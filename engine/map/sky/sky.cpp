#include "../../precompiled/stdafx.h"
#include "../../engine/base.h"
#include "../../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// Sky standard definitions
//////////////////////////////////////////////////////////////////////////

#define MAX_STAR_SIZE		2
#define MIN_STAR_SIZE		1
#define MIN_STAR_LUM		0.5f
#define MIN_HORIZON_ANGLE	20

#define SUN_SIZE			30
#define MOON_SIZE			20

#define MIN_TRANSITION		0.5f;

//////////////////////////////////////////////////////////////////////////
// Day/Night Gamma ramp
//////////////////////////////////////////////////////////////////////////

// initialize sky gamma
const float fGamma[8] = { 0.0f,		// 00:00
						  0.0f,		// 03:00
						  0.5f,		// 06:00
						  1.0f,		// 09:00
						  1.0f,		// 12:00
						  1.0f,		// 15:00
						  0.5f,		// 18:00
						  0.0f };	// 21:00

//////////////////////////////////////////////////////////////////////////
// CSky default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CSky::CSky()
{
	// reset variables
	m_iResolution = 0;
	m_iStars = 0;
	m_fHeight = 0.0f;
	m_fBaseRadius = 0.0f;
	m_fActualGamma = 0.0f;
	m_fDayTime = 0.0f;

	// pointers
	m_pVertices = NULL;
	m_pStars = NULL;
	m_texMoon = NULL;
	m_texSun = NULL;
	m_texStar = NULL;
	m_texCloudLayer = NULL;
}

CSky::~CSky()
{
	// free
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates the skydome
//////////////////////////////////////////////////////////////////////////

bool CSky::Create(float fHeight, float fBaseRadius, int iResolution,
				  int iMinStars, int iMaxStars,
				  float fCloudCover, CVector3 & vCenter,
				  CVector3 & vNightBaseColor,	CVector3 & vDayBaseColor,
				  CVector3 & vNightBaseAmbient, CVector3 & vDayBaseAmbient)
{
	// PHASE 0: free
	Destroy();

	// check parameters
	if(fHeight <= 0.0f || fBaseRadius <= 0.0f || iResolution <= 0)
		return false;

	// set local variables
	m_iResolution = iResolution;
	m_fHeight = fHeight;
	m_fBaseRadius = fBaseRadius;
	m_vCenter = vCenter;

	// set color vectors
	m_vNightBaseColor = vNightBaseColor;
	m_vDayBaseColor = vDayBaseColor;
	m_vNightBaseAmbient = vNightBaseAmbient;
	m_vDayBaseAmbient = vDayBaseAmbient;

	// create dome
	if(!CreateMesh(iResolution, fBaseRadius, fHeight))
	{
		Destroy();
		return false;
	}

	// create stars
	if(iMaxStars != 0 && iMaxStars > iMinStars)
	{
		if(!CreateStars(iMinStars, iMaxStars, fBaseRadius, fHeight))
		{
			Destroy();
			return false;
		}
	}

	// create sky texture
	if(fCloudCover > 0.0f)
	{
		if(!CreateClouds(fCloudCover))
		{
			Destroy();
			return false;
		}
	}

	// load textures
	if(!LoadTextures())
	{
		Destroy();
		return false;
	}

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CreateClouds() : Creates perlin noise clouds
//////////////////////////////////////////////////////////////////////////

bool CSky::CreateClouds(float fCloudCover)
{
	CImage image;
	int x, y;
	int x2, y2;
	int length;
	int distance;
	float sharpness;
	unsigned char *p;
	unsigned char original;
	const unsigned char cover = (1.0f - fCloudCover) * 255;

	// set texture length
	length = ext::get_option_int("high_quality_textures") ? 1024 : 512;

	// save a looottt of time on slow pc's
	if(ext::get_option_int("pre_generated_perlin"))
	{
		// load pre-generated perlin noise
		if(!image.Create("gfx/environment.pak/preperlin.png", FIF_PNG))
			return false;

		// resize
		image.Resize(length, length);
	}
	else
	{
		CPerlinNoise perlin;

		// create image
		if(!image.Create(length, length, 8))
			return false;

		// setup perlin generator
		perlin.SetParameters(0.01f, 0.65f, 8);

		// fill perlin noise image
		if(!perlin.FillTable((unsigned char *) image.GetData(), length, length))
			return false;
	}

	// setup retouch
	sharpness = (float)(cover - 45) / 1200.0f + 0.9f;
	if(sharpness > 0.99f) sharpness = 0.99f;
	p = (unsigned char *) image.GetData();

	// use exponential curve to regulate cloudcover
	for(x = 0; x < length; ++x)
	{
		for(y = 0; y < length; ++y)
		{
			// assign the byte
			original = p[y * length + x];
			original = ExponentialCurve(original, cover, sharpness);
			p[y * length + x] = original;
		}
	}

	// new texture object
	m_texCloudLayer = new CTexture();
	if(!m_texCloudLayer) return false;

	// load cloud layer texture
	if(!m_texCloudLayer->CreateFromIMG(&image, false))
	{
		// delete object
		delete m_texCloudLayer;
		m_texCloudLayer = NULL;

		// error
		return false;
	}

	// well done
	return true;
}

//////////////////////////////////////////////////////////////////////////
// LoadTextures() : Loads the moon, stars & sun
//////////////////////////////////////////////////////////////////////////

bool CSky::LoadTextures()
{
	// load star texture
	if(m_iStars != 0)
	{
		m_texStar = new CTexture();
		if(!m_texStar) return false;
		m_texStar->Create("gfx/environment.pak/star.png", FIF_PNG, true);
	}

	// load moon texture
	m_texMoon = new CTexture();
	if(!m_texMoon) return false;
	m_texMoon->Create("gfx/environment.pak/moon.png", FIF_PNG, true);

	// load sun texture
	m_texSun = new CTexture();
	if(!m_texSun) return false;
	m_texSun->Create("gfx/environment.pak/sun.png", FIF_PNG, true);

	// well done
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CreateStars() : Creates the stars
//////////////////////////////////////////////////////////////////////////

bool CSky::CreateStars(int iMinStars, int iMaxStars, float fBaseRadius, float fHeight)
{
	int index = 0;
	double cos_y = 0.0;
	double cos_z = 0.0;
	double sin_y = 0.0;
	double sin_z = 0.0;
	double angle_y = 0.0;
	double angle_z = 0.0;
	double var_y = 90.0 / m_iResolution;
	double var_z = 360.0 / m_iResolution;

	// set number of stars
	m_iStars = (rand() % (iMaxStars - iMinStars)) + iMinStars;

	// PHASE 3: create stars array
	m_pStars = new TStar [m_iStars];
	if(!m_pStars) return false;

	// randomly create the stars
	for(index = 0; index < m_iStars; ++index)
	{
		// minutes & seconds
		angle_y = (rand() % (90 - MIN_HORIZON_ANGLE)) + MIN_HORIZON_ANGLE;
		angle_y += (float) rand() / (float) (RAND_MAX + 1);

		// minutes & seconds
		angle_z = rand() % 360;
		angle_z += (float) rand() / (float) (RAND_MAX + 1);

		// get sines & cosines
		cos_y = cos(angle_y * PI / 180.0);
		cos_z = cos(angle_z * PI / 180.0);
		sin_y = sin(angle_y * PI / 180.0);
		sin_z = sin(angle_z * PI / 180.0);

		// set star position
		m_pStars[index].vPos.x = (float) (cos_z * cos_y * fBaseRadius);
		m_pStars[index].vPos.y = (float) (sin_y * fHeight);
		m_pStars[index].vPos.z = (float) (sin_z * cos_y * fBaseRadius);

		// set star size
		m_pStars[index].fSize = (rand() % (MAX_STAR_SIZE - MIN_STAR_SIZE)) +
								MIN_STAR_SIZE;
		m_pStars[index].fSize += (float) rand() / (float) RAND_MAX;

		// set star lumination
		float star_lum = MIN_STAR_LUM +
			((float) rand() / (float)(RAND_MAX + 1) * MIN_STAR_LUM);
		m_pStars[index].vColor = CVector3(star_lum, star_lum, star_lum);
	}

	// well done
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CreateMesh() : Creates the dome mesh
//////////////////////////////////////////////////////////////////////////

bool CSky::CreateMesh(int iResolution, float fBaseRadius, float fHeight)
{
	int index = 0;
	double cos_y = 0.0;
	double cos_z = 0.0;
	double sin_y = 0.0;
	double sin_z = 0.0;
	double angle_y = 0.0;
	double angle_z = 0.0;
	double var_y = 90.0 / m_iResolution;
	double var_z = 360.0 / m_iResolution;

	// allocate vertices
	m_pVertices = new CVector3 [1 + iResolution * iResolution];
	if(!m_pVertices) return false;

	// first point (top)
	m_pVertices[index].x = (float) 0.0f;
	m_pVertices[index].y = (float) fHeight;
	m_pVertices[index++].z = (float) 0.0f;

	// rest of the dome over Y
	for(angle_y = 90.0 - var_y; angle_y > 0; angle_y -= var_y)
	{
		// over Z (x)
		for(angle_z = 0.0; angle_z <= 360; angle_z += var_z)
		{
			// get sines and cosines
			cos_y = cos(angle_y * PI / 180.0);
			cos_z = cos(angle_z * PI / 180.0);
			sin_y = sin(angle_y * PI / 180.0);
			sin_z = sin(angle_z * PI / 180.0);

			// set point coords
			m_pVertices[index].x = (float) (cos_z * cos_y * fBaseRadius);
			m_pVertices[index].y = (float) (sin_y * fHeight);
			m_pVertices[index++].z = (float) (sin_z * cos_y * fBaseRadius);
		}
	}

	// well done
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Frees the sky
//////////////////////////////////////////////////////////////////////////

void CSky::Destroy()
{
	// delete dome vertices
	delete [] m_pVertices;
	m_pVertices = NULL;

	// delete dome starts
	delete [] m_pStars;
	m_pStars = NULL;

	// destroy textures (moon)
	delete m_texMoon;
	m_texMoon = NULL;

	// destroy textures (sun)
	delete m_texSun;
	m_texSun = NULL;

	// destroy textures (star)
	delete m_texStar;
	m_texStar = NULL;

	// destroy cloud layer texture
	delete m_texCloudLayer;
	m_texCloudLayer = NULL;

	// reset variables
	m_iResolution = 0;
	m_iStars = 0;
	m_fHeight = 0.0f;
	m_fBaseRadius = 0.0f;
	m_fActualGamma = 0.0f;
	m_fDayTime = 0.0f;

	// reset vectors;
	m_vCenter = CVector3(0, 0, 0);
	m_vNightBaseColor = CVector3(0, 0, 0);
	m_vDayBaseColor = CVector3(0, 0, 0);
	m_vNightBaseAmbient = CVector3(0, 0, 0);
	m_vDayBaseAmbient = CVector3(0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// Render() : Draws the sky
//////////////////////////////////////////////////////////////////////////

void CSky::Render(const CVector3 & vPosition, float fShoreLine,
				  const CVector3 & vFogColor)
{
	// color & position vectors
	CVector3 vSkyColor;
	CVector3 vSun, vMoon;

	// transparency gamma variables
	int iGammaIndex = m_fDayTime * 7;
	float fGammaGap = 1.0f - (m_fDayTime * 7.0f - iGammaIndex);

	// set gamma
	m_fActualGamma = (fGamma[iGammaIndex] * fGammaGap) +
					 (fGamma[iGammaIndex + 1] * (1.0f - fGammaGap));

	// increase daytime
	m_fDayTime += 0.0001f;
	if(m_fDayTime >= 1.0f) m_fDayTime = 0.0f;

	// set sky color
	vSkyColor = ((m_vDayBaseColor - m_vNightBaseColor) *
				m_fActualGamma + m_vNightBaseColor);

	// set sun position
	float sun_y = (m_fDayTime - 0.25f) * 2 * PI;
	vSun.x = cos(sun_y) * m_fBaseRadius;
	vSun.y = sin(sun_y) * m_fBaseRadius;
	vSun.z = 0.0f;

	// set moon position
	vMoon.x = 0.0f;
	vMoon.y = sin(40.0f * PI / 180.0f) * m_fHeight;
	vMoon.z = cos(50.0f * PI / 180.0f) * sin(50.0f * PI / 180.0f) * m_fBaseRadius;

	// set ogl 4-dome
	glDepthRange(1.0f, 1.0f);
	glDepthMask(GL_FALSE);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	// move to center - push matrix
	glPushMatrix();
	glTranslatef(vPosition.x, vPosition.y, vPosition.z);

	// clear sky
	glClearColor(vSkyColor.x, vSkyColor.y, vSkyColor.z, 1.0f);

	// renders the basic colored sky-dome
	RenderColoredSkyDome(vSkyColor);

	// enable blending
	glEnable(GL_BLEND);

	// render the stars
	if(m_iStars > 0)
	{
		// set blending, texturing & color for the stars
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE);
		glEnable(GL_TEXTURE_2D);
		m_texStar->Use();

		// render the stars
		RenderStars();
	}

	// set blending, texturing & color for the sun
	glBlendFunc(GL_ONE, GL_ONE);
	glColor4ub(255, 150, 0, 255);
	m_texSun->Use();

	// render sun
	RenderSun(vSun);

	// set blending, texturing & color for the moon
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f - m_fActualGamma);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_texMoon->Use();

	// render moon
	//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderMoon(vMoon);

	// render upper cloud layer
	if(m_texCloudLayer)
	{
		// set blending, texturing & color for the upper cloud layer
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_texCloudLayer->Use();

		// render clouds
		RenderCloudLayer();
	}

	// renders the fog at the sealine
	glTranslatef(0.0f, fShoreLine - vPosition.y, 0.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	RenderFogLayer(vFogColor);

	// restore matrix
	glPopMatrix();

	// set ogl back
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_FOG);
}

//////////////////////////////////////////////////////////////////////////
// RenderFogLayer() : Renders the fog that makes the sky meet the horizon
//////////////////////////////////////////////////////////////////////////

void CSky::RenderFogLayer(const CVector3 & vFogColor) const
{
	float x, z;
	float angle;
	const float next_angle = 2 * PI / m_iResolution;

	// start the circle
	glBegin(GL_TRIANGLE_STRIP);
	for(angle = 0.0f; angle <= 2 * PI + next_angle; angle += next_angle)
	{
		// get vertex coordinates
		x = cos(angle) * m_fBaseRadius;
		z = sin(angle) * m_fBaseRadius;
		
		// upper vertex
		glColor4f(vFogColor.x, vFogColor.y, vFogColor.z, 0.0f);
		glVertex3f(x, 40.0f, z);

		// lower vertex
		glColor4f(vFogColor.x, vFogColor.y, vFogColor.z, 1.0f);
		glVertex3f(x, 0.0f, z);
	}
	glEnd();
}

//////////////////////////////////////////////////////////////////////////
// RenderCloudLayer() : Renders the textured dome
//////////////////////////////////////////////////////////////////////////

void CSky::RenderCloudLayer() const
{
	static int index1 = 0;
	static int index2 = 0;
	static int index3 = 0;
	static int index4 = 0;
	static float windfactor = 0.0f;

	// draw first part with triangle-fan
	glBegin(GL_TRIANGLE_FAN);

	// center vertex -> texture center
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(CloudLayerCoord(0.0f) + windfactor,
		CloudLayerCoord(0.0f) + windfactor);
	glVertex3fv((float *) &m_pVertices[0]);

	// first ring vertices
	for(int i = 1; i < m_iResolution + 2; ++i)
	{
		glTexCoord2f(CloudLayerCoord(m_pVertices[i].x) + windfactor,
			CloudLayerCoord(m_pVertices[i].z) + windfactor);
		glVertex3fv((float *) &m_pVertices[i]);
	}
	glEnd();

	// draw the rest of the dome with t-strip
	for(int rx = 0; rx < m_iResolution - 2; ++rx)
	{
		// compute row transparency
		float upper = 1.0f - CloudLayerTrans(rx);
		float lower = 1.0f - CloudLayerTrans(rx + 1);

		glBegin(GL_TRIANGLE_STRIP);
		for(int rz = 0; rz < m_iResolution; ++rz)
		{
			// compute reusable indices
			index1 = (rx * (m_iResolution + 1)) + rz + 1;
			index2 = ((rx + 1) * (m_iResolution + 1)) + rz + 1;
			index3 = (rx * (m_iResolution + 1)) + rz + 2;
			index4 = ((rx + 1) * (m_iResolution + 1)) + rz + 2;

			// top-left vertex
			glColor4f(upper, upper, upper, upper);
			glTexCoord2f(CloudLayerCoord(m_pVertices[index1].x) + windfactor,
						 CloudLayerCoord(m_pVertices[index1].z) + windfactor);
			glVertex3fv((float *) &m_pVertices[index1]);

			// bottom-left vertex
			glColor4f(lower, lower, lower, lower);
			glTexCoord2f(CloudLayerCoord(m_pVertices[index2].x) + windfactor,
						 CloudLayerCoord(m_pVertices[index2].z) + windfactor);
			glVertex3fv((float *) &m_pVertices[index2]);

			// top-right
			glColor4f(upper, upper, upper, upper);
			glTexCoord2f(CloudLayerCoord(m_pVertices[index3].x) + windfactor,
						 CloudLayerCoord(m_pVertices[index3].z) + windfactor);
			glVertex3fv((float *) &m_pVertices[index3]);

			// bottom-right
			glColor4f(lower, lower, lower, lower);
			glTexCoord2f(CloudLayerCoord(m_pVertices[index4].x) + windfactor,
						 CloudLayerCoord(m_pVertices[index4].z) + windfactor);
			glVertex3fv((float *) &m_pVertices[index4]);
		}
		glEnd();
	}

	windfactor += 0.0001f;
	if(windfactor > 1.0f) windfactor = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// RenderMoon() : Renders the billboarded moon
//////////////////////////////////////////////////////////////////////////

void CSky::RenderMoon(const CVector3 & vMoon) const
{
	// go to moon's position
	glTranslatef(vMoon.x, vMoon.y, vMoon.z);

	// face me
	g_OpenGL->SphericalBillboardBegin();

	// draw moon
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 0);
		glVertex3f(-(MOON_SIZE / 2), MOON_SIZE / 2, 0);

		glTexCoord2i(1, 0);
		glVertex3f(-(MOON_SIZE / 2), -(MOON_SIZE / 2), 0);

		glTexCoord2i(0, 1);
		glVertex3f(MOON_SIZE / 2, MOON_SIZE / 2, 0);

		glTexCoord2i(1, 1);
		glVertex3f(MOON_SIZE / 2, -(MOON_SIZE / 2), 0);
	glEnd();	

	// don't face me
	g_OpenGL->BillboardEnd();

	// get back
	glTranslatef(-vMoon.x, -vMoon.y, -vMoon.z);
}

//////////////////////////////////////////////////////////////////////////
// RenderSun() : Renders the billboarded sun
//////////////////////////////////////////////////////////////////////////

void CSky::RenderSun(const CVector3 & vSun) const
{
	// go to sun's position
	glTranslatef(vSun.x, vSun.y, vSun.z);

	// face me again
	g_OpenGL->SphericalBillboardBegin();

	// draw sun
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 0);
		glVertex3f(-(SUN_SIZE / 2), SUN_SIZE / 2, 0);

		glTexCoord2i(1, 0);
		glVertex3f(-(SUN_SIZE / 2), -(SUN_SIZE / 2), 0);

		glTexCoord2i(0, 1);
		glVertex3f(SUN_SIZE / 2, SUN_SIZE / 2, 0);

		glTexCoord2i(1, 1);
		glVertex3f(SUN_SIZE / 2, -(SUN_SIZE / 2), 0);
	glEnd();

	// don't face me
	g_OpenGL->BillboardEnd();

	// go back
	glTranslatef(-vSun.x, -vSun.y, -vSun.z);
}

//////////////////////////////////////////////////////////////////////////
// RenderStars() : Renders the billboarded stars
//////////////////////////////////////////////////////////////////////////

void CSky::RenderStars() const
{
	// render ALL-STARS
	for(int i = 0; i < m_iStars; ++i)
	{
		// compute half the star size
		float fStarSize2 = m_pStars[i].fSize / 2.0f;

		// go to star's position
		glTranslatef(m_pStars[i].vPos.x, m_pStars[i].vPos.y, m_pStars[i].vPos.z);

		// spherical billboard begin (*always* face the camera)
		g_OpenGL->SphericalBillboardBegin();

		// set star color & transparency
		glColor4f(m_pStars[i].vColor.x, m_pStars[i].vColor.y,
			m_pStars[i].vColor.z, m_fActualGamma);

		// draw star
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, 0);
			glVertex3f(-fStarSize2, fStarSize2, 0.0f);

			glTexCoord2i(1, 0);
			glVertex3f(-fStarSize2, -fStarSize2, 0);

			glTexCoord2i(0, 1);
			glVertex3f(fStarSize2, fStarSize2, 0.0f);

			glTexCoord2i(1, 1);
			glVertex3f(fStarSize2, -fStarSize2, 0.0f);
		glEnd();

		// don't face me anymore
		g_OpenGL->BillboardEnd();

		// go back from the star's position to the original position
		glTranslatef(-m_pStars[i].vPos.x, -m_pStars[i].vPos.y, -m_pStars[i].vPos.z);
	}
}

//////////////////////////////////////////////////////////////////////////
// RenderColoredSkyDome() : Renders the colored dome
//////////////////////////////////////////////////////////////////////////

void CSky::RenderColoredSkyDome(const CVector3 & vSkyColor) const
{
	CVector3 uppercolor;
	CVector3 lowercolor;

	// draw first part with triangle-fan
	glBegin(GL_TRIANGLE_FAN);

	// crown color	
	uppercolor = DomeColor(vSkyColor, 0);
	lowercolor = DomeColor(vSkyColor, 1);

	// first vertex
	glColor3fv((float *) &uppercolor);
	glVertex3fv((float *) &m_pVertices[0]);

	// first crown
	for(int i = 1; i < m_iResolution + 2; ++i)
	{
		
		glColor3fv((float *) &lowercolor);
		glVertex3fv((float *) &m_pVertices[i]);
	}

	// no more fun (fan) ;)
	glEnd();

	// draw the rest of the dome with t-strip
	for(int rx = 0; rx < m_iResolution - 2; ++rx)
	{
		// begin triengle strip
		glBegin(GL_TRIANGLE_STRIP);
		
		// dance around the clock
		for(int rz = 0; rz < m_iResolution; ++rz)
		{
			uppercolor = DomeColor(vSkyColor, rx);
			lowercolor = DomeColor(vSkyColor, rx + 1);

			// top-left vertex
			glColor3fv((float *) &uppercolor);
			glVertex3fv((float *) &m_pVertices[(rx * (m_iResolution + 1)) + rz + 1]);

			// bottom-left vertex
			glColor3fv((float *) &lowercolor);
			glVertex3fv((float *) &m_pVertices[((rx + 1) * (m_iResolution + 1)) + rz + 1]);

			// top-right
			glColor3fv((float *) &uppercolor);
			glVertex3fv((float *) &m_pVertices[(rx * (m_iResolution + 1)) + rz + 2]);

			// bottom-right
			glColor3fv((float *) &lowercolor);
			glVertex3fv((float *) &m_pVertices[((rx + 1) * (m_iResolution + 1)) + rz + 2]);
		}

		// end triangle strip
		glEnd();
	}
}

//////////////////////////////////////////////////////////////////////////
// CloudLayerCoord() : Returns the texture coord for that point in the dome
//////////////////////////////////////////////////////////////////////////

inline float CSky::CloudLayerCoord(float fDomeCoord) const
{
	return (fDomeCoord / (m_fBaseRadius * 0.9f - 1.0f) + 1.0f) * 0.5f;
}

inline float CSky::CloudLayerTrans(int iRing) const
{
	float r = ((float)iRing / (float)(m_iResolution - 2) - 0.6f) / 0.3f;
	return (r >= 0.0f ? r : 0.0f);
}

inline CVector3 CSky::DomeColor(const CVector3 & vSkyColor, int iRing) const
{
	return vSkyColor - (0.1f - 0.1f * ((float)iRing / (float)(m_iResolution - 2)));
}

//////////////////////////////////////////////////////////////////////////
// ExponentialCurve() : Returns a sharpness/saturation modified value
//////////////////////////////////////////////////////////////////////////

inline unsigned char CSky::ExponentialCurve(unsigned char ubValue,
				unsigned char ubSaturation, float fSharpness) const
{
	// calculate exponent
	int iExponent = ubValue - ubSaturation;
	if(iExponent < 0) iExponent = 0;

	// return exp curve
	return ((unsigned char) (255 - (pow(fSharpness, iExponent) * 255)));
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
