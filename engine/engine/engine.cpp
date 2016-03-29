#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// Global debug options
//////////////////////////////////////////////////////////////////////////

#define FULLSCREEN_FLAG true
CTexture *mm = new CTexture();

//////////////////////////////////////////////////////////////////////////
// Global engine and modules variables
//////////////////////////////////////////////////////////////////////////

CEngine					*g_Engine = NULL;
COpenGL					*g_OpenGL = NULL;
CInputSystem			*g_Input = NULL;
CWindowSystem			*g_Winsys = NULL;
CNetworkSystem			*g_Network = NULL;
CFrustum				*g_Frustum = NULL;
CMap					*g_Map = NULL;
CPlayer					*g_Player = NULL;
CConsole				*g_Console = NULL;
CLanguagePack			*g_Language = NULL;

ext::manager<CTexture>	*g_TextureMgr = NULL;
ext::manager<CFontBase>	*g_FontMgr = NULL;
ext::manager<COption>	*g_Options = NULL;

//////////////////////////////////////////////////////////////////////////
// CEngine default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CEngine::CEngine()
{
	// set global variables
	g_Engine = this;

	// state
	m_bRunning = false;

	// screen variables
	m_uiScreenWidth = 0;
	m_uiScreenHeight = 0;
	m_uiScreenDepth = 0;

	// modules
	m_OpenGL = NULL;
	m_Console = NULL;
	m_Input = NULL;
	m_Frustum = NULL;
	m_Map = NULL;
	m_Network = NULL;
	m_Winsys = NULL;
	m_Player = NULL;
	m_Language = NULL;

	// managers
	m_TextureMgr = NULL;
	m_FontMgr = NULL;
	m_Options = NULL;
}

CEngine::~CEngine()
{
	// free
	Close();
}

//////////////////////////////////////////////////////////////////////////
// Startup() : Prepares the engine to run
//////////////////////////////////////////////////////////////////////////

int CEngine::Startup(const std::string & strTitle)
{
	// sets up the texture manager
	m_TextureMgr = new ext::manager<CTexture>;
	if(!m_TextureMgr) return ERR_OUTOFMEM;
	g_TextureMgr = m_TextureMgr;

	// sets up the font manager
	m_FontMgr = new ext::manager<CFontBase>;
	if(!m_FontMgr) return ERR_OUTOFMEM;
	g_FontMgr = m_FontMgr;

	// sets up the options manager
	m_Options = new ext::manager<COption>;
	if(!m_Options) return ERR_OUTOFMEM;
	g_Options = m_Options;

	// sets up the language manager
	m_Language = new CLanguagePack();
	if(!m_Language) return ERR_OUTOFMEM;
	g_Language = m_Language;

	// create opengl object
	m_OpenGL = new COpenGL();
	if(!m_OpenGL) return ERR_OUTOFMEM;
	g_OpenGL = m_OpenGL;

	// create input object
	m_Input = new CInputSystem();
	if(!m_Input) return ERR_OUTOFMEM;
	g_Input = m_Input;

	// create network
	//m_Network = new CNetworkSystem();
	//if(!m_Network) return ERR_OUTOFMEM;
	//g_Network = m_Network;

	// create window system
	m_Winsys = new CWindowSystem();
	if(!m_Winsys) return ERR_OUTOFMEM;
	g_Winsys = m_Winsys;

	// create frustum object
	m_Frustum = new CFrustum();
	if(!m_Frustum) return ERR_OUTOFMEM;
	g_Frustum = m_Frustum;

	// create console object
	m_Console = new CConsole();
	if(!m_Console) return ERR_OUTOFMEM;
	g_Console = m_Console;

	// create player object
	m_Player = new CHumanPlayer();
	if(!m_Player) return ERR_OUTOFMEM;
	g_Player = m_Player;

	// create map object
	m_Map = new CMap();
	if(!m_Map) return ERR_OUTOFMEM;
	g_Map = m_Map;

	// set window title
	m_strWindowTitle = strTitle;

	// initialize sdl
	SDL_Init(SDL_INIT_TIMER);

	// initialize random number generator
	srand(SDL_GetTicks());

	// no errors
	return ERR_OK;
}

//////////////////////////////////////////////////////////////////////////
// Close() : Closes and frees the engine
//////////////////////////////////////////////////////////////////////////

