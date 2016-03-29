#ifndef __SKY_H__
#define __SKY_H__

//////////////////////////////////////////////////////////////////////////
// CSky class
//////////////////////////////////////////////////////////////////////////

class CSky {
public:
	// constructor & destructor
	CSky();
	~CSky();

	// creates a sky
	bool Create(float fHeight, float fBaseRadius, int iResolution,
				int iMinStars, int iMaxStars,
				float fCloudCover, CVector3 & vCenter,
				CVector3 & vNightBaseColor,	CVector3 & vDayBaseColor,
				CVector3 & vNightBaseAmbient, CVector3 & vDayBaseAmbient);

	// frees the sky
	void Destroy();

	// returns the actual time of day 0.0 - 1.0
	float GetDayTime() const { return m_fDayTime; }

	// sets the actual time of day 0.0 - 1.0
	void SetDayTime(float fDayTime) { m_fDayTime = fDayTime; }

	// returns y-scale height
	float GetHeight() const { return m_fHeight; }

	// sets y-scale height
	void SetHeight(float fHeight) { m_fHeight = fHeight; }

	// renders the skydome
	void Render(const CVector3 & vPosition, float fShoreLine,
		const CVector3 & vFogColor);

private:
	// creates the dome mesh
	bool CreateMesh(int iResolution, float fBaseRadius, float fHeight);

	// creates the stars
	bool CreateStars(int iMinStars, int iMaxStars,
		float fBaseRadius, float fHeight);

	// creates perlin noise clouds
	bool CreateClouds(float fCloudCover);

	// loads some textures
	bool LoadTextures();

	// renders the colored dome
	void RenderColoredSkyDome(const CVector3 & vSkyColor) const;

	// renders the billboarded stars
	void RenderStars() const;

	// renders the billboarded sun
	void RenderSun(const CVector3 & vSun) const;

	// renders the billboarded moon
	void RenderMoon(const CVector3 & vMoon) const;

	// renders the textured dome
	void RenderCloudLayer() const;

	// renders the fog that makes the sky meet the horizon
	void RenderFogLayer(const CVector3 & vFogColor) const;

	// returns the needed texture coord
	float CloudLayerCoord(float fDomeCoord) const;

	// returns the transparency at that point
	float CloudLayerTrans(int iRing) const;

	// returns the desired dome color at ring
	CVector3 DomeColor(const CVector3 & vSkyColor, int iRing) const;

	// returns a sharpness/saturation modified value
	unsigned char ExponentialCurve(unsigned char ubValue,
		unsigned char ubSaturation, float fSharpness) const;

private:
	// star structure
	typedef struct {
		CVector3 vPos;
		CVector3 vColor;
		float fSize;
	} TStar;

private:
	// holds the x-resolution
	int m_iResolution;

	// holds the skydome height
	float m_fHeight;

	// holds the skydome's base radius
	float m_fBaseRadius;

	// holds the actual datime
	float m_fDayTime;

	// holds the actual gamma
	float m_fActualGamma;

	// holds all the skydome's mesh vertices
	CVector3 *m_pVertices;

	// holds the number of stars in the sky
	int	m_iStars;

	// holds the stars in the sky
	TStar *m_pStars;

	// moon texture
	CTexture *m_texMoon;

	// sun texture
	CTexture *m_texSun;

	// star texture
	CTexture *m_texStar;

	// cloud layer texture
	CTexture *m_texCloudLayer;

	// relative sky center
	CVector3 m_vCenter;

	// day/night colors
	CVector3 m_vNightBaseColor;
	CVector3 m_vDayBaseColor;
	CVector3 m_vNightBaseAmbient;
	CVector3 m_vDayBaseAmbient;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif