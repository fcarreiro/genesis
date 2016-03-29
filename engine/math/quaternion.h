#ifndef __QUATERNION_H__
#define __QUATERNION_H__

//////////////////////////////////////////////////////////////////////////
// CQuaternion class (x, y, z, w)
//////////////////////////////////////////////////////////////////////////

class CQuaternion
{
public:
	// default constructor
	CQuaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }

	// overloaded constructor
	CQuaternion(float X, float Y, float Z, float W) :
		x(X), y(Y), z(Z), w(W) { }

public:
	// operator +
	CQuaternion operator+(const CQuaternion & q) const
	{
		return CQuaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	// operator +=
	void operator+=(const CQuaternion & q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
	}

	// operator -
	CQuaternion operator-(const CQuaternion & q) const
	{
		return CQuaternion(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	// operator -=
	void operator-=(const CQuaternion & q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
	}

	// operator * (scalar)
	CQuaternion operator*(const float num) const
	{
		return CQuaternion(x * num, y * num, z * num, w * num);
	}

	// operator *= (scalar)
	void operator*(const float num)
	{
		x *= num;
		y *= num;
		z *= num;
		w *= num;
	}

	// operator / (scalar)
	CQuaternion operator/(const float num) const
	{
		return CQuaternion(x / num, y / num, z / num, w / num);
	}

	// operator /= (scalar)
	void operator/(const float num)
	{
		x /= num;
		y /= num;
		z /= num;
		w /= num;
	}

public:
	// variables
	float x, y, z, w;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
