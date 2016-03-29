#include "../../precompiled/stdafx.h"
#include "../../engine/base.h"

//////////////////////////////////////////////////////////////////////////////////

#define SEA_SPEED		1.0f
#define TEXTURE_SPEED	0.5f

//////////////////////////////////////////////////////////////////////////////////

CSea::CSea()
{
	m_iLength = 0;
	m_fShoreLine = 0.0f;
	m_fPhase = 0.0f;
	m_fTexCoord = 0.0f;

	m_pDropTextures = NULL;
	m_pWaterTextures = NULL;
	m_pSeaTree = NULL;
}

CSea::~CSea()
{
	// free
	Destroy();
}

//////////////////////////////////////////////////////////////////////////////////

bool CSea::Create(int iLength, float fShoreLine, CLandscape *pLandscape)
{
	// free
	Destroy();

	// set local
	m_iLength = iLength;
	m_fShoreLine = fShoreLine;

	// create water texture objects
	m_pWaterTextures = new CTexture();
	if(!m_pWaterTextures) return false;

	// read 16 textures
	if(!m_pWaterTextures->Create("gfx/environment.pak/wave.png",	FIF_PNG, true))
	{
		return false;
	}

	// create quadtree
	m_pSeaTree = new CSeaNode();
	if(!m_pSeaTree) return false;

	int iHalf = (m_iLength - 1) / 2;
	m_pSeaTree->Create(iHalf, iHalf, m_iLength - 1, 0, pLandscape, this);
	
	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

void CSea::Destroy()
{
	// reset variables
	m_iLength = 0;
	m_fShoreLine = 0.0f;
	m_fPhase = 0.0f;
	m_fTexCoord = 0.0f;

	// destroy drop textures
	delete [] m_pDropTextures;
	m_pDropTextures = NULL;

	// destroy water textures
	delete m_pWaterTextures;
	m_pWaterTextures = NULL;

	// destroy tree
	delete m_pSeaTree;
	m_pSeaTree = NULL;
}

//////////////////////////////////////////////////////////////////////////////////

void CSea::Render()
{
	// update timer
	float fTimePassed = (float)m_tTimer.GetTimePassed() / 1000.0f;
	m_tTimer.Update();

	// add sine sea movement
	m_fPhase += fTimePassed * SEA_SPEED;
	if(m_fPhase >= 2 * PI) m_fPhase = 0.0f;

	// add texture sea movement
	m_fTexCoord += fTimePassed * TEXTURE_SPEED;
	if(m_fTexCoord >= 1.0f) m_fTexCoord = 0.0f;

	// render tree
	m_pSeaTree->Render();
}

//////////////////////////////////////////////////////////////////////////////////
