#ifndef __HEIGHTMAP_H__
#define __HEIGHTMAP_H__

//////////////////////////////////////////////////////////////////////////
// Vertex shader struct
//////////////////////////////////////////////////////////////////////////

typedef struct
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float tx;
	float ty;
} TTexturedVertex;

//////////////////////////////////////////////////////////////////////////
// CLandscape class
//////////////////////////////////////////////////////////////////////////

class CLandscape
{
public:
	// friends
	friend class CQuadNode;
	friend class CMap;

	// contructor & destructor
	CLandscape();
	~CLandscape();

	// creates a landscape from a heightmap and a texture
	bool Create(int iLength, float fScale,
		const char *szHeightmap, const char *szTexture);

	// unloads everything
	void Destroy();

	// returns true if we have a map loaded
	bool IsLoaded() { return (m_pMesh != NULL && m_pTexture != NULL); }

	// draws the landscape
	void Render();

	// returns the height in an specific coord
	float HeightAt(int x, int y) {
		if(m_pMesh) return m_pMesh[(y * m_iLength) + x].y;
		else return 0; }

	// returns a pointer to the vertex in a specific coord
	TTexturedVertex *VertexAt(int x, int y) {
		if(m_pMesh) return &m_pMesh[(y * m_iLength) + x];
		else return NULL; }

private:
	// creates the quadtree to help frustum culling
	bool CreateQuadTree();

	// destroys the quadtree
	void DestroyQuadTree();

private:
	// holds the length of the landscape
	int	m_iLength;

	// holds the scale relationship with the original heightmap
	float m_fScale;

	// max quadtree subdivisions
	int	m_iMaxSubdivisions;

	// this is the landscape mesh
	TTexturedVertex	*m_pMesh;

	// this is the quadtree
	CQuadNode *m_pQuadTree;

	// holds the terrain texture
	CTexture *m_pTexture;

	// optional detail texture
	CTexture *m_pDetailTexture;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
