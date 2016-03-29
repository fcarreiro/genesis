#include "../precompiled/stdafx.h"
#include "../engine/base.h"
#include "../../common/utility/ext_xml.h"
#include "../../common/utility/ext_util.h"

//////////////////////////////////////////////////////////////////////////
// CMap default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CMap::CMap()
{
	// sub-modules
	m_pSky = NULL;
	m_pSea = NULL;
	m_pLandscape = NULL;

	// variables
	m_fVisibility = 0.0f;
	m_fFogDensity = 0.0f;
	m_fBarAlpha = 0.0f;
}

CMap::~CMap()
{
	// free data
	Free();
}

//////////////////////////////////////////////////////////////////////////
// Free() : Unloads all map-related data from memmory
//////////////////////////////////////////////////////////////////////////

void CMap::Free()
{
	// destroy sky
	delete m_pSky;
	m_pSky = NULL;

	// destroy sea
	delete m_pSea;
	m_pSea = NULL;

	// destroy landscape
	delete m_pLandscape;
	m_pLandscape = NULL;

	// reset variables
	m_fVisibility = 0.0f;
	m_fFogDensity = 0.0f;
	m_fBarAlpha = 0.0f;
	m_Timer.Update();
	m_strTitle.erase();

	// free players list
	m_Players.clear();
}

//////////////////////////////////////////////////////////////////////////
// Load() : Loads a map
//////////////////////////////////////////////////////////////////////////

bool CMap::Load(int iMap, TMapWarp iProcedence)
{
	// format the string
	std::ostringstream temp;
	temp << "maps/map" << iMap << ".pak";

	// load the map
	return Load(temp.str(), iProcedence);
}

//////////////////////////////////////////////////////////////////////////
// Load() : Loads a map
//////////////////////////////////////////////////////////////////////////

