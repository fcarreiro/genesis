#include "../../precompiled/stdafx.h"
#include "../../engine/base.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

#define STEPS 4

////////////////////////////////////////////////////////////////////////////////////////////////////

CQuadNode::CQuadNode()
{
	m_fMaxHeight = 0.0f;
	m_fMinHeight = 0.0f;
	m_bSubdivided = false;
	m_iNodeX = 0;
	m_iNodeY = 0;
	m_iSize = 0;

	m_pLandscape = NULL;
	m_pTopLeft = NULL;
	m_pTopRight = NULL;
	m_pBottomLeft = NULL;
	m_pBottomRight = NULL;
}

CQuadNode::~CQuadNode()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool CQuadNode::Create(int iCenterX, int iCenterY, int iLength, int iLevel, CLandscape *pLandscape)
{
	// set info for this node
	m_iSize = iLength;
	m_pLandscape = pLandscape;
	m_iNodeX = (iCenterX - (iLength / 2));
	m_iNodeY = (iCenterY - (iLength / 2));

	// find heights
	float h;
	int x, y;
	m_fMinHeight = 255.0f * pLandscape->m_fScale;

	// set max & min heights of this sector
	for(y = m_iNodeY; y < m_iNodeY + m_iSize; ++y)
	{
		for(x = m_iNodeX; x < m_iNodeX + m_iSize; ++x)
		{
			h = (float) m_pLandscape->HeightAt(x, y);
			if(h > m_fMaxHeight) m_fMaxHeight = h;
			if(h < m_fMinHeight) m_fMinHeight = h;
		}
	}

	// subdivide?
	if(iLevel < pLandscape->m_iMaxSubdivisions)
	{
		int l2 = iLength / 2;
		int l4 = iLength / 4;

		// continue subdividing
		m_bSubdivided = true;

		// top left node
		m_pTopLeft = new CQuadNode();
		if(!m_pTopLeft) return false;
		m_pTopLeft->Create(iCenterX - l4, iCenterY - l4, l2, iLevel + 1, pLandscape);

		// top right node
		m_pTopRight = new CQuadNode();
		if(!m_pTopRight) return false;
		m_pTopRight->Create(iCenterX + l4, iCenterY - l4, l2, iLevel + 1, pLandscape);

		// bottom left node
		m_pBottomLeft = new CQuadNode();
		if(!m_pBottomLeft) return false;
		m_pBottomLeft->Create(iCenterX - l4, iCenterY + l4, l2, iLevel + 1, pLandscape);

		// bottom right node
		m_pBottomRight = new CQuadNode();
		if(!m_pBottomRight) return false;
		m_pBottomRight->Create(iCenterX + l4, iCenterY + l4, l2, iLevel + 1, pLandscape);
	}

	// no errors
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CQuadNode::Render()
{
	// do we have the multitexture extension available?
	static bool ARB = g_OpenGL->GetExtension("GL_ARB_multitexture");

	// frustum culling
	float y_size = m_fMaxHeight - m_fMinHeight;
	float y = m_fMinHeight + (y_size / 2);

	if(!g_Frustum->BoxInFrustum((float) m_iNodeX + (m_iSize / 2),
								(float) y,
								(float) -(m_iNodeY + (m_iSize / 2)),
								(float) m_iSize,
								(float) y_size,
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
		////////////////////////////////
		// LANDSCAPE
		////////////////////////////////

		// full color
		glColor3ub(255, 255, 255);

		// texture (multipass)
		if(ARB)	glActiveTextureARB(GL_TEXTURE0_ARB);

		// map texture
		glEnable(GL_TEXTURE_2D);
		m_pLandscape->m_pTexture->Use();

		// multi texture
		if(0)
		{
			if(1) // above water
			{
				// detail texture
				glActiveTextureARB(GL_TEXTURE1_ARB);
				glEnable(GL_TEXTURE_2D);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
				m_pLandscape->m_pDetailTexture->Use();
			}
			else // underwater
			{
				// water texture
//				glActiveTextureARB(GL_TEXTURE1_ARB);
//				glEnable(GL_TEXTURE_2D);
//				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
//				glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
//				m_pLandscape->m_pWaterTexture[0].Use();
			}
		}

		// allocate variables
		int x, y;
		TTexturedVertex *v;

		// loop
		for(y = m_iNodeY; y < m_iNodeY + m_iSize; y += STEPS)
		{
			glBegin(GL_TRIANGLE_STRIP);

			for(x = m_iNodeX + m_iSize; x >= m_iNodeX; x -= STEPS)
			{
				////////////////////////////////
				// FIRST POINT
				////////////////////////////////

				// get vertex
				v = m_pLandscape->VertexAt(x, y);

				// if multipass is enabled
				if(ARB)
				{
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, v->tx, v->ty);
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (float)(x * 0.125), (float)(y * 0.125));
				}
				else
				{
					glTexCoord2f(v->tx, v->ty);
				}

				// pass normal
				glNormal3f(v->nx, v->ny, v->nz);

				// pass vertex
				glVertex3f(v->x, v->y, v->z);

				////////////////////////////////
				// SECOND POINT
				////////////////////////////////

				// get vertex
				v = m_pLandscape->VertexAt(x, y + STEPS);

				// if multipass is enabled
				if(ARB)
				{
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, v->tx, v->ty);
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (float)(x * 0.125), (float)((y + STEPS) * 0.125));
				}
				else
				{
					glTexCoord2f(v->tx, v->ty);
				}

				// pass normal
				glNormal3f(v->nx, v->ny, v->nz);

				// pass vertex
				glVertex3f(v->x, v->y, v->z);
			}

			glEnd();
		}

		// turn the second multitexture pass off
		if(ARB)	glActiveTextureARB(GL_TEXTURE1_ARB);

		// turn texturing off
		glDisable(GL_TEXTURE_2D);

		// turn the first multitexture pass off
		if(ARB)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB);		
			glDisable(GL_TEXTURE_2D);
		}

		/************************ SHOW NORMALS ****************************/
		/*glColor3ub(255,0,0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		// loop
		for(y = m_iNodeY; y < m_iNodeY + m_iSize; y += STEPS)
		{
			for(x = m_iNodeX + m_iSize; x >= m_iNodeX; x -= STEPS)
			{
				// get vertex
				v = m_pLandscape->VertexAt(x, y);

				glVertex3f(v->x, v->y, v->z);

				CVector3 a(v->x, v->y, v->z);
				CVector3 b(v->nx, v->ny, v->nz);
				CVector3 c;
				c = a + b;
				glVertex3fv((float*)&c);
			}
		}
		glEnd();*/
		/****************************************************/
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CQuadNode::Destroy()
{
	// reset variables
	m_fMaxHeight = 0.0f;
	m_fMinHeight = 0.0f;
	m_bSubdivided = false;
	m_iNodeX = 0;
	m_iNodeY = 0;
	m_iSize = 0;

	// do not link to any landscape anymore
	m_pLandscape = NULL;

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
