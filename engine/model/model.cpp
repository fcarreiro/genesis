#include "../precompiled/stdafx.h"
#include "../math/vectors.h"
#include "../math/3dmath.h"
#include "../file_io/file_base.h"
#include "../file_io/package.h"
#include "../file_io/standard.h"
#include "../file_io/file.h"
#include "../texture/texture.h"
#include "model.h"

//////////////////////////////////////////////////////////////////////////
// MD3 structures
//////////////////////////////////////////////////////////////////////////

// mesh data
typedef struct
{
	// This stores the mesh ID (We don't care)
	char meshID[4];

	// This stores the mesh name (We do care)
	char strName[68];

	// This stores the mesh aniamtion frame count
	int numMeshFrames;

	// This stores the mesh skin count
	int numSkins;

	// This stores the mesh vertex count
	int numVertices;

	// This stores the mesh face count
	int numTriangles;

	// This stores the starting offset for the triangles
	int triStart;

	// This stores the header size for the mesh
	int headerSize;

	// This stores the starting offset for the UV coordinates
	int uvStart;

	// This stores the starting offset for the vertex indices
	int vertexStart;

	// This stores the total mesh size
	int meshSize;
} TMd3MeshInfo;

// tag
typedef struct
{
	// This stores the name of the tag (I.E. "tag_torso")
	char strName[64];

	// This stores the translation that should be performed
	CVector3 vPosition;

	// This stores the 3x3 rotation matrix for this frame
	float rotation[3][3];
} TMd3Tag;

// bone information
typedef struct
{
	// This is the min (x, y, z) value for the bone
	float mins[3];

	// This is the max (x, y, z) value for the bone
	float maxs[3];

	// This stores the bone position
	float position[3];

	// This stores the scale of the bone
	float scale;

	// The modeler used to create the model (I.E. "3DS Max")
	char creator[16];
} TMd3Bone;

// this stores the normals and vertex indices 
typedef struct
{
	// The vertex for this face
   signed short vertex[3];

   // This stores some crazy normal values
   unsigned char normal[2];
} TMd3Triangle;

// This stores the indices into the vertex and texture coordinate arrays
typedef struct
{
   int vertexIndices[3];				
} TMd3Face;

// UV coordinates
typedef struct
{
   float textureCoord[2];
} TMd3TexCoord;

// skin name
typedef struct
{
	char strName[68];
} TMd3Skin;

//////////////////////////////////////////////////////////////////////////
// C3DModel class
//////////////////////////////////////////////////////////////////////////

C3DModel::C3DModel()
{
}

C3DModel::~C3DModel()
{
	// just in case we didnt free it before
	Free();
}

//////////////////////////////////////////////////////////////////////////
// Load() : Loads a MD3 model & textures
//////////////////////////////////////////////////////////////////////////

