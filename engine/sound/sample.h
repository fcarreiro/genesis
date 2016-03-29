#ifndef __SOUNDSAMPLE_H__
#define __SOUNDSAMPLE_H__

//////////////////////////////////////////////////////////////////////////
// Sound loader prototype
//////////////////////////////////////////////////////////////////////////

class CSoundSample;
typedef (*snd_loader)(const std::string &, CSoundSample *);

//////////////////////////////////////////////////////////////////////////
// CSample class
//////////////////////////////////////////////////////////////////////////

class CSoundSample
{
public:
	// constructor & destructor
	CSoundSample();
	~CSoundSample();

public:
	// creates a sound sample through a sound loading function
	bool Create(const std::string & strFilename, snd_loader loader);

	// creates a sound sample
	bool Create(unsigned long dwTime, unsigned int uiFrequency,
		unsigned int uiBitDepth, unsigned int uiChannels);

	// returns the sample length in seconds
	unsigned long GetTime() const { return m_dwTime; }

	// returns the sample frequency
	unsigned int GetFrequency() const { return m_uiFrequency; }

	// returns the sample bit-depth
	unsigned int GetDepth() const { return m_uiBitDepth; }

	// returns the sample's number of channels
	unsigned int GetChannels() const { return m_uiChannels; }

	// returns a pointer to the data
	void *GetData() { return (m_vData.empty() ? NULL : &m_vData[0]); }

	// frees pcm data
	void Destroy();

private:
	// holds pcm data
	std::vector<unsigned char> m_vData;

	// holds time
	unsigned long m_dwTime;

	// holds frequency
	unsigned int m_uiFrequency;

	// holds bit depth
	unsigned int m_uiBitDepth;

	// holds channels
	unsigned int m_uiChannels;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