int CEngine::Close()
{
	// destroy map object
	delete m_Map;
	m_Map = NULL;
	g_Map = NULL;

	// destroy player object
	delete m_Player;
	m_Player = NULL;
	g_Player = NULL;

	// destroy console object
	delete m_Console;
	m_Console = NULL;
	g_Console = NULL;

	// destroy window system object
	delete m_Winsys;
	m_Winsys = NULL;
	g_Winsys = NULL;

	// destroy frustum object
	delete m_Frustum;
	m_Frustum = NULL;
	g_Frustum = NULL;

	// destroy network object
	//delete m_Network;
	//m_Network = NULL;
	//g_Network = NULL;

	// shuts down sound system
	//alutExit();

	// destroy input object
	delete m_Input;
	m_Input = NULL;
	g_Input = NULL;

	// destroy opengl object
	delete m_OpenGL;
	m_OpenGL = NULL;
	g_OpenGL = NULL;

	// destroy language object
	delete m_Language;
	m_Language = NULL;
	g_Language = NULL;

	// destroys the options manager
	delete m_Options;
	m_Options = NULL;
	g_Options = NULL;

	// destroys the font manager
	delete m_FontMgr;
	m_FontMgr = NULL;
	g_FontMgr = NULL;

	// destroys the texture manager
	delete m_TextureMgr;
	m_TextureMgr = NULL;
	g_TextureMgr = NULL;

	// shut down sdl
	SDL_Quit();

	// screen variables
	m_uiScreenWidth = 0;
	m_uiScreenHeight = 0;
	m_uiScreenDepth = 0;

	// state
	m_bRunning = false;

	// no errors
	return ERR_OK;
}

//////////////////////////////////////////////////////////////////////////
// SetupModules() : Initializes the modules
//////////////////////////////////////////////////////////////////////////

int CEngine::Setup()
{
	bool bRet;

	// load options
	if(!LoadOptions()) return ERR_SETUP;

	// get screen info
	m_uiScreenWidth = ext::get_option_int("screen_width");
	m_uiScreenHeight = ext::get_option_int("screen_height");
	m_uiScreenDepth = ext::get_option_int("screen_depth");

	// load language pack
	bRet = m_Language->Load("language/español.xml");
	if(!bRet) return ERR_SETUP;

	// create window
	bRet = m_OpenGL->InitWindow(m_strWindowTitle, m_uiScreenWidth,
		m_uiScreenHeight, m_uiScreenDepth, FULLSCREEN_FLAG);
	if(!bRet) return ERR_SETUP;

	// startup input
	bRet = m_Input->Init();
	if(!bRet) return ERR_SETUP;

	// startup sound system
	bool bSfx = ext::get_option_int("sfx") ? true : false;
	bool bMusic = ext::get_option_int("music") ? true : false;
/*	if(bSfx || bMusic)
	{
		alutInit(NULL, 0);
		bRet = alGetError() ? true : false;
		if(bRet) return ERR_SETUP;
	}
*/
	// startup network system
	//bRet = m_Network->Startup();
	//if(!bRet) return ERR_SETUP;

	// startup window system
	bRet = m_Winsys->Startup();
	if(!bRet) return ERR_SETUP;

	// startup console
	bRet = m_Console->Initialize(0, m_uiScreenHeight - 150, 350, 150, "arial");
	if(!bRet) return ERR_SETUP;

	// setup console colors
	m_Console->SetBackColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_Console->SetTextColor(1.0f, 1.0f, 1.0f, 0.7f);

	// setup window system
	m_Winsys->Enable(true);
	m_Winsys->EnableMouse(true);

	// no errors here
	return ERR_OK;
}

//////////////////////////////////////////////////////////////////////////
// Run() : Starts the engine core
//////////////////////////////////////////////////////////////////////////

int CEngine::Run()
{
	int iRet;
	SDL_Event event;

	// setup modules
	iRet = Setup();
	if(iRet != ERR_OK) return iRet;

	// load stuff
	InitialLoad();

	// start running
	m_bRunning = true;

	// main loop
	while(m_bRunning)
	{
		// poll sdl events
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				m_bRunning = false;
				break;
			}
		}

		// Process Engine Tick
		Tick();
	}

	// no errors
	return ERR_OK;
}

