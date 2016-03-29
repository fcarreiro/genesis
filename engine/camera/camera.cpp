#include "../precompiled/stdafx.h"
#include "../math/vectors.h"
#include "../math/3dmath.h"
#include "../../common/utility/timer.h"
#include "camera.h"

//////////////////////////////////////////////////////////////////////////
// Look() : Look through camera
//////////////////////////////////////////////////////////////////////////

void CCamera::Look(unsigned char mode)
{
	// switch mode
	if(mode == CAMERA_FIRST_PERSON)
	{
		// look first person
		gluLookAt(m_vPos.x,  m_vPos.y,  m_vPos.z,	
				  m_vView.x, m_vView.y, m_vView.z,	
				  m_vUp.x,   m_vUp.y,   m_vUp.z);
	}
	else
	{
		// calculate eye's position
		CVector3 vVector = m_vPos - (m_vView - m_vPos);

		// look third person
		gluLookAt(vVector.x, vVector.y, vVector.z,	
				  m_vPos.x,  m_vPos.y,  m_vPos.z,	
				  m_vUp.x,   m_vUp.y,   m_vUp.z);
	}

	// update timer
	m_Timer.Update();
}


//////////////////////////////////////////////////////////////////////////
// SetPosition() : Sets the camera position
//////////////////////////////////////////////////////////////////////////

void CCamera::SetPosition(float pos_x,  float pos_y,  float pos_z,
						  float view_x, float view_y, float view_z,
						  float up_x,   float up_y,   float up_z)
{
	// set the position vector
	m_vPos = CVector3(pos_x, pos_y, pos_z);

	// set the view vector
	m_vView = CVector3(view_x, view_y, view_z);

	// set the up vector
	m_vUp = CVector3(up_x, up_y, up_z);
}


//////////////////////////////////////////////////////////////////////////
// Move() : Moves the camera foreward or backward
//////////////////////////////////////////////////////////////////////////

void CCamera::Move(float speed)
{
	CVector3 vVector;
	vVector = Normalize(m_vView - m_vPos);

	// time based movement
	speed = speed * ((float)m_Timer.GetTimePassed() / 1000.0f);

	// forward positive speed and backward negative -speed.
	m_vPos  = m_vPos  + vVector * speed;
	m_vView = m_vView + vVector * speed;
}

//////////////////////////////////////////////////////////////////////////
// Strafe() : Moves the camera left or right
//////////////////////////////////////////////////////////////////////////

void CCamera::Strafe(float speed)
{
	CVector3 vCross = Cross(m_vView - m_vPos, m_vUp);
	CVector3 vStrafe = Normalize(vCross);

	// time based movement
	speed = speed * ((float)m_Timer.GetTimePassed() / 1000);

	// Add the strafe vector to our position
	m_vPos.x += vStrafe.x * speed;
	m_vPos.z += vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += vStrafe.x * speed;
	m_vView.z += vStrafe.z * speed;
}

//////////////////////////////////////////////////////////////////////////
// Rotate() : Rotates the camera over its own axis
//////////////////////////////////////////////////////////////////////////

void CCamera::Rotate(float x, float y, float z)
{
	// get the view vector
	CVector3 vVector = m_vView - m_vPos;

	// rotate x
	if(x) 
	{
		m_vView.z = (float)(m_vPos.z + sin(x) * vVector.y + cos(x) * vVector.z);
		m_vView.y = (float)(m_vPos.y + cos(x) * vVector.y - sin(x) * vVector.z);
	}

	// rotate y
	if(y) 
	{
		m_vView.z = (float)(m_vPos.z + sin(y) * vVector.x + cos(y) * vVector.z);
		m_vView.x = (float)(m_vPos.x + cos(y) * vVector.x - sin(y) * vVector.z);
	}

	// rotate z
	if(z) 
	{
		m_vView.x = (float)(m_vPos.x + sin(z) * vVector.y + cos(z) * vVector.x);		
		m_vView.y = (float)(m_vPos.y + cos(z) * vVector.y - sin(z) * vVector.x);
	}
}

//////////////////////////////////////////////////////////////////////////
// Rotate_Around_Point() : Rotates the camera around an specific point
//////////////////////////////////////////////////////////////////////////

void CCamera::Rotate_Around_Point(CVector3 & vCenter, float x, float y, float z)
{
	// get the center
	CVector3 vVector = m_vPos - vCenter;

	// rotate x
	if(x)
	{
		m_vPos.z = (float)(vCenter.z + sin(x) * vVector.y + cos(x) * vVector.z);
		m_vPos.y = (float)(vCenter.y + cos(x) * vVector.y - sin(x) * vVector.z);
	}

	// rotate y
	if(y) 
	{
		m_vPos.z = (float)(vCenter.z + sin(y) * vVector.x + cos(y) * vVector.z);
		m_vPos.x = (float)(vCenter.x + cos(y) * vVector.x - sin(y) * vVector.z);
	}

	// rotate z
	if(z)
	{
		m_vPos.x = (float)(vCenter.x + sin(z) * vVector.y + cos(z) * vVector.x);		
		m_vPos.y = (float)(vCenter.y + cos(z) * vVector.y - sin(z) * vVector.x);
	}
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