bool CMap::Load(const std::string & strFile, TMapWarp iProcedence)
{
	// temporary variables
	xmlDocPtr doc;
	xmlNodePtr node;
	std::string strTemp;

	// map info variables
	std::string name;
	std::string music;
	int length;
	int hm_length;
	int shoreline;
	int min_stars;
	int max_stars;
	float fog_density;
	float visibility;
	float scale;
	CVector3 vDayBaseColor;
	CVector3 vNightBaseColor;
	CVector3 vDayBaseAmbient;
	CVector3 vNightBaseAmbient;

	// PHASE 0: free everything
	Free();

	// PHASE 1: open xml file
	strTemp = strFile + "/settings.xml";
	doc = ext::xml_parse_file(strTemp);

	// check for document
	if(!doc) return false;

	// get root element and check for map
	node = xmlDocGetRootElement(doc);
	if(!node || xmlStrcmp(node->name, (const xmlChar *) "map"))
	{
		xmlFreeDoc(doc);
		return false;
	}

	// get directly to first node
	node = node->children->next;

	// retrieve map settings
	name		= ext::xml_get_string(doc, node, "name");
	music		= ext::xml_get_string(doc, node, "music");
	length		= ext::xml_get_int(doc, node, "length");
	hm_length	= ext::xml_get_int(doc, node, "heightmap_length");
	shoreline	= ext::xml_get_int(doc, node, "shoreline");
	min_stars	= ext::xml_get_int(doc, node, "min_stars");
	max_stars	= ext::xml_get_int(doc, node, "max_stars");
	fog_density	= ext::xml_get_float(doc, node, "fog_density");
	visibility	= ext::xml_get_float(doc, node, "visibility");
	scale		= ext::xml_get_float(doc, node, "scale");

	// retrieve sky settings
	vDayBaseColor.x = ext::xml_get_prop_float(doc, node, "sky_day_color", "r");
	vDayBaseColor.y = ext::xml_get_prop_float(doc, node, "sky_day_color", "g");
	vDayBaseColor.z = ext::xml_get_prop_float(doc, node, "sky_day_color", "b");
	vNightBaseColor.x = ext::xml_get_prop_float(doc, node, "sky_night_color", "r");
	vNightBaseColor.y = ext::xml_get_prop_float(doc, node, "sky_night_color", "g");
	vNightBaseColor.z = ext::xml_get_prop_float(doc, node, "sky_night_color", "b");
	vDayBaseAmbient.x = ext::xml_get_prop_float(doc, node, "sky_day_ambient", "r");
	vDayBaseAmbient.y = ext::xml_get_prop_float(doc, node, "sky_day_ambient", "g");
	vDayBaseAmbient.z = ext::xml_get_prop_float(doc, node, "sky_day_ambient", "b");
	vNightBaseAmbient.x = ext::xml_get_prop_float(doc, node, "sky_night_ambient", "r");
	vNightBaseAmbient.y = ext::xml_get_prop_float(doc, node, "sky_night_ambient", "g");
	vNightBaseAmbient.z = ext::xml_get_prop_float(doc, node, "sky_night_ambient", "b");

	// free file
	xmlFreeDoc(doc);

	// PHASE 2: Sky allocation
	m_pSky = new CSky();
	if(!m_pSky) return false;

	// place slightly under the player
	CVector3 vCenter = CVector3(0.0f, 20.0f, 0.0f);

	// create sky
	m_pSky->Create(200, 200, 20,
				  min_stars, max_stars,
				  0.5f, vCenter,
				  vNightBaseColor, vDayBaseColor,
				  vNightBaseAmbient, vDayBaseAmbient);

	// set null time (night)
	m_pSky->SetDayTime(0.0f);

	// PHASE 3: Landscape allocation
	m_pLandscape = new CLandscape();
	if(!m_pLandscape) return false;

	// create landsape
	strTemp = strFile + "/texture.tex";
	std::string strHeightmap = strFile + "/heightmap.raw";

	if(!m_pLandscape->Create(hm_length, scale,
		strHeightmap.c_str(), strTemp.c_str())) return false;

	// PHASE 5: create the sea
	if(shoreline)
	{
		// allocate sea
		m_pSea = new CSea();
		if(!m_pSea) return false;

		// create sea
		if(!m_pSea->Create(length, shoreline * scale, m_pLandscape))
			return false;
	}
/*
	// PHASE 6: player posistion
	float py;

	// where do we come from?
	switch(iProcedence)
	{
	case FROM_NORTH:
		py = m_pLandscape.HeightAt(vPosition.x, iMapLength - 300) + 2;
		g_Player->SetPosition(vPosition.x, py, 300 - iMapLength);
		break;

	case FROM_SOUTH:
		py = m_pLandscape.HeightAt(vPosition.x, 300) + 2;
		g_Player->SetPosition(vPosition.x, py, -300);
		break;

	case FROM_EAST:
		py = m_pLandscape.HeightAt(iMapLength - 300, -vPosition.z) + 2;
		g_Player->SetPosition(iMapLength - 300, py, vPosition.z);
		break;

	case FROM_WEST:
		py = m_pLandscape.HeightAt(300, -vPosition.z) + 2;
		g_Player->SetPosition(300, py, vPosition.z);
		break;
	}
*/
	// PHASE 7: set local values
	m_strTitle = name;
	m_fVisibility = visibility;
	m_fFogDensity = fog_density;

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Render() : Renders the map
//////////////////////////////////////////////////////////////////////////

void CMap::Render()
{
	float fFogColor[4];
	bool bUnderWater = false;

	// set fog parameters
	if(bUnderWater)
	{
		// set fog color
		fFogColor[0] = 0.1f;
		fFogColor[1] = 0.1f;
		fFogColor[2] = 0.3f;
		fFogColor[3] = 1.0f;

		// set fog options
		glFogf(GL_FOG_DENSITY, m_fFogDensity);
		glFogf(GL_FOG_START, 50.0f);
		glFogf(GL_FOG_END, 100.0f);
		glFogfv(GL_FOG_COLOR, fFogColor);
		glEnable(GL_FOG);
	}
	else
	{
		// set fog color
		fFogColor[0] = 0.6f;
		fFogColor[1] = 0.6f;
		fFogColor[2] = 0.6f;
		fFogColor[3] = 1.0f;

		// set fog options
		glFogf(GL_FOG_DENSITY, m_fFogDensity);
		glFogf(GL_FOG_START, m_fVisibility * 200.0f);
		glFogf(GL_FOG_END, 300.0f);
		glFogfv(GL_FOG_COLOR, fFogColor);
	}

	glEnable(GL_FOG);

	// set player's viewpoint
	g_Player->Look();

	// update frustum
	g_Frustum->CalculateFrustum();

	// render sky
	m_pSky->Render(g_Player->GetPosition(), m_pSea ? m_pSea->GetShoreLine() : 0.0f,
		CVector3(fFogColor[0], fFogColor[1], fFogColor[2]));

	// render landscape
	m_pLandscape->Render();

	// render sea
	if(m_pSea) m_pSea->Render();

	// render bar with name
	if(m_fBarAlpha >= 0.0f) RenderBar();

	glDisable(GL_FOG);
}

//////////////////////////////////////////////////////////////////////////
// RenderBar() : Draws the bar with the map title
//////////////////////////////////////////////////////////////////////////

void CMap::RenderBar()
{
	static CFont barFont("blackletter", 20, FONT_TEXTURE);

	// calculate alpha
	if(m_fBarAlpha < 1.0f)
	{
		m_fBarAlpha += 0.01f;
		m_Timer.Update();
	}
	else
	{
		if(m_Timer.GetTimePassed() >= 5000) m_fBarAlpha -= 0.01f;
	}

	// get width & height
	static const int iWidth = ext::get_option_int("screen_width");
	static const int iHeight = ext::get_option_int("screen_height");

	// overlay begin
	g_OpenGL->OverlayBegin(iWidth, iHeight, true);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set alpha
	glColor4f(0.0f, 0.0f, 0.0f, m_fBarAlpha);

	// draw bar background
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(iWidth, 35);
		glVertex2i(0, 35);
		glVertex2i(iWidth, 0);
		glVertex2i(0, 0);
	glEnd();

	// set white
	glColor4f(1.0f, 1.0f, 1.0f, m_fBarAlpha);

	// draw map name
	barFont.Print(10, 8, m_strTitle);

	// disable blending
	glDisable(GL_BLEND);

	// overlay ending
	g_OpenGL->OverlayEnd();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CMap::AddPlayer(unsigned long dwId, CPlayer *p)
{
	// if we *DON'T* have the player
	if(m_Players.find(dwId) == m_Players.end())
	{
		// add it
		m_Players.insert(std::make_pair(dwId, p));
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void CMap::RemovePlayer(unsigned long dwId)
{
	// if we *DO* have the player
	if(m_Players.find(dwId) != m_Players.end())
	{
		// delete it
		m_Players.erase(dwId);
	}
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
