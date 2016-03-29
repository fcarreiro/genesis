#ifndef __ENGINE_H__
#define __ENGINE_H__

//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////

#define CLIENT_VERSION			"0.1 alpha"
#define CLIENT_VERSION_UPPER	0
#define CLIENT_VERSION_LOWER	1

//////////////////////////////////////////////////////////////////////////
// Window enumerations
//////////////////////////////////////////////////////////////////////////

enum {
	ID_CONNECT_USERNAME = 1,
	ID_CONNECT_PASSWORD,
	ID_CREATE_USERNAME,
	ID_CREATE_PASSWORD,
	ID_CREATE_MAIL,
	ID_DELETE_USERNAME,
	ID_DELETE_PASSWORD
};

//////////////////////////////////////////////////////////////////////////
// Error enumerations
//////////////////////////////////////////////////////////////////////////

enum {
	ERR_OK,
	ERR_OUTOFMEM,
	ERR_SETUP,
	ERR_UNKNOWN
};

//////////////////////////////////////////////////////////////////////////
// CEngine class
//////////////////////////////////////////////////////////////////////////

class CEngine
{
public:
	// constructor & destructor
	CEngine();
	~CEngine();

	// starts the engine execution
	int Run();

	// stops the engine after one frame
	void Stop() { m_bRunning = false; }

	// frees the engine
	int Close();

	// prepares the engine
	int Startup(const std::string & strTitle);

private:
	// functions
	void InitialLoad();
	int  Setup();
	void Tick();

	// options
	bool LoadOptions();
	bool SaveOptions();

private:
	// states
	static void to_MainMenu();
	static void to_ConnectMenu();
	static void to_CreatePlayerMenu();
	static void to_DeletePlayerMenu();
	static void to_OptionsMenu();
	static void to_CreditsMenu();
	static void to_Playing();

	// menu functions
	static void menu_Connect();
	static void menu_Disconnect();
	static void menu_Create();

private:
	// holds the main window's title
	std::string				m_strWindowTitle;

	// state
	bool					m_bRunning;

	// holds screen info
	unsigned int			m_uiScreenWidth;
	unsigned int			m_uiScreenHeight;
	unsigned int			m_uiScreenDepth;

	// managers
	ext::manager<CTexture>	*m_TextureMgr;
	ext::manager<CFontBase>	*m_FontMgr;
	ext::manager<COption>	*m_Options;

	// modules
	COpenGL					*m_OpenGL;
	CInputSystem			*m_Input;
	CWindowSystem			*m_Winsys;
	CNetworkSystem			*m_Network;
	CFrustum				*m_Frustum;
	CMap					*m_Map;
	CPlayer					*m_Player;
	CConsole				*m_Console;
	CLanguagePack			*m_Language;
};

//////////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////////

extern ext::manager<CTexture>	*g_TextureMgr;
extern ext::manager<CFontBase>	*g_FontMgr;
extern ext::manager<COption>	*g_Options;

extern CEngine					*g_Engine;
extern COpenGL					*g_OpenGL;
extern CInputSystem				*g_Input;
extern CWindowSystem			*g_Winsys;
extern CNetworkSystem			*g_Network;
extern CFrustum					*g_Frustum;
extern CMap						*g_Map;
extern CPlayer					*g_Player;
extern CConsole					*g_Console;
extern CLanguagePack			*g_Language;

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif