#ifndef __ACTOR_H__
#define __ACTOR_H__

//////////////////////////////////////////////////////////////////////////
// CActor class
//////////////////////////////////////////////////////////////////////////

class CActor
{
public:
	// constructor & destructor
	CActor();
	virtual ~CActor();

	// moves the actor depending on the speed
	void Move(float fSpeed);

	// stafe the actor to left (-) or right (+)
	void Strafe(float fSpeed);

	// rotate in the three angles
	void Rotate(float x, float y, float z);

	// set actor's position
	void SetPosition(CVector3 & vPos) { m_vPosition = vPos; }
	
	// set actor's position
	void SetPosition(float x, float y, float z) { m_vPosition = CVector3(x, y, z); }

	// get actor's position
	CVector3 & GetPosition() { return m_vPosition; }

protected:
	// holds actor position
	CVector3 m_vPosition;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
