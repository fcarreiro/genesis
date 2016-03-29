#include "../precompiled/stdafx.h"
#include "../engine/base.h"

//////////////////////////////////////////////////////////////////////////
// CFile default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CFile::CFile() : m_File(NULL)
{
	// set variables to zero
}

CFile::~CFile()
{
	// close file handle
	Close();
}

//////////////////////////////////////////////////////////////////////////
// CFile overloaded open constructor
//////////////////////////////////////////////////////////////////////////

CFile::CFile(const std::string & strFilename,
			 file_type_t type, file_mode_t mode)
{
	// set variables to zero
	m_File = NULL;

	// open
	Open(strFilename, type, mode);
}

//////////////////////////////////////////////////////////////////////////
// Open() : Opens a file
//////////////////////////////////////////////////////////////////////////

bool CFile::Open(const std::string & strFilename,
				 file_type_t type, file_mode_t mode)
{
	// maybe we already have some file opened
	if(m_File) return false;

	// check for specified type
	if(type == FILE_PACKAGE)
	{
		return OpenPackage(strFilename, mode);
	}
	else if(strFilename.find(".pak") != std::string::npos)
	{
		return OpenPackage(strFilename, mode);
	}
	else
	{
		return OpenStandard(strFilename, mode);
	}

	// nothing could be done
	return false;
}

//////////////////////////////////////////////////////////////////////////
// OpenStandard() : Opens a standard file
//////////////////////////////////////////////////////////////////////////

inline bool CFile::OpenStandard(const std::string & strFilename,
								file_mode_t mode)
{
	// create standard file object
	m_File = new CFileStd();
	if(!m_File) return false;

	// open it
	return m_File->Open(strFilename, mode);
}

//////////////////////////////////////////////////////////////////////////
// OpenPackage() : Opens a package
//////////////////////////////////////////////////////////////////////////

inline bool CFile::OpenPackage(const std::string & strFilename,
							   file_mode_t mode)
{
	// create standard file object
	m_File = new CFilePak();
	if(!m_File) return false;

	// open it
	return m_File->Open(strFilename, mode);
}

//////////////////////////////////////////////////////////////////////////
// Close() : Closes the file handle
//////////////////////////////////////////////////////////////////////////

void CFile::Close()
{
	// destroy file handle
	delete m_File;
	m_File = NULL;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
