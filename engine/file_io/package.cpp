#include "../precompiled/stdafx.h"
#include "../engine/base.h"

//////////////////////////////////////////////////////////////////////////
// Constructor & Destructor
//////////////////////////////////////////////////////////////////////////

CFilePak::CFilePak() : m_File(NULL),
m_Size(0), m_Offset(0)
{
	// set variables to zero
}

CFilePak::~CFilePak()
{
	// close
	Close();
}

//////////////////////////////////////////////////////////////////////////
// Open() : Opens the package
//////////////////////////////////////////////////////////////////////////

bool CFilePak::Open(const std::string & strFilename, file_mode_t mode)
{
	int iErr;
	unzFile	package;
	unz_file_info info;
	std::string str_package;
	std::string str_inner_file;
	std::string::size_type index;

	// close any open file
	Close();

	// find '\'
	index = strFilename.find_last_of('\\');

	// if not found try '/'
	if(index == std::string::npos)
	index = strFilename.find_last_of('/');

	// check for error (not '\' nor '/')
	if(index == std::string::npos) return false;

	// separate pak file from inner file
	str_package.assign(strFilename, 0, index);
	str_inner_file.assign(strFilename, index + 1, strFilename.length());

	// try to open file
	package = unzOpen(str_package.c_str());
	if(!package) return false;
	
	// locate file
	if(unzLocateFile(package, str_inner_file.c_str(), 1) != UNZ_OK ||
	   unzOpenCurrentFile(package) != UNZ_OK)
	{
		// close package
		unzClose(package);

		// oops
		return false;
	}

	// get file size
	iErr = unzGetCurrentFileInfo(package, &info,
		NULL, NULL, NULL, 0, NULL, 0);
	
	// did we have an error?
	if(iErr != UNZ_OK)
	{
		// just in case we had open a file inside
		unzCloseCurrentFile(package);

		// close package
		unzClose(package);

		// error
		return false;
	}
	else
	{
		m_Size = info.uncompressed_size;
	}

	// allocate file
	m_File = new unsigned char[m_Size];
	if(!m_File)
	{
		// just in case we had open a file inside
		unzCloseCurrentFile(package);

		// close package
		unzClose(package);

		// error
		return false;
	}

	// read file
	unzReadCurrentFile(package, m_File, m_Size);

	// just in case we had open a file inside
	unzCloseCurrentFile(package);

	// close package
	unzClose(package);

	// oookk
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Close() : Closes the package
//////////////////////////////////////////////////////////////////////////

void CFilePak::Close()
{
	// destroy file array
	delete [] (unsigned char *) m_File;
	m_File = NULL;

	// set variables to 0
	m_Size = 0;
	m_Offset = 0;
}

//////////////////////////////////////////////////////////////////////////
// Eof() : Returns true if we have reached the end of the file
//////////////////////////////////////////////////////////////////////////

bool CFilePak::IsEof() const
{
	// eof?
	if(m_File && m_Offset != m_Size) return false;

	// error
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Size() : Returns the size of the file
//////////////////////////////////////////////////////////////////////////

unsigned long CFilePak::Size()
{
	// returns the size
	return m_Size;
}

//////////////////////////////////////////////////////////////////////////
// Read() : Reads file inside package
//////////////////////////////////////////////////////////////////////////

unsigned long CFilePak::Read(void *pBuffer, unsigned long dwLength)
{
	// do some checking
	if(!m_File) return 0;

	// trim/crop
	if(m_Offset + dwLength > m_Size) dwLength = m_Size - m_Offset;

	// copy bytes
	memcpy(pBuffer, (unsigned char *) m_File + m_Offset, dwLength);	

	// increment offset
	m_Offset += dwLength;

	// return read bytes
	return dwLength;
}

//////////////////////////////////////////////////////////////////////////
// Tell() : Returns the actual offset in the file
//////////////////////////////////////////////////////////////////////////

unsigned long CFilePak::Tell()
{
	// return offset
	return m_Offset;
}

//////////////////////////////////////////////////////////////////////////
// Seek() : Seeks to requested position
//////////////////////////////////////////////////////////////////////////

void CFilePak::Seek(long dwOffset, unsigned int uiFrom)
{
	// check
	if(!m_File) return;

	// seek
	switch(uiFrom)
	{
	case SEEK_SET:
		m_Offset = dwOffset;
		if(m_Offset > m_Size) m_Offset = m_Size;
		if(m_Offset < 0) m_Offset = 0;
		break;

	case SEEK_CUR:
		m_Offset += dwOffset;
		if(m_Offset > m_Size) m_Offset = m_Size;
		if(m_Offset < 0) m_Offset = 0;
		break;

	case SEEK_END:
		m_Offset = m_Size + dwOffset;
		if(m_Offset > m_Size) m_Offset = m_Size;
		if(m_Offset < 0) m_Offset = 0;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
