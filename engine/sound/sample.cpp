#include "../precompiled/stdafx.h"
#include "sample.h"

//////////////////////////////////////////////////////////////////////////
// CSoundSample default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CSoundSample::CSoundSample() : m_dwTime(0),
m_uiFrequency(0), m_uiBitDepth(0), m_uiChannels(0)
{
	// initialize variables
}

CSoundSample::~CSoundSample()
{
	// just int case
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// Create() : Creates a sample through a loading function
//////////////////////////////////////////////////////////////////////////

bool CSoundSample::Create(const std::string & strFilename, snd_loader loader)
{
	// load it
	return loader(strFilename, this);
}

//////////////////////////////////////////////////////////////////////////
// Create() : Allocates a sample using the provided info
//////////////////////////////////////////////////////////////////////////

bool CSoundSample::Create(unsigned long dwTime, unsigned int uiFrequency,
						  unsigned int uiBitDepth, unsigned int uiChannels)
{
	// destroy old data (if any)
	Destroy();

	// allocate vector
	m_vData.resize(dwTime * uiFrequency * uiChannels * (uiBitDepth / 8));
	if(m_vData.empty()) return false;

	// set variables
	m_dwTime = dwTime;
	m_uiFrequency = uiFrequency;
	m_uiBitDepth = uiBitDepth;
	m_uiChannels = uiChannels;

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Destroy() : Frees all the data
//////////////////////////////////////////////////////////////////////////

void CSoundSample::Destroy()
{
	// reset variables
	m_dwTime = 0;
	m_uiFrequency = 0;
	m_uiBitDepth = 0;
	m_uiChannels = 0;

	// clear vector
	m_vData.clear();
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
