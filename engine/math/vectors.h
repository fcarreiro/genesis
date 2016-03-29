#ifndef __VECTORS_H__
#define __VECTORS_H__

//////////////////////////////////////////////////////////////////////////
// CVector2 class (x, y)
//////////////////////////////////////////////////////////////////////////

class CVector2
{
public:
	// default constructor
	CVector2() : x(0.0f), y(0.0f) { }

	// overloaded constructors
	CVector2(float X, float Y) : x(X), y(Y) { }

public:
	// operator +
	CVector2 operator+(const CVector2 & vVector) const
	{
		// return the added vectors result
		return CVector2(x + vVector.x, y + vVector.y);
	}

	// operator +
	CVector2 operator+(const float f) const
	{
		// return the added vectors result
		return CVector2(x + f, y + f);
	}

	// operator +=
	void operator+=(const CVector2 & vVector)
	{
		// add the vectors
		x += vVector.x;
		y += vVector.y;
	}

	// operator +=
	void operator+=(const float f)
	{
		// add the vectors
		x += f;
		y += f;
	}

	// operator -
	CVector2 operator-(const CVector2 & vVector) const
	{
		// return the subtracted vectors result
		return CVector2(x - vVector.x, y - vVector.y);
	}
	
	// operator -
	CVector2 operator-(const float f) const
	{
		// return the subtracted vectors result
		return CVector2(x - f, y - f);
	}

	// operator -=
	void operator-=(const CVector2 & vVector)
	{
		// substract the vectors
		x -= vVector.x;
		y -= vVector.y;
	}

	// operator -=
	void operator-=(const float f)
	{
		// substract the vectors
		x -= f;
		y -= f;
	}

	// operator *
	CVector2 operator*(const float num) const
	{
		// return the scaled vector
		return CVector2(x * num, y * num);
	}

	// operator *=
	void operator*=(const float num)
	{
		// scale the vector
		x *= num;
		y *= num;
	}

	// operator /
	CVector2 operator/(const float num) const
	{
		// return the scale vector
		return CVector2(x / num, y / num);
	}

	// operator /=
	void operator/=(const float num)
	{
		// scale the vector
		x /= num;
		y /= num;
	}

public:
	// variables
	float x, y;
};

//////////////////////////////////////////////////////////////////////////
// CVector3 class (x, y, z)
//////////////////////////////////////////////////////////////////////////

class CVector3
{
public:
	// default constructor
	CVector3() : x(0.0f), y(0.0f), z(0.0f) { }

	// overloaded constructors
	CVector3(float X, float Y, float Z) : x(X), y(Y), z(Z) { }

public:
	// operator +
	CVector3 operator+(const CVector3 & vVector) const
	{
		// return the added vectors result
		return CVector3(x + vVector.x, y + vVector.y, z + vVector.z);
	}

	// operator +
	CVector3 operator+(const float f) const
	{
		// return the added vectors result
		return CVector3(x + f, y + f, z + f);
	}

	// operator +=
	void operator+=(const CVector3 & vVector)
	{
		// add the vector
		x += vVector.x;
		y += vVector.y;
		z += vVector.z;
	}

	// operator +=
	void operator+=(const float f)
	{
		// add the vector
		x += f;
		y += f;
		z += f;
	}

	// operator -
	CVector3 operator-(const CVector3 & vVector) const
	{
		// return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	// operator -
	CVector3 operator-(const float f) const
	{
		// return the subtracted vectors result
		return CVector3(x - f, y - f, z - f);
	}

	// operator -=
	void operator-=(const CVector3 & vVector)
	{
		// substract vector
		x -= vVector.x;
		y -= vVector.y;
		z -= vVector.z;
	}

	// operator -=
	void operator-=(const float f)
	{
		// substract vector
		x -= f;
		y -= f;
		z -= f;
	}

	// operator * (dot)
	CVector3 operator*(const CVector3 & vVector) const
	{
		// return the dot product between vectors
		return CVector3(x * vVector.x, y * vVector.y, z * vVector.z);
	}

	// operator *= (dot)
	void operator*=(const CVector3 & vVector)
	{
		// do the dot product
		x *= vVector.x;
		y *= vVector.y;
		z *= vVector.z;
	}

	// operator * (scalar)
	CVector3 operator*(const float num) const
	{
		// return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}

	// operator *= (scalar)
	void operator*=(const float num)
	{
		// scale the vector
		x *= num;
		y *= num;
		z *= num;
	}

	// operator / (scalar)
	CVector3 operator/(const float num) const
	{
		// return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	// operator /= (scalar)
	void operator/=(const float num)
	{
		// scale the vector
		x /= num;
		y /= num;
		z /= num;
	}

public:
	// variables
	float x, y, z;						
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
