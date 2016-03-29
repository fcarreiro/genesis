#ifndef __SEA_H__
#define __SEA_H__

//////////////////////////////////////////////////////////////////////////////////

#define WATER_CYCLE				3
#define WATER_SIZE				0.125
#define TESSELATION_MAX_SIZE	10
#define WAVE_MAX_AMPLITUDE		3

////////////////////////////////////////////////////////////////////////

class CSea {
public:
	// friends
	friend class CSeaNode;

	// constructor & destructor
	CSea();
	~CSea();

	// functions
	bool Create(int iLength, float fShoreLine, CLandscape *pLandscape);
	void Destroy();

	void SetShoreLine(float fShoreLine) { m_fShoreLine = fShoreLine; }
	float GetShoreLine() { return m_fShoreLine; }
	float GetPhase() { return m_fPhase; }
	float GetTexturePos() { return m_fTexCoord; }

	void Render();

private:
	// variables
	int			m_iLength;
	float		m_fShoreLine;
	float		m_fPhase;
	float		m_fTexCoord;

	CTimer		m_tTimer;

	CTexture	*m_pWaterTextures;
	CTexture	*m_pDropTextures;
	CSeaNode	*m_pSeaTree;
};

////////////////////////////////////////////////////////////////////////

#endif