bool C3DModel::Load(const std::string & strFilename)
{
	CFile model;
	TMd3Header header;

	// free old data
	Free();

	// Open the MD3 file in binary
	model.Open(strFilename, FILE_AUTO, FILE_BINARY);
	if(!model.IsOpen()) return false;

	// read the header data
	model.Read(&header, sizeof(TMd3Header));

	// check header
	if(!CheckMD3(header))
	{
		// bye bye
		Free();
		return false;
	}

	// load bones
	if(!LoadBones(header, &model))
	{
		// bye bye
		Free();
		return false;
	}

	// load tags
	if(!LoadTags(header, &model))
	{
		// bye bye
		Free();
		return false;
	}

	// load objects
	if(!LoadObjects(header, &model))
	{
		// bye bye
		Free();
		return false;
	}

	// close file
	model.Close();

	// return
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CheckMD3() : Checks file signature
//////////////////////////////////////////////////////////////////////////

inline bool C3DModel::CheckMD3(const TMd3Header & header)
{
	// make sure the ID == IDP3 and the version
	// is this crazy number '15' or else it's a bad egg
	if((header.fileID[0] != 'I'  ||
		header.fileID[1] != 'D'  ||
		header.fileID[2] != 'P'  ||
		header.fileID[3] != '3') ||
		header.version != 15)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
// LoadBones() : Loads the model's bones
//////////////////////////////////////////////////////////////////////////

inline bool C3DModel::LoadBones(const TMd3Header & header, CFile *model)
{
	TMd3Bone *bones;

	// allocate bones
	bones = new TMd3Bone[header.numFrames];
	if(!bones) return false;

	// read the bones
	model->Read(bones, sizeof(TMd3Bone));

	// convert bones
	// no conversion, we don't store them

	// free bones
	delete [] bones;

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// LoadTags() : Loads the model's tags
//////////////////////////////////////////////////////////////////////////

inline bool C3DModel::LoadTags(const TMd3Header & header, CFile *model)
{
	TTag tag;
	TMd3Tag	md3_tag;

	// convert tags
	for(int i = 0; i < header.numFrames * header.numTags; ++i)
	{
		// read tags
		model->Read(&md3_tag, sizeof(TMd3Tag));

		// convert
		tag.strName = md3_tag.strName;
		tag.vPosition = md3_tag.vPosition;
		memcpy(&tag.rotation, md3_tag.rotation, sizeof(float) * 9);

		// push tag
		m_Tags.push_back(tag);
	}

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// LoadObjects() : Loads the model's tags
//////////////////////////////////////////////////////////////////////////

inline bool C3DModel::LoadObjects(const TMd3Header & header, CFile *model)
{
	int i;
	TMd3MeshInfo mesh_header;
	unsigned long mesh_offset;

	// get the current offset in the file
	mesh_offset = model->Tell();

	// go through all the meshes
	for(i = 0; i < header.numMeshes; ++i)
	{
		TObject object;

		// Seek to the start of this mesh and read in it's header
		model->Seek(mesh_offset, SEEK_SET);
		model->Read(&mesh_header, sizeof(TMd3MeshInfo));

		// Assign name
		object.strName = mesh_header.strName;

		// Read in the skin information
		for(i = 0; i < mesh_header.numSkins; ++i)
		{
			TMd3Skin skin;

			// read the skin
			model->Read(&skin, sizeof(TMd3Skin));

			// push the skin
			// whatever
		}
		
		// Seek to the start of the triangle/face data
		model->Seek(mesh_offset + mesh_header.triStart, SEEK_SET);

		// ...then read it in
		for(i = 0; i < mesh_header.numTriangles; ++i)
		{
			TFace face;
			TMd3Face triangle;

			// read face
			model->Read(&triangle, sizeof(TMd3Face));

			// transform face
			face.vertIndex[0] = triangle.vertexIndices[0];
			face.vertIndex[1] = triangle.vertexIndices[1];
			face.vertIndex[2] = triangle.vertexIndices[2];

			// push it
			object.faces.push_back(face);
		}

		// Seek to the start of the UV coordinate data
		model->Seek(mesh_offset + mesh_header.uvStart, SEEK_SET);

		// ...then read it in
		for(i = 0; i < mesh_header.numVertices; ++i)
		{
			CVector2 coord;
			TMd3TexCoord texCoord;

			// read the texture coordinate
			model->Read(&texCoord, sizeof(TMd3TexCoord));

			// convert the coord
			coord.x = texCoord.textureCoord[0];
			coord.y = texCoord.textureCoord[1];

			// push the coord
			object.texVerts.push_back(coord);
		}

		// Seek to the start of the vertex/face index information
		model->Seek(mesh_offset + mesh_header.vertexStart, SEEK_SET);

		// ...then read it in.
		for(i = 0; i < mesh_header.numMeshFrames * mesh_header.numVertices; ++i)
		{
			CVector3 vertex;
			TMd3Triangle triangle;

			// read the vertex
			model->Read(&triangle, sizeof(TMd3Triangle));

			// convert
			vertex.x = (float) triangle.vertex[0] / 128.0f;
			vertex.y = (float) triangle.vertex[1] / 128.0f;
			vertex.z = (float) triangle.vertex[2] / 128.0f;

			// push
			object.verts.push_back(vertex);
		}

		// push object
		m_Objects.push_back(object);

		// Increase the offset into the file
		mesh_offset += mesh_header.meshSize;
	}

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Free() : Destroy all data
//////////////////////////////////////////////////////////////////////////

void C3DModel::Free()
{
	// free containters
	m_Materials.clear();
	m_Objects.clear();
	m_Animations.clear();
	m_Tags.clear();
}

//////////////////////////////////////////////////////////////////////////
// Render() : Renders the specified model animation at frame "fFrame"
//////////////////////////////////////////////////////////////////////////

void C3DModel::Render(int iAnimationIndex, float fFrame)
{
	// do we have any object?
	if(m_Objects.size() <= 0) return;

	// Rotate the model to compensate for the z up orientation
	// that the model was saved
	glRotatef(-90, 1, 0, 0);

	// Go through all of the objects stored in this model
	for(int i = 0; i < m_Objects.size(); ++i)
	{
		// reference to the current object
		TObject & actual_object = m_Objects[i];

		// Find the current starting index for the current key frame we are on
		int currentIndex = (int) fFrame * actual_object.verts.size();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		glBegin(GL_TRIANGLES);

		// Go through all of the faces (polygons) of the object and draw them
		for(int j = 0; j < actual_object.faces.size(); ++j)
		{
			// Go through each vertex of the triangle and draw it.
			for(int whichVertex = 0; whichVertex < 3; ++whichVertex)
			{
				// Get the index for the current point in the face list
				int index = actual_object.faces[j].vertIndex[whichVertex];
							
				// Make sure there is texture coordinates for this (%99.9 likelyhood)
				if(actual_object.texVerts.size() > 0)
				{
					// Assign the texture coordinate to this vertex
					glTexCoord2f(actual_object.texVerts[index].x, 
								 actual_object.texVerts[index].y);

					// Pass the vertices
					glVertex3f(actual_object.verts[index].x,
							   actual_object.verts[index].y,
							   actual_object.verts[index].z);
				}
			}
		}

		glEnd();

		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

//////////////////////////////////////////////////////////////////////////
// ReloadTextures() : Loads the model textures
//////////////////////////////////////////////////////////////////////////

void C3DModel::ReloadTextures()
{
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
