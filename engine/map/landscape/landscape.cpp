#include "../../precompiled/stdafx.h"
#include "../../engine/base.h"

//////////////////////////////////////////////////////////////////////////

#define MAX_TRIANGLES	256
#define MULTITEXTURE	"GL_ARB_multitexture"

//////////////////////////////////////////////////////////////////////////
// CLandscape constructor & destructor
//////////////////////////////////////////////////////////////////////////

CLandscape::CLandscape()
{
	m_pMesh = NULL;
	m_pQuadTree = NULL;
	m_pTexture = NULL;
	m_pDetailTexture = NULL;

	m_iLength = 0;
	m_fScale = 0.0f;
	m_iMaxSubdivisions = 0;
}

CLandscape::~CLandscape()
{
	// free
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates the landscape
//////////////////////////////////////////////////////////////////////////

bool CLandscape::Create(int iLength, float fScale,
						const char *szHeightmap, const char *szTexture)
{
	CFile file;
	unsigned char *file_buffer = NULL;

	// free old data
	Destroy();

	// set local parameters
	m_iLength = iLength;
	m_fScale = fScale;

	// create heightmap stream
	file.Open(szHeightmap, FILE_PACKAGE, FILE_BINARY);
	if(!file.IsOpen()) return false;

	// allocate heightmap
	int size = file.Size();
	file_buffer = new unsigned char [size];
	if(!file_buffer) return false;

	// read heightmap
	file.Read(file_buffer, size);

	// close the file
	file.Close();

	// allocate mesh array
	m_pMesh = new TTexturedVertex [iLength * iLength];
	if(!m_pMesh) return false;

	// temporary variables
	float my_y;
	int x, y, index;

	// create mesh
	for(y = 0; y < iLength; ++y)
	{
		for(x = 0; x < iLength; ++x)
		{
			index = (y * iLength) + x;

			// vertex coords (flip vertical)
			m_pMesh[index].x  = (float) x;
			m_pMesh[index].y  = (float) file_buffer[((iLength - y) * iLength) + x] * fScale;
			m_pMesh[index].z  = (float) -y;

			// clear normals
			m_pMesh[index].nx  = (float)  0.0f;
			m_pMesh[index].ny  = (float) -1.0f;
			m_pMesh[index].nz  = (float)  0.0f;

			// texture coords
			m_pMesh[index].tx = (float) x / (float) m_iLength;
			m_pMesh[index].ty = (float) y / (float) m_iLength;
		} 
	}

	// free heightmap
	delete [] file_buffer;

	// calculate normals
	for(y = 0; y < iLength - 1; ++y)
	{
		for(x = 0; x < iLength - 1; ++x)
		{
			CVector3 v1, v2, v3, a, b, n;

			// first vector
			index = (y * iLength) + x;
			v1.x = m_pMesh[index].x;
			v1.y = m_pMesh[index].y;
			v1.z = m_pMesh[index].z;

			// second vector
			index = (y * iLength) + x + 1;
			v2.x = m_pMesh[index].x;
			v2.y = m_pMesh[index].y;
			v2.z = m_pMesh[index].z;

			// third vector
			index = ((y + 1) * iLength) + x;
			v3.x = m_pMesh[index].x;
			v3.y = m_pMesh[index].y;
			v3.z = m_pMesh[index].z;

			// vectors for the normal
			a = v3 - v1;
			b = v2 - v1;

			// normal
			n = Cross(b, a);
			n = Normalize(n);

			// assign
			index = (y * iLength) + x;
			m_pMesh[index].nx  = n.x;
			m_pMesh[index].ny  = n.y;
			m_pMesh[index].nz  = n.z;
		}
	}

	// geo-smoothing
	for(y = 1; y < iLength - 1; ++y)
	{
		for(x = 1; x < iLength - 1; ++x)
		{
			my_y = m_pMesh[(y * iLength) + x].y;

			// up
			m_pMesh[((y - 1) * iLength) + x].y += my_y;
			m_pMesh[((y - 1) * iLength) + x].y /= 2.0f;

			// down
			m_pMesh[((y + 1) * iLength) + x].y += my_y;
			m_pMesh[((y + 1) * iLength) + x].y /= 2.0f;

			// left
			m_pMesh[(y * iLength) + x - 1].y += my_y;
			m_pMesh[(y * iLength) + x - 1].y /= 2.0f;

			// right
			m_pMesh[(y * iLength) + x + 1].y += my_y;
			m_pMesh[(y * iLength) + x + 1].y /= 2.0f;
		}
	}

	// set max subdivisions
	m_iMaxSubdivisions = (int) (log((m_iLength-1) * (m_iLength-1) * 2 / MAX_TRIANGLES) / log(4));

	// create quad tree
	if(!CreateQuadTree())
	{
		Destroy();
		return false;
	}

	// allocate landscape texture
	m_pTexture = new CTexture();
	if(!m_pTexture) return false;

	// load landscape texture
	if(!m_pTexture->Create(szTexture, FIF_JPEG, false))
	{
		Destroy();
		return false;
	}

	// if we can have multitexturing then we load the detail texture
	if(g_OpenGL->GetExtension(MULTITEXTURE))
	{
		// allocate
		m_pDetailTexture = new CTexture();
		if(!m_pDetailTexture) return true;

		// load
		if(!m_pDetailTexture->Create("gfx/environment.pak/detail.jpg",
			FIF_JPEG, true))
		{
			// free
			delete m_pDetailTexture;
			return true;
		}
	}

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Frees landscape data
//////////////////////////////////////////////////////////////////////////

void CLandscape::Destroy()
{
	// reset variables
	m_iLength = 0;
	m_fScale = 0.0f;
	m_iMaxSubdivisions = 0;

	// destroy heightmap
	delete [] m_pMesh;
	m_pMesh = NULL;

	// destroy quad tree
	delete m_pQuadTree;
	m_pQuadTree = NULL;

	// delete map texture
	delete m_pTexture;
	m_pTexture = NULL;

	// delete map detail texture
	delete m_pDetailTexture;
	m_pDetailTexture = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Render() : Renders the landscape tree
//////////////////////////////////////////////////////////////////////////

void CLandscape::Render()
{
	// draw quadtree
	if(m_pQuadTree) m_pQuadTree->Render();
}

//////////////////////////////////////////////////////////////////////////
// CreateQuadTree() : Creates the landscape tree
//////////////////////////////////////////////////////////////////////////

bool CLandscape::CreateQuadTree()
{
	// create quad tree
	m_pQuadTree = new CQuadNode();
	if(!m_pQuadTree) return false;

	// recurse quadtree
	int iHalf = (m_iLength - 1) / 2;
	m_pQuadTree->Create(iHalf, iHalf, m_iLength - 1, 0, this);

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
