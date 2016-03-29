#ifndef __FILE_PACKAGE_H__
#define __FILE_PACKAGE_H__

//////////////////////////////////////////////////////////////////////////
// CFilePak class
//////////////////////////////////////////////////////////////////////////

class CFilePak : public CFileBase
{
public:
	// constructor & destructor
	CFilePak();
	virtual ~CFilePak();

	// returns true if we have an open package
	virtual bool IsOpen() const { return (m_File != NULL); }

	// returns true if we have reached the end
	virtual bool IsEof() const;

	// returns the file size
	virtual unsigned long Size();

	// reads from the selected file inside the package
	virtual unsigned long Read(void *pBuffer, unsigned long dwLength);

	// returns the actual position in the file
	virtual unsigned long Tell();

	// seeks to requested position
	virtual void Seek(long dwOffset, unsigned int uiFrom);

	// opens a package
	virtual bool Open(const std::string & strFilename, file_mode_t mode);

	// close the actual package
	virtual void Close();

private:
	// the whole file
	void *m_File;

	// file size
	unsigned long m_Size;

	// actual file offset
	unsigned long m_Offset;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
