#ifndef __FILE_H__
#define __FILE_H__

//////////////////////////////////////////////////////////////////////////
// CFile class
//////////////////////////////////////////////////////////////////////////

class CFile
{
public:
	// default constructor & destructor
	CFile();
	~CFile();

	// overloaded open constructor
	CFile(const std::string & strFilename,
		file_type_t type = FILE_AUTO, file_mode_t mode = FILE_BINARY);

public:
	// opens the file
	bool Open(const std::string & strFilename,
		file_type_t type = FILE_AUTO, file_mode_t mode = FILE_BINARY);

	// closes the file
	void Close();

	// returns true if we have an open file
	bool IsOpen() const { if(m_File) return m_File->IsOpen(); else return false; }

	// returns true if we have reached the end
	bool IsEof() const { if(m_File) return m_File->IsEof(); else return false; }

	// returns the file size
	unsigned long Size() { if(m_File) return m_File->Size(); else return 0; }

	// returns actual offset in the file
	unsigned long Tell() { if(m_File) return m_File->Tell(); else return 0; }

	// seeks to asked position
	void Seek(long dwOffset, unsigned int uiFrom) {
		if(m_File) m_File->Seek(dwOffset, uiFrom); }

	// reads from the file
	unsigned long Read(void *pBuffer, unsigned long dwLength) {
		if(m_File) return m_File->Read(pBuffer, dwLength); else return 0; }

private:
	// opens standard file
	bool OpenStandard(const std::string & strFilename, file_mode_t mode);

	// opens package
	bool OpenPackage(const std::string & strFilename, file_mode_t mode);

private:
	// pointer to real file class
	CFileBase *m_File;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
