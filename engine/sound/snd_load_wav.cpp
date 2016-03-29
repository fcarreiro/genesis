#include "../precompiled/stdafx.h"
#include "../file_io/file_base.h"
#include "../file_io/package.h"
#include "../file_io/standard.h"
#include "../file_io/file.h"
#include "sample.h"
#include "snd_load_wav.h"

//////////////////////////////////////////////////////////////////////////
// sndLoadWAV() : Loads sound sample from WAV file
//////////////////////////////////////////////////////////////////////////

bool sndLoadWAV(const std::string & filename, CSoundSample *sample)
{
/*	CFile in;
	std::vector<char> buffer;

	// open wav
	in.Open(filename);
	if(!in.IsOpen()) return false;

	// allocate file size
	buffer.resize(in.Size());
	if(buffer.empty()) return false;

	// read the file & close it
	in.Read(&buffer[0], in.Size());
	in.Close();
*/
	// load wav
//	ALvoid *data;
//	ALsizei size, freq, format;
	//alutLoadWAVMemory(&buffer[0], &format, &data, &size, &freq);

	// clear buffer
//	buffer.clear();

	// copy & set
	//sample->Create(0, freq, *bits*, *channels*);
//	memcpy(sample->GetData(), data, size);

	// unload wav
//	alutUnloadWAV(format, &data, size, freq);

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
