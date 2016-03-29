#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// CHumanPlayer default contructor & destructor
//////////////////////////////////////////////////////////////////////////

CHumanPlayer::CHumanPlayer()
{
	// variables
	m_dwID = 0;
	m_bAlive = false;

	// camera
	m_Camera = NULL;

	// models
	m_Head = NULL;
	m_Body = NULL;
	m_Legs = NULL;

	// qualities
	memset(&m_Qualities, 0, sizeof(m_Qualities));
}

CHumanPlayer::~CHumanPlayer()
{
	// destroy the human player
	CHumanPlayer::Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates a player and sets its properties
//////////////////////////////////////////////////////////////////////////

bool CHumanPlayer::Create(const std::string & strName,
						  TQualities *pQualities)
{
	// error checking
	if(!pQualities) return false;

	// free old data
	Destroy();

	// create camera
	m_Camera = new CCamera();
	if(!m_Camera) return false;

	// set camera position
	m_Camera->SetPosition(0, 0, 0, 0, 0, -15, 0, 1, 0);

	// set local name
	m_strName = strName;

	// set local id
	//m_dwID = x86_string_crc(m_strName.c_str());

	// set qualities
	memcpy(&m_Qualities, pQualities, sizeof(m_Qualities));

	m_Head = new C3DModel;
	m_Body = new C3DModel;
	m_Legs = new C3DModel;

	m_Head->Load("models/head.md3");
//	m_Body->Load("models/upper.md3");
//	m_Legs->Load("models/lower.md3");

	// its alive!
	m_bAlive = true;

	// sucess
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Eliminates all player data from memory
//////////////////////////////////////////////////////////////////////////

void CHumanPlayer::Destroy()
{
	// destroy camera
	delete m_Camera;
	m_Camera = NULL;

	// destroy head model
	delete m_Head;
	m_Head = NULL;

	// destroy body model
	delete m_Body;
	m_Body = NULL;

	// destroy legs model
	delete m_Legs;
	m_Legs = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Update() : Controls player
//////////////////////////////////////////////////////////////////////////

void CHumanPlayer::Update()
{
	int x, y;

	g_Input->GetMouseMotion(&x, &y);

	if(ext::get_option_int("mouse_invert_x")) x = -x;
	if(!ext::get_option_int("mouse_invert_y")) y = -y;

	m_Camera->m_vView.y += (y / 20.0f);
	m_Camera->Rotate(0, x / 500.0f, 0);
}

//////////////////////////////////////////////////////////////////////////
// Render() : Renders player model & derivates
//////////////////////////////////////////////////////////////////////////

void CHumanPlayer::Render() const
{
	glTranslatef(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glColor3ub(255,0,0);

//	if(m_Head) m_Head->Render(0, 0.0f);
//	if(m_Body) m_Body->Render(0, 0.0f);
//	if(m_Legs) m_Legs->Render(0, 0.0f);
}

//////////////////////////////////////////////////////////////////////////
// Move() : Moves the player (foreward, backward)
//////////////////////////////////////////////////////////////////////////

void CHumanPlayer::Move(float speed)
{
	m_Camera->Move(speed);
	m_vPosition = m_Camera->m_vPos;
}

//////////////////////////////////////////////////////////////////////////
// Strafe() : Moves the player (left, right)
//////////////////////////////////////////////////////////////////////////

void CHumanPlayer::Strafe(float speed)
{
	m_Camera->Strafe(speed);
	m_vPosition = m_Camera->m_vPos;
}

//////////////////////////////////////////////////////////////////////////
// Rotate() : Rotates the player over its own axis
//////////////////////////////////////////////////////////////////////////

void CHumanPlayer::Rotate(float x, float y, float z)
{
	m_Camera->Rotate(x, y, z);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
