#include "../precompiled/stdafx.h"
#include "../math/vectors.h"
#include "../math/3dmath.h"
#include "perlin.h"

//////////////////////////////////////////////////////////////////////////
// CPerlinNoise default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CPerlinNoise::CPerlinNoise() : m_fFrequency(0),
m_fPersistance(0), m_iOctaves(0)
{
	// new random image
	m_iRandom = rand();

	// set default (built-int) functions
	m_Noise = noise;
	m_Interpolation = cosine_interpolation;
}

//////////////////////////////////////////////////////////////////////////
// SetParameters() : Setups the info needed to do perlin calculation
//////////////////////////////////////////////////////////////////////////

void CPerlinNoise::SetParameters(float frequency, float persistance, int octaves)
{
	// crop frequency
	if(frequency < 0.0f) frequency = 0.0f;
	if(frequency > 1.0f) frequency = 1.0f;

	// crop persistance
	if(persistance < 0.0f) persistance = 0.0f;
	if(persistance > 1.0f) persistance = 1.0f;

	// crop octaves
	if(octaves <= 0) octaves = 1;

	// set local info
	m_fFrequency = frequency;
	m_fPersistance = persistance;
	m_iOctaves = octaves;
}

//////////////////////////////////////////////////////////////////////////
// GetPoint() : Perlin noise function, returns [0, 1] noise
//////////////////////////////////////////////////////////////////////////

float CPerlinNoise::GetPoint(int x, int y) const
{
	float total = 0.0f;
	float amplitude = 1.0f;
	float frequency = m_fFrequency;

	// create octaves
	for(int i = 0; i < m_iOctaves; ++i)
	{
		// add octave value
		total += interpolate_noise((x + m_iRandom) * frequency,
			(y + m_iRandom) * frequency) * amplitude;

		// setup next octave
		frequency *= 2;
		amplitude *= m_fPersistance;
	}

	// resize total
	total = total * 0.5f + 0.5f;

	// crop total
	if(total < 0.0f) total = 0.0f;
	else if(total > 1.0f) total = 1.0f;

	// total
	return total;
}

//////////////////////////////////////////////////////////////////////////
// FillTable() : Fills an unsigned char table
//////////////////////////////////////////////////////////////////////////

bool CPerlinNoise::FillTable(unsigned char *table, int width, int height) const
{
	int x;
	int y;
	float total;

	// if we dont have an output buffer we quit
	if(!table) return false;

	// filling loop
	for(x = 0; x < width; ++x)
	{
		for(y = 0; y < height; ++y)
		{
			// seamless
			total =  GetPoint(x, y) * (width - x) * (height - y);
			total += GetPoint(x - width, y) * (x) * (height - y);
			total += GetPoint(x - width, y - height) * (x) * (y);
			total += GetPoint(x, y - height) * (width - x) * (y);
			total /= (width * height);

			// assign
			table[y * height + x] = (unsigned char)(total * 255);
		}
	}

	// all right
	return true;
}

//////////////////////////////////////////////////////////////////////////
// FillTable() : Fills an float table
//////////////////////////////////////////////////////////////////////////

bool CPerlinNoise::FillTable(float *table, int width, int height) const
{
	int x;
	int y;
	float total;

	// if we dont have an output buffer we quit
	if(!table) return false;

	// filling loop
	for(x = 0; x < width; ++x)
	{
		for(y = 0; y < height; ++y)
		{
			// seamless
			total =  GetPoint(x, y) * (width - x) * (height - y);
			total += GetPoint(x - width, y) * (x) * (height - y);
			total += GetPoint(x - width, y - height) * (x) * (y);
			total += GetPoint(x, y - height) * (width - x) * (y);
			total /= (width * height);

			// assign
			table[y * height + x] = total;
		}
	}

	// all right
	return true;
}

//////////////////////////////////////////////////////////////////////////
// interpolate_noise() : Returns interpolated smoothed noise
//////////////////////////////////////////////////////////////////////////

float CPerlinNoise::interpolate_noise(float x, float y) const
{
	static int integer_X;
	static int integer_Y;
	static float fractional_X;
	static float fractional_Y;
	static float v1, v2, v3, v4, i1, i2;

	// separate integer & decimal x
	integer_X = (int) x;
	fractional_X = x - integer_X;

	// separate integer & decimal y
	integer_Y = (int) y;
	fractional_Y = y - integer_Y;

	// get 4 corners
	v1 = smooth_noise(integer_X,     integer_Y);
	v2 = smooth_noise(integer_X + 1, integer_Y);
	v3 = smooth_noise(integer_X,     integer_Y + 1);
	v4 = smooth_noise(integer_X + 1, integer_Y + 1);

	// interpolate 4 corners
	i1 = m_Interpolation(v1, v2, fractional_X);
	i2 = m_Interpolation(v3, v4, fractional_X);

	// interpolate 2 points
	return m_Interpolation(i1, i2 ,fractional_Y);
}

//////////////////////////////////////////////////////////////////////////
// smooth_noise() : Returns smoothed noise
//////////////////////////////////////////////////////////////////////////

float CPerlinNoise::smooth_noise(int x, int y) const
{
	static float corners;
	static float sides;
	static float center;

	// get corners
	corners = (m_Noise(x - 1, y - 1) +
			  m_Noise(x + 1, y - 1) +
			  m_Noise(x - 1, y + 1) +
			  m_Noise(x + 1, y + 1)) / 16.0f;

	// get sides
	sides = (m_Noise(x - 1, y) +
		    m_Noise(x + 1, y) +
		    m_Noise(x, y - 1) +
		    m_Noise(x, y + 1)) / 8.0f;

	// get center
	center = m_Noise(x, y) / 4.0f;

	// return smoothed noise
	return (corners + sides + center);
}

//////////////////////////////////////////////////////////////////////////
// noise() : Default noise function
//////////////////////////////////////////////////////////////////////////

float CPerlinNoise::noise(int x, int y)
{
	static int n;
	
	n = x + y * 57;
	n = (n << 13) ^ n;

	return (1.0f - ((n * (n * n * 15731 + 789221)
	+ 1376312589) & 0x7fffffff ) / 1073741824.0f);
}

//////////////////////////////////////////////////////////////////////////
// linear_interpolation() : Crappy interpolation
//////////////////////////////////////////////////////////////////////////

float CPerlinNoise::linear_interpolation(float a, float b, float x)
{
	// return interpolated point
	return a * (1.0f - x) + (b * x);
}

//////////////////////////////////////////////////////////////////////////
// cosine_interpolation() : Smooth interpolation
//////////////////////////////////////////////////////////////////////////

float CPerlinNoise::cosine_interpolation(float a, float b, float x)
{
	static float f;

	// return interpolated point
	f = (float) ((1.0f - cos(x * PI)) * 0.5f);
	return a * (1.0f - f) + (b * f);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
