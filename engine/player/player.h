#ifndef __PLAYER_H__
#define __PLAYER_H__

//////////////////////////////////////////////////////////////////////////
// Objects
//////////////////////////////////////////////////////////////////////////

enum {
	OBJ_NOTHING,
	MAX_OBJECTS
};

//////////////////////////////////////////////////////////////////////////
// Qualities
//////////////////////////////////////////////////////////////////////////

typedef struct
{
	// physic info
	unsigned char ucRace;
	unsigned char ucGender;

	// status info
	int	iHP;
	int	iMaxHP;
	int	iMana;
	int	iMaxMana;
	int	iExperience;
	int	iLevel;
	int	iEnergy;
	
	// base info
	char cStrength;
	char cInteligence;
	char cAgility;
	char cVisibility;
	
	// extra info
	char cMagic;
	char cShortRangeWeapons;
	char cLongRangeWeapons;
	char cMagicResistance;
	char cPhisicResistance;

	// job info
	unsigned char ucLumber;
	unsigned char ucFish;
	unsigned char ucCommerce;
	unsigned char ucCook;
	unsigned char ucSail;
	unsigned char ucBlacksmith;
	unsigned char ucTame;
	unsigned char ucMine;
	unsigned char ucAlchemist;
	unsigned char ucConstructor;
	unsigned char ucCarpenter;
	unsigned char ucTailor;
} TQualities;

//////////////////////////////////////////////////////////////////////////
// CPlayer class
//////////////////////////////////////////////////////////////////////////

class CPlayer : public CActor
{
public:
	// constructor & destructor
	CPlayer();
	virtual ~CPlayer();

	// returns true if the player is alive
	bool IsAlive() const { return m_bAlive; }

	// looks through the player's eyes
	void Look() { if(m_Camera) m_Camera->Look(CAMERA_FIRST_PERSON); }

	// creates a player
	virtual bool Create(const std::string & strName,
		TQualities *pQualities) = 0;

	// destroys the player
	virtual void Destroy();

	// updates the player info
	virtual void Update() = 0;

	// draws the player
	virtual void Render() const = 0;

	// moves the player foreward or backwards
	virtual void Move(float speed) = 0;

	// moves the player to the right or left
	virtual void Strafe(float speed) = 0;

	// rotates the player over its own axis
	virtual void Rotate(float x, float y, float z) = 0;

protected:
	// is he/she alive?
	bool m_bAlive;

	// this is this players ID number
	unsigned long m_dwID;

	// holds this player's name
	std::string m_strName;

	// player qualities
	TQualities m_Qualities;

	// my camera
	CCamera *m_Camera;

	// models
	C3DModel *m_Head;
	C3DModel *m_Body;
	C3DModel *m_Legs;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
