#ifndef __PERLIN_H__
#define __PERLIN_H__

//////////////////////////////////////////////////////////////////////////
// Function pointer types
//////////////////////////////////////////////////////////////////////////

typedef float (*noise_func)(int, int);
typedef float (*interpolation_func)(float, float, float);

//////////////////////////////////////////////////////////////////////////
// CPerlinNoise class
//////////////////////////////////////////////////////////////////////////

class CPerlinNoise
{
public:
	// default constructor
	CPerlinNoise();

public:
	// setups the info needed to do perlin calculation
	void SetParameters(float frequency, float persistance, int octaves);

	// sets new random number
	void SetRandom(int rnd) { m_iRandom = rnd; }

	// sets the interpolation function
	void SetInterpolation(interpolation_func new_interpolation)	{
	if(new_interpolation) m_Interpolation = new_interpolation; }

	// sets the noise function
	void SetNoise(noise_func new_noise)	{
	if(new_noise) m_Noise = new_noise; }

	// returns perlin noise [0, 1] for point x, y
	float GetPoint(int x, int y) const;

	// fills an unsigned char table
	bool FillTable(unsigned char *table, int width, int height) const;

	// fills an float table
	bool FillTable(float *table, int width, int height) const;

public:
	// interpolates between a and b (cosine)
	static float cosine_interpolation(float a, float b, float x);

	// interpolates between a and b (linear)
	static float linear_interpolation(float a, float b, float x);

private:
	// returns smoothed noise for x, y
	float smooth_noise(int x, int y) const;

	// returns interpolated smoothed noise for x, y
	float interpolate_noise(float x, float y) const;

	// default noise function
	static float noise(int x, int y);

private:
	// noise function
	noise_func m_Noise;

	// interpolation function
	interpolation_func m_Interpolation;

	// random number
	int m_iRandom;

	// noise frequency
	float m_fFrequency;

	// octave persistance
	float m_fPersistance;

	// number of octaves
	int m_iOctaves;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
