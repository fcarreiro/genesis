#ifndef __FILE_BASE_H__
#define __FILE_BASE_H__

//////////////////////////////////////////////////////////////////////////
// Archive type constants
//////////////////////////////////////////////////////////////////////////

enum file_type_t
{
	FILE_AUTO,
	FILE_STANDARD,
	FILE_PACKAGE
};

//////////////////////////////////////////////////////////////////////////
// Open mode constants
//////////////////////////////////////////////////////////////////////////

enum file_mode_t
{
	FILE_ASCII,
	FILE_BINARY
};

//////////////////////////////////////////////////////////////////////////
// CFileBase class
//////////////////////////////////////////////////////////////////////////

class CFileBase
{
public:
	CFileBase() { }
	virtual ~CFileBase() { }

public:
	// returns true if we have an open file
	virtual bool IsOpen() const = 0;

	// returns true if we have reached the end
	virtual bool IsEof() const = 0;

	// returns the file size
	virtual unsigned long Size() = 0;

	// reads from the file
	virtual unsigned long Read(void *pBuffer, unsigned long dwLength) = 0;

	// returns the actual position in the file
	virtual unsigned long Tell() = 0;

	// seeks to requested position
	virtual void Seek(long dwOffset, unsigned int uiFrom) = 0;

	// opens a file
	virtual bool Open(const std::string & strFilename, file_mode_t mode) = 0;

	// close the file
	virtual void Close() = 0;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
