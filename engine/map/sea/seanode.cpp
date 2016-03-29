#include "../../precompiled/stdafx.h"
#include "../../engine/base.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

CSeaNode::CSeaNode()
{
	m_iSize = 0;
	m_iNodeCenterX = 0;
	m_iNodeCenterY = 0;
	m_bSubdivided = false;

	m_pSea = NULL;
	m_pTopLeft = NULL;
	m_pTopRight = NULL;
	m_pBottomLeft = NULL;
	m_pBottomRight = NULL;
}

CSeaNode::~CSeaNode()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CSeaNode::Create(int iCenterX, int iCenterY, int iLength, int iLevel, CLandscape *pLandscape, CSea *pSea)
{
	// get node x and y
	int iNodeX = iCenterX - iLength / 2;
	int iNodeY = iCenterY - iLength / 2;

	// set info for this node
	m_pSea = pSea;
	m_iNodeCenterX = iCenterX;
	m_iNodeCenterY = iCenterY;
	m_iSize = iLength;

	if(m_iSize > TESSELATION_MAX_SIZE)
	{
		// check if we have some terrain above us
		int x, y;
		bool bUnderWater = false;

		for(y = iNodeY; y < iNodeY + iLength; ++y)
		{
			for(x = iNodeX; x < iNodeX + iLength; ++x)
			{
				 if(pLandscape->HeightAt(x, y) < pSea->GetShoreLine() + WAVE_MAX_AMPLITUDE)
				 {
					 bUnderWater = true;
					 break;
				 }
			}
			
			// if we already have water above us, we're done here
			if(bUnderWater) break;
		}

		// if we have terrain above us then we'll never be seen!
		if(!bUnderWater) return true;

		// set sizes
		int l2 = iLength / 2;
		int l4 = iLength / 4;

		// continue subdividing
		m_bSubdivided = true;

		// top left node
		m_pTopLeft = new CSeaNode();
		if(!m_pTopLeft) return false;
		m_pTopLeft->Create(iCenterX - l4, iCenterY - l4, l2, iLevel + 1, pLandscape, pSea);

		// top right node
		m_pTopRight = new CSeaNode();
		if(!m_pTopRight) return false;
		m_pTopRight->Create(iCenterX + l4, iCenterY - l4, l2, iLevel + 1, pLandscape, pSea);

		// bottom left node
		m_pBottomLeft = new CSeaNode();
		if(!m_pBottomLeft) return false;
		m_pBottomLeft->Create(iCenterX - l4, iCenterY + l4, l2, iLevel + 1, pLandscape, pSea);

		// bottom right node
		m_pBottomRight = new CSeaNode();
		if(!m_pBottomRight) return false;
		m_pBottomRight->Create(iCenterX + l4, iCenterY + l4, l2, iLevel + 1, pLandscape, pSea);
	}
	else
	{
		float pos_x, pos_y;

		// set top left coords & sin wave position
		pos_x = (float)iNodeX / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;
		pos_y = (float)iNodeY / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;

		m_vTopLeft = CVector2(pos_x, pos_y);

		// set bottom left coords & sin wave position
		pos_x = (float)iNodeX / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;
		pos_y = (float)(iNodeY + iLength) / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;

		m_vBottomLeft = CVector2(pos_x, pos_y);

		// set top right coords & sin wave position
		pos_x = (float)(iNodeX + iLength) / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;
		pos_y = (float)iNodeY / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;

		m_vTopRight = CVector2(pos_x, pos_y);

		// set bottom right coords & sin wave position
		pos_x = (float)(iNodeX + iLength) / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;
		pos_y = (float)(iNodeY + iLength) / (float)m_pSea->m_iLength * (float)m_iSize * 2.0f * (float)PI;

		m_vBottomRight = CVector2(pos_x, pos_y);
	}

	// no errors
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CSeaNode::Render()
{
	// frustum culling
	if(!g_Frustum->BoxInFrustum((float) m_iNodeCenterX,
								(float) m_pSea->GetShoreLine() / 2.0f,
								(float) -m_iNodeCenterY,
								(float) m_iSize,
								(float) m_pSea->GetShoreLine() + (WAVE_MAX_AMPLITUDE / 2),
								(float) m_iSize)) return;

	// is it subdivided?
	if(m_bSubdivided)
	{
		m_pTopLeft->Render();
		m_pTopRight->Render();
		m_pBottomLeft->Render();
		m_pBottomRight->Render();
	}
	else
	{
		// not a final node
		if(m_iSize > TESSELATION_MAX_SIZE) return;

		// get actual wave position sin
		float	fWavePhase = m_pSea->GetPhase();
		float	fShoreLine = m_pSea->GetShoreLine();
		float	fTexPosition = m_pSea->GetTexturePos();
		int		iNodeX = m_iNodeCenterX - m_iSize / 2;
		int		iNodeY = m_iNodeCenterY - m_iSize / 2;

		// select texture and set alpha settings
		m_pSea->m_pWaterTextures->Use();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(20, 80, 120, 170);

		// start triangle strip
		glBegin(GL_TRIANGLE_STRIP);

		// upper left point
		glTexCoord2f(0.0f + fTexPosition, 0.0f + fTexPosition);
		glNormal3i(0, 1, 0);
		glVertex3f(iNodeX, fShoreLine + sin(m_vTopLeft.x + fWavePhase) * sin(m_vTopLeft.y + fWavePhase) * WAVE_MAX_AMPLITUDE, -iNodeY);

		// lower left point
		glTexCoord2f(0.0f + fTexPosition, m_iSize * WATER_SIZE + fTexPosition);
		glNormal3i(0, 1, 0);
		glVertex3f(iNodeX, fShoreLine + sin(m_vBottomLeft.x + fWavePhase) * sin(m_vBottomLeft.y + fWavePhase) * WAVE_MAX_AMPLITUDE, -(iNodeY + m_iSize));

		// upper right point
		glTexCoord2f(m_iSize * WATER_SIZE + fTexPosition, 0.0f + fTexPosition);
		glNormal3i(0, 1, 0);
		glVertex3f(iNodeX + m_iSize, fShoreLine + sin(m_vTopRight.x + fWavePhase) * sin(m_vTopRight.y + fWavePhase) * WAVE_MAX_AMPLITUDE, -iNodeY);

		// lower right point
		glTexCoord2f(m_iSize * WATER_SIZE + fTexPosition, m_iSize * WATER_SIZE + fTexPosition);
		glNormal3i(0, 1, 0);
		glVertex3f(iNodeX + m_iSize, fShoreLine + sin(m_vBottomRight.x + fWavePhase) * sin(m_vBottomRight.y + fWavePhase) * WAVE_MAX_AMPLITUDE, -(iNodeY + m_iSize));

		// finish triangle strip
		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CSeaNode::Destroy()
{
	// reset variables
	m_iSize = 0;
	m_iNodeCenterX = 0;
	m_iNodeCenterY = 0;
	m_bSubdivided = false;

	// set vectors to 0
	m_vTopLeft = CVector2(0, 0);
	m_vBottomLeft = CVector2(0, 0);
	m_vTopRight = CVector2(0, 0);
	m_vBottomRight = CVector2(0, 0);

	// do not link to any sea anymore
	m_pSea = NULL;

	// delete top left node
	delete m_pTopLeft;
	m_pTopLeft = NULL;

	// delete top right node
	delete m_pTopRight;
	m_pTopRight = NULL;

	// delete bottom left node
	delete m_pBottomLeft;
	m_pBottomLeft = NULL;

	// delete bottom node
	delete m_pBottomRight;
	m_pBottomRight = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
