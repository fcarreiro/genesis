#ifndef __NPCPLAYER_H__
#define __NPCPLAYER_H__

//////////////////////////////////////////////////////////////////////////
// CNPCPlayer class
//////////////////////////////////////////////////////////////////////////

class CNPCPlayer : public CPlayer
{
public:
	// constructor & destructor
	CNPCPlayer();
	virtual ~CNPCPlayer();

	// creates a player
	virtual bool Create(const std::string & strName,
		TQualities *pQualities);

	// destroys the player
	virtual void Destroy();

	// updates the player info
	virtual void Update();

	// draws the player
	virtual void Render() const;

	// moves the player foreward or backwards
	virtual void Move(float speed);

	// moves the player to the right or left
	virtual void Strafe(float speed);

	// rotates the player over its own axis
	virtual void Rotate(float x, float y, float z);
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