//////////////////////////////////////////////////////////////////////////
// Tick() : Main engine loop function
//////////////////////////////////////////////////////////////////////////

void CEngine::Tick()
{
	/////////////////////////////////////////////////////
	// Update
	/////////////////////////////////////////////////////

	// Keyboard & Mouse
	m_Input->Update();

	// Networking System
	//m_Network->Control();

	// Window system
	if(m_Winsys->IsEnabled()) m_Winsys->Update();

	// Player
	if(m_Player->IsAlive()) m_Player->Update();

	// Console (Activation)
	if(m_Input->GetKeyHit(SDLK_RETURN))
	{
		char c = m_Input->GetQueuedKey();
		m_Console->PassKey(c);
		m_Console->Switch();
	}

	// Console (Pass Keys)
	if(m_Console->IsActive())
	{
		// queue key
		char c = m_Input->GetQueuedKey();

		// exhaust queue
		while(c != 0)
		{
			m_Console->PassKey(c);
			c = m_Input->GetQueuedKey();
		}
	}

	/////////////////////////////////////////////////////
	// Handle
	/////////////////////////////////////////////////////

	// screenshot handler
	if(m_Input->GetKey(SDLK_F12)) m_OpenGL->SaveScreenshot();
	if(m_Input->GetKey(SDLK_ESCAPE)) Stop();

	// Player
	if(m_Player->IsAlive())
	{
		// move
		if(m_Input->GetKey(SDLK_w)) m_Player->Move(40);
		if(m_Input->GetKey(SDLK_s)) m_Player->Move(-40);
		if(m_Input->GetKey(SDLK_a)) m_Player->Strafe(-40);
		if(m_Input->GetKey(SDLK_d)) m_Player->Strafe(40);

		// update player
		m_Player->Update();
	}

	/////////////////////////////////////////////////////
	// Clear
	/////////////////////////////////////////////////////

	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Clear color & buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	/////////////////////////////////////////////////////
	// Render
	/////////////////////////////////////////////////////

	// Map
	if(!m_Winsys->IsEnabled())
	{
		// map
		if(m_Map) m_Map->Render();

		// Player model
		if(m_Player) m_Player->Render();

		// Console
		if(m_Console->IsEnabled())
		{
			// Overlay
			m_OpenGL->OverlayBegin(m_uiScreenWidth, m_uiScreenHeight, true);

			// Render Console
			m_Console->Render();

			// Set 3D
			m_OpenGL->OverlayEnd();
		}
	}
	else
	{
		// Set 2D
		m_OpenGL->OverlayBegin(m_uiScreenWidth, m_uiScreenHeight, true);

		// Window system
		if(m_Winsys->IsEnabled()) m_Winsys->Render();

		// Set 3D
		m_OpenGL->OverlayEnd();
	}
	
	// swap buffers
	m_OpenGL->SwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// InitialLoad() : Shows loading screen and loads primary stuff
//////////////////////////////////////////////////////////////////////////

void CEngine::InitialLoad()
{
	//////////////////////////
	// Draw loading text

	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear color & buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	
	// Set 2D
	m_OpenGL->OverlayBegin(m_uiScreenWidth, m_uiScreenHeight, true);

	// Texture
	//glEnable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	//texLoading->Use();

	glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, 1);
			glVertex2i(110, 200);

			glTexCoord2i(0, 0);
			glVertex2i(110, 400);
			
			glTexCoord2i(1, 1);
			glVertex2i(690, 200);

			glTexCoord2i(1, 0);
			glVertex2i(690, 400);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Set 3D
	m_OpenGL->OverlayEnd();

	// Swap buffers
	m_OpenGL->SwapBuffers();

	//////////////////////////
	// Load...

	mm->Create("gfx/gui.pak/main_menu.jpg", FIF_UNKNOWN, false);

	// Fade
	g_OpenGL->FadeToBlack(60);

	//////////////////////////
	// Change mode

	to_MainMenu();
}

//////////////////////////////////////////////////////////////////////////
// MainMenu() : MENU
//////////////////////////////////////////////////////////////////////////

