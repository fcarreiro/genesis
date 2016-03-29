#ifndef __FILE_STANDARD_H__
#define __FILE_STANDARD_H__

//////////////////////////////////////////////////////////////////////////
// CStandard class
//////////////////////////////////////////////////////////////////////////

class CFileStd : public CFileBase
{
public:
	// constructor & destructor
	CFileStd();
	virtual ~CFileStd();

	// returns true if we have an open file
	virtual bool IsOpen() const { return (m_Handle != NULL); }

	// returns true if we have reached the end
	virtual bool IsEof() const;

	// returns the file size
	virtual unsigned long Size();

	// reads from the file
	virtual unsigned long Read(void *pBuffer, unsigned long dwLength);

	// returns the actual position in the file
	virtual unsigned long Tell();

	// seeks to requested position
	virtual void Seek(long dwOffset, unsigned int uiFrom);

	// opens a file
	virtual bool Open(const std::string & strFilename, file_mode_t mode);

	// close the actual file
	virtual void Close();

private:
	// the file handle
	FILE *m_Handle;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
