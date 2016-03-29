#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

//////////////////////////////////////////////////////////////////////////
// Frustum sides enumeration
//////////////////////////////////////////////////////////////////////////

enum frustum_side_t
{
	RIGHT,
	LEFT,
	BOTTOM,
	TOP,
	BACK,
	FRONT
}; 

//////////////////////////////////////////////////////////////////////////
// Plane data enumeration
//////////////////////////////////////////////////////////////////////////

enum plane_data_t
{
	A,	// The X value of the plane's normal
	B,	// The Y value of the plane's normal
	C,	// The Z value of the plane's normal
	D	// The distance the plane is from the origin
};

//////////////////////////////////////////////////////////////////////////
// CFrustum class
//////////////////////////////////////////////////////////////////////////

class CFrustum
{
public:
	// Call this every time the camera moves to update the frustum
	void CalculateFrustum();

	// This takes a 3D point and returns TRUE if it's inside of the frustum
	bool PointInFrustum(float x, float y, float z);

	// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
	bool SphereInFrustum(float x, float y, float z, float radius);

	// This takes the center and half the length of the cube.
	bool CubeInFrustum(float x, float y, float z, float size);

	// My function - Takes center, width, height, depth
	bool BoxInFrustum(float x, float y, float z, float width, float height, float depth);

private:
	// This holds the A B C and D values for each side of our frustum.
	float m_Frustum[6][4];
};

//////////////////////////////////////////////////////////////////////////
// NormalizePlane() : This normalizes a plane from a given frustum.
//////////////////////////////////////////////////////////////////////////

inline void NormalizePlane(float frustum[6][4], unsigned char side)
{
	// Here we calculate the magnitude of the normal to the plane (point A B C)
	// Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
	// To calculate magnitude you use the equation:  magnitude = sqrt(x^2 + y^2 + z^2)
	float magnitude = (float)sqrt( frustum[side][A] * frustum[side][A] + 
								   frustum[side][B] * frustum[side][B] + 
								   frustum[side][C] * frustum[side][C] );

	// Then we divide the plane's values by it's magnitude.
	// This makes it easier to work with.
	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude; 
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