void CEngine::to_MainMenu()
{
	static CButton winConnect;
	static CButton winCreatePlayer;
	static CButton winDeletePlayer;
	static CButton winOptions;
	static CButton winCredits;
	static CButton winQuit;

	// clear window system's windows
	g_Winsys->ClearWindows();

	// boton : "conectar"
	//if(g_Network->IsConnected())
	if(0)
	{
		winConnect.SetCaption("Desconectar");
		winConnect.SetOnClick(menu_Disconnect);
	}
	else
	{
		//winConnect.SetCaption("Conectar al servidor");
		winConnect.SetCaption(g_Language->GetString(STR_CONNECT_TO_SERVER).c_str());
		winConnect.SetOnClick(to_ConnectMenu);
	}
	winConnect.SetPos(550, 150);
	winConnect.SetSize(200, 30);
	g_Winsys->AddWindow(&winConnect);

	// boton : "crear personaje"
	winCreatePlayer.SetPos(550, 200);
	winCreatePlayer.SetSize(200, 30);
	winCreatePlayer.SetCaption("Crear personaje");
	winCreatePlayer.SetOnClick(to_CreatePlayerMenu);
	g_Winsys->AddWindow(&winCreatePlayer);

	// boton : "borrar personaje"
	winDeletePlayer.SetPos(550, 250);
	winDeletePlayer.SetSize(200, 30);
	winDeletePlayer.SetCaption("Borrar personaje");
	winDeletePlayer.SetOnClick(to_DeletePlayerMenu);
	g_Winsys->AddWindow(&winDeletePlayer);

	// boton : "opciones"
	winOptions.SetPos(550, 300);
	winOptions.SetSize(200, 30);
	winOptions.SetCaption("Opciones");
	winOptions.SetOnClick(to_OptionsMenu);
	g_Winsys->AddWindow(&winOptions);

	// boton : "creditos"
	winCredits.SetPos(550, 350);
	winCredits.SetSize(200, 30);
	winCredits.SetCaption("Creditos");
	winCredits.SetOnClick(to_CreditsMenu);
	g_Winsys->AddWindow(&winCredits);

	// boton : "salir"
	winQuit.SetPos(550, 400);
	winQuit.SetSize(200, 30);
	winQuit.SetCaption("Salir");
	g_Winsys->AddWindow(&winQuit);

	// set background
	g_Winsys->SetBackground(mm);
}

//////////////////////////////////////////////////////////////////////////
// Connect() : MENU
//////////////////////////////////////////////////////////////////////////

void CEngine::to_ConnectMenu()
{
	static CButton		winConnect;
	static CButton		winBack;

	static CStatic		sUserName;
	static CTextBox		winUserName;

	static CStatic		sPassWord;
	static CTextBox		winPassWord;

	// clear windows
	g_Winsys->ClearWindows();

	// boton : "conectar"
	winConnect.SetPos(550, 150);
	winConnect.SetSize(200, 30);
	winConnect.SetCaption("Conectar!");
	winConnect.SetOnClick(menu_Connect);
	g_Winsys->AddWindow(&winConnect);

	// boton : "volver"
	winBack.SetPos(550, 200);
	winBack.SetSize(200, 30);
	winBack.SetCaption("Volver");
	winBack.SetOnClick(to_MainMenu);
	g_Winsys->AddWindow(&winBack);

	/////////////////////////////////////////////

	sUserName.SetPos(50, 140);
	sUserName.SetCaption("Nombre de usuario:");
	g_Winsys->AddWindow(&sUserName);

	winUserName.SetPos(50, 150);
	winUserName.SetSize(300, 20);
	winUserName.SetCaption("BetaTester");
	winUserName.SetMaxLength(25);
	winUserName.SetID(ID_CONNECT_USERNAME);
	g_Winsys->AddWindow(&winUserName);

	sPassWord.SetPos(50, 190);
	sPassWord.SetCaption("Password:");
	g_Winsys->AddWindow(&sPassWord);

	winPassWord.SetPos(50, 200);
	winPassWord.SetSize(300, 20);
	winPassWord.SetPassword(true);
	winPassWord.SetMaxLength(25);
	winPassWord.SetID(ID_CONNECT_PASSWORD);
	g_Winsys->AddWindow(&winPassWord);

	// set background
	//g_Winsys->SetBackground(texGetObject("gui_main_menu.jpg"));

	// default button : connect
	winConnect.BringToTop();

}

