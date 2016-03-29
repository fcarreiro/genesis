#ifndef __MODEL_H__
#define __MODEL_H__

//////////////////////////////////////////////////////////////////////////
// C3DModel class
//////////////////////////////////////////////////////////////////////////

class C3DModel
{
public:
	// constructor & destructor
	C3DModel();
	~C3DModel();

	// loads a model given the filename
	bool Load(const std::string & strFilename);

	// unloads the model
	void Free();

	// draws the model
	void Render(int iAnimationIndex, float fFrame);

	// reloads the model's textures
	void ReloadTextures();

private:
	// header information
	typedef struct
	{ 
		// This stores the file ID - Must be "IDP3"
		char fileID[4];

		// This stores the file version - Must be 15
		int version;

		// This stores the name of the file
		char strFile[68];

		// This stores the number of animation frames
		int numFrames;

		// This stores the tag count
		int numTags;

		// This stores the number of sub-objects in the mesh
		int numMeshes;

		// This stores the number of skins for the mesh
		int numMaxSkins;

		// This stores the mesh header size
		int headerSize;

		// This stores the offset into the file for tags
		int tagStart;

		// This stores the end offset into the file for tags
		int tagEnd;

		// This stores the file size
		int fileSize;
	} TMd3Header;

	// face structure
	struct TFace
	{
		int	vertIndex[3];
	};

	// material structure
	struct TMaterial
	{
		// The texture name
		std::string strName;

		// The texture file name (If this is set it's a texture map)
		std::string strFile;

		// The color of the object (R, G, B)
		unsigned char ubColor[3];

		// The texture itself
		CTexture texTexture;

		// UV tiling
		CVector2 vTiling;

		// UV offset
		CVector2 vOffset;
	};

	// object structure
	struct TObject 
	{
		// The texture ID to use, which is the index into our texture array
		int materialID;

		// This is TRUE if there is a texture map for this object
		bool hasTexture;

		// The name of the object
		std::string strName;

		// The object's vertices
		std::vector<CVector3> verts;

		// The object's normals
		std::vector<CVector3> normals;

		// The texture's UV coordinates
		std::vector<CVector2> texVerts;

		// The faces information of the object
		std::vector<TFace> faces;
	};

	// animation structure
	struct TAnimation
	{
		// This stores the name of the animation (I.E. "TORSO_STAND")
		std::string strName;

		// This stores the first frame number for this animation
		int startFrame;

		// This stores the last frame number for this animation
		int endFrame;

		// This stores the time in milliseconds that this animation lasts
		unsigned long timeFrame;
	};

	// tag structure
	struct TTag
	{
		// This stores the name of the tag (I.E. "tag_torso")
		std::string strName;

		// This stores the translation that should be performed
		CVector3 vPosition;

		// This stores the 3x3 rotation matrix for this frame
		float rotation[3][3];
	};

private:
	// checks the file signature
	bool CheckMD3(const TMd3Header & header);

	// loads the bones
	bool LoadBones(const TMd3Header & header, CFile *model);

	// loads the tags
	bool LoadTags(const TMd3Header & header, CFile *model);

	// loads the objects
	bool LoadObjects(const TMd3Header & header, CFile *model);

private:
	// The list of materials
	std::vector<TMaterial> m_Materials;

	// The object list for our model
	std::vector<TObject> m_Objects;

	// The list of animations
	std::vector<TAnimation> m_Animations;

	// This stores all the tags for the model animations
	std::vector<TTag> m_Tags;
};

////////////////////////////////////////////////////////////////////////
// End
////////////////////////////////////////////////////////////////////////

#endif
