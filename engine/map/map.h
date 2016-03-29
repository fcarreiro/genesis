#ifndef __MAP_H__
#define __MAP_H__

//////////////////////////////////////////////////////////////////////////
// Map warp directions constants
//////////////////////////////////////////////////////////////////////////

enum TMapWarp {
	FROM_NORTH,
	FROM_SOUTH,
	FROM_EAST,
	FROM_WEST
};

//////////////////////////////////////////////////////////////////////////
// CMap class
//////////////////////////////////////////////////////////////////////////

class CMap
{
public:
	// contructor & destructor
	CMap();
	~CMap();

	// loads a map given the filename
	bool Load(const std::string & strFile, TMapWarp iProcedence);

	// loads a map given the map number
	bool Load(int iMap, TMapWarp iProcedence);

	// unloads the map from memory
	void Free();

	// returns true if there is a map loaded
	bool IsLoaded() const;

	// renders the map
	void Render();

	// adds a player to the list
	void AddPlayer(unsigned long dwId, CPlayer *p);

	// removes a player from the list
	void RemovePlayer(unsigned long dwId);

	// removes all players
	void ClearPlayers() { m_Players.clear(); }

private:
	// renders the map-name bar
	void RenderBar();

private:
	// the sky is falling!
	CSky *m_pSky;

	// life is better in the sea
	CSea *m_pSea;

	// lets get somewhere to stand on.
	CLandscape *m_pLandscape;

	// holds the overall visibility (0-1)
	float m_fVisibility;

	// holds the fog density (0-1)
	float m_fFogDensity;

	// holds overall visibility (0-1)
	float m_fBarAlpha;

	// an auxiliary timer
	CTimer m_Timer;
	
	// holds the map title
	std::string m_strTitle;

	// players database
	std::map<const unsigned long, CPlayer*> m_Players;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