void CEngine::menu_Connect()
{
	// get username & password
	CWindow *winUserName = g_Winsys->FindWindow(ID_CONNECT_USERNAME);
	CWindow *winPassWord = g_Winsys->FindWindow(ID_CONNECT_PASSWORD);

	// create player
	TQualities t;
	g_Player->Create(winUserName->GetText(), &t);

	// load map
	DWORD tt = GetTickCount();
	g_Map->Load(11, FROM_NORTH);
	g_Player->SetPosition(0, 0, 0);
	std::ostringstream o;
	o << "Map loading time: " << GetTickCount() - tt << " ms.";
	g_Console->Print("Genesis: The Book of Firsts");
	g_Console->Print("Engine Version: 0.1 Alpha");
	g_Console->Print(o.str());

	// set modules state
	g_Winsys->ClearWindows();
	g_Winsys->FreeBackground();
	g_Winsys->Enable(false);
	g_Winsys->EnableMouse(false);
	g_Console->Enable(true);
}

void CEngine::menu_Disconnect()
{
	//g_Network->Disconnect();
}

//////////////////////////////////////////////////////////////////////////
// CreatePlayerMenu() : MENU
//////////////////////////////////////////////////////////////////////////

void CEngine::to_CreatePlayerMenu()
{
	static CButton		winCreate;
	static CButton		winBack;

	static CStatic		sUserName;
	static CTextBox		winUserName;

	static CStatic		sPassWord;
	static CTextBox		winPassWord;

	static CStatic		sMail;
	static CTextBox		winMail;

	static CStatic		sRace;

	static CStatic		sHuman;
	static CCheckBox	winHuman;

	static CStatic		sElf;
	static CCheckBox	winElf;

	static CStatic		sOrc;
	static CCheckBox	winOrc;

	static CImageBox	winRaceImage;

	// clear windows
	g_Winsys->ClearWindows();

	/////////////////////////////////////////////

	// boton : "crear"
	winCreate.SetPos(550, 150);
	winCreate.SetSize(200, 30);
	winCreate.SetCaption("Crear!");
	winCreate.SetOnClick(menu_Create);
	g_Winsys->AddWindow(&winCreate);

	// boton : "volver"
	winBack.SetPos(550, 200);
	winBack.SetSize(200, 30);
	winBack.SetCaption("Volver");
	winBack.SetOnClick(to_MainMenu);
	g_Winsys->AddWindow(&winBack);

	/////////////////////////////////////////////

	sUserName.SetPos(50, 140);
	sUserName.SetCaption("Nombre de usuario:");
	g_Winsys->AddWindow(&sUserName);

	winUserName.SetPos(50, 150);
	winUserName.SetSize(200, 20);
	winUserName.SetCaption("BetaTester");
	winUserName.SetMaxLength(25);
	winUserName.SetID(ID_CREATE_USERNAME);
	g_Winsys->AddWindow(&winUserName);

	sPassWord.SetPos(50, 190);
	sPassWord.SetCaption("Contraseña:");
	g_Winsys->AddWindow(&sPassWord);

	winPassWord.SetPos(50, 200);
	winPassWord.SetSize(200, 20);
	winPassWord.SetMaxLength(25);
	winPassWord.SetPassword(true);
	winPassWord.SetID(ID_CREATE_PASSWORD);
	g_Winsys->AddWindow(&winPassWord);

	sMail.SetPos(50, 240);
	sMail.SetCaption("E-Mail:");
	g_Winsys->AddWindow(&sMail);

	winMail.SetPos(50, 250);
	winMail.SetSize(200, 20);
	winMail.SetMaxLength(60);
	winMail.SetID(ID_CREATE_MAIL);
	g_Winsys->AddWindow(&winMail);

	sRace.SetPos(50, 290);
	sRace.SetCaption("Selecciona una raza:");
	g_Winsys->AddWindow(&sRace);

	sHuman.SetPos(70, 310);
	sHuman.SetCaption("Humano");
	g_Winsys->AddWindow(&sHuman);

	winHuman.SetPos(130, 300);
	winHuman.SetSize(10, 10);
	g_Winsys->AddWindow(&winHuman);

	sElf.SetPos(70, 330);
	sElf.SetCaption("Elfo");
	g_Winsys->AddWindow(&sElf);

	winElf.SetPos(130, 320);
	winElf.SetSize(10, 10);
	g_Winsys->AddWindow(&winElf);

	sOrc.SetPos(70, 350);
	sOrc.SetCaption("Orco");
	g_Winsys->AddWindow(&sOrc);

	winOrc.SetPos(130, 340);
	winOrc.SetSize(10, 10);
	g_Winsys->AddWindow(&winOrc);

	winRaceImage.SetPos(290, 130);
	winRaceImage.SetSize(200, 440);
	//winRaceImage.SetTexture(texGetObject("gui_human.jpg"));
	g_Winsys->AddWindow(&winRaceImage);

	// set background
	//g_Winsys->SetBackground(texGetObject("gui_main_menu.jpg"));

	// default button : connect
	winCreate.BringToTop();
}

