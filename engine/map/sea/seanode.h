#ifndef __SEANODE_H__
#define __SEANODE_H__

////////////////////////////////////////////////////////////////////////

class CSea;
class CLandscape;

////////////////////////////////////////////////////////////////////////

class CSeaNode
{
public:
	// contructor & destructor
	CSeaNode();
	~CSeaNode();

	// functions
	bool Create(int iCenterX, int iCenterY, int iLength, int iLevel, CLandscape *pLandscape, CSea *pSea);
	void Destroy();

	void Render();

private:
	// variables
	int			m_iSize;
	bool		m_bSubdivided;
	int			m_iNodeCenterX;
	int			m_iNodeCenterY;

	CVector2	m_vTopLeft;
	CVector2	m_vBottomLeft;
	CVector2	m_vTopRight;
	CVector2	m_vBottomRight;

	CSea		*m_pSea;
	CSeaNode	*m_pTopLeft;
	CSeaNode	*m_pTopRight;
	CSeaNode	*m_pBottomLeft;
	CSeaNode	*m_pBottomRight;
};

////////////////////////////////////////////////////////////////////////

#endif