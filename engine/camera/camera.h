#ifndef __CAMERA_H__
#define __CAMERA_H__

//////////////////////////////////////////////////////////////////////////
// Camera person constants
//////////////////////////////////////////////////////////////////////////

enum
{
	CAMERA_FIRST_PERSON,
	CAMERA_THIRD_PERSON
};

//////////////////////////////////////////////////////////////////////////
// CCamera class
//////////////////////////////////////////////////////////////////////////

class CCamera
{
public:
	// look through cmera
	void Look(unsigned char mode);

	// set camera position
	void SetPosition(float pos_x,  float pos_y,	 float pos_z,
			 		 float view_x, float view_y, float view_z,
					 float up_x,   float up_y,   float up_z);

	// strafe the camera
	void Strafe(float speed);

	// move the camera foreward or backward
	void Move(float speed);

	// rotate the camera on its own axis
	void Rotate(float x, float y, float z);

	// rotate the camera on a specific point
	void Rotate_Around_Point(CVector3 & vCenter, float x, float y, float z);

public:
	// hold position vectors
	CVector3 m_vPos;
	CVector3 m_vView;
	CVector3 m_vUp;

private:
	// time-based movement timer
	CTimer m_Timer;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif __CAMERA_H__