void CEngine::menu_Create()
{
}

//////////////////////////////////////////////////////////////////////////
// DeletePlayerMenu() : MENU
//////////////////////////////////////////////////////////////////////////

void CEngine::to_DeletePlayerMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// Options() : MENU
//////////////////////////////////////////////////////////////////////////

void CEngine::to_OptionsMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// Credits() : MENU
//////////////////////////////////////////////////////////////////////////

void CEngine::to_CreditsMenu()
{
	static CStatic sCredits;
	static CButton winBack;

	// clear windows
	g_Winsys->ClearWindows();

	// boton : "volver"
	winBack.SetPos(550, 150);
	winBack.SetSize(200, 30);
	winBack.SetCaption("Volver");
	winBack.SetOnClick(to_MainMenu);
	g_Winsys->AddWindow(&winBack);

	// creditos
	sCredits.SetPos(50, 140);
	sCredits.SetColor(255, 255, 255);
	sCredits.SetCaption("== Lords of Genesis == Copyright (c) 2003 F.M.C.");
	g_Winsys->AddWindow(&sCredits);

	// set background
	//g_Winsys->SetBackground(texGetObject("gui_main_menu.jpg"));

	// default button : connect
	winBack.BringToTop();
}

//////////////////////////////////////////////////////////////////////////
// LoadOptions() : Loads all the options in "options.xml" to a database
//////////////////////////////////////////////////////////////////////////

bool CEngine::LoadOptions()
{
	xmlDocPtr	doc;
	xmlNodePtr	node;
	xmlChar		*type;

	// open and parse options file
	doc = xmlParseFile("options.xml");
	if(!doc) return false;

	// get root node
	node = xmlDocGetRootElement(doc);
	if(!node)
	{
		xmlFreeDoc(doc);
		return false;
	}

	// check for options file
	if(xmlStrcmp(node->name, (const xmlChar *) "options") != 0)
	{
		xmlFreeDoc(doc);
		return false;
	}

	// set to first sub-node
	node = node->children->next;

	// start reading options
	while(node != NULL)
	{
		// we dont want the text inside
		if(xmlStrcmp(node->name, (const xmlChar *) "text") != 0)
		{
			// get option type
			type = xmlGetProp(node, (const xmlChar *) "type");

			// string option
			if(xmlStrcmp(type, (const xmlChar *) "string") == 0)
			{
				xmlChar *key = xmlNodeListGetString(doc, node->children, 1);

				COption *opt = new COption(std::string((const char *) key));
				g_Options->Add(std::string((const char *) node->name), opt);

				xmlFree(key);
			}
			// int option
			else if(xmlStrcmp(type, (const xmlChar *) "int") == 0)
			{
				xmlChar *key = xmlNodeListGetString(doc, node->children, 1);
				int number = (int) xmlXPathCastStringToNumber(key);

				COption *opt = new COption(number);
				g_Options->Add(std::string((const char *) node->name), opt);
				
				xmlFree(key);
			}
			// float option
			else if(xmlStrcmp(type, (const xmlChar *) "float") == 0)
			{
				xmlChar *key = xmlNodeListGetString(doc, node->children, 1);
				float number = (float) xmlXPathCastStringToNumber(key);

				COption *opt = new COption(number);
				g_Options->Add(std::string((const char *) node->name), opt);
				
				xmlFree(key);
			}

			// free type
			xmlFree(type);
		}

		// go to the next node
		node = node->next;
	}

	// close options file
	xmlFreeDoc(doc);

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// SaveOptions() : Saves the options database to "options.xml"
//////////////////////////////////////////////////////////////////////////

bool CEngine::SaveOptions()
{
	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
