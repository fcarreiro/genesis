#ifndef __QUADNODE_H__
#define __QUADNODE_H__

//////////////////////////////////////////////////////////////////////////
// CQuadNode class
//////////////////////////////////////////////////////////////////////////

class CQuadNode
{
public:
	// contructor & destructor
	CQuadNode();
	~CQuadNode();

	// creates the quadtree given a landscape
	bool Create(int iCenterX, int iCenterY,
		int iLength,int iLevel, CLandscape *pLandscape);

	// frees the quadtree
	void Destroy();

	// recursively renders the quadtree
	void Render();

private:
	// max height of this sector
	float m_fMaxHeight;

	// min height of this sector
	float m_fMinHeight;

	// do we have children?
	bool m_bSubdivided;

	// node's x coord
	int	m_iNodeX;

	// node's y coord
	int	m_iNodeY;

	// node size
	int	m_iSize;

	// pointer to the original landscape
	CLandscape *m_pLandscape;

	// top-left node pointer
	CQuadNode *m_pTopLeft;

	// top-right node pointer
	CQuadNode *m_pTopRight;

	// bottom-left node pointers
	CQuadNode *m_pBottomLeft;

	// bottom-right node pointer
	CQuadNode *m_pBottomRight;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
