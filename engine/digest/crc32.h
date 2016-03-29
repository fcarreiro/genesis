#ifndef __CRC32_H__
#define __CRC32_H__

//////////////////////////////////////////////////////////////////////////
// CCrc32 class
//////////////////////////////////////////////////////////////////////////

class CDigestCrc32 : public CDigest
{
public:
	// default constructor
	CDigestCrc32();
	virtual ~CDigestCrc32();

public:
	// initializes the hashing machine
	virtual bool Initialize();

	// appends a string
	virtual bool Append(const std::string & str);

	// appends an unknown ptr
	virtual bool Append(unsigned char *ptr, unsigned long length);

	// finishes the hash
	virtual bool Finish();

	// get the digest in binary format
	virtual bool GetDigest(char **ptr);

	// get the digest in binary format
	virtual bool GetDigest(std::string & str);

	// get the digest in hex format
	virtual bool GetDigestHex(char **ptr);

	// get the digest in hex format
	virtual bool GetDigestHex(std::string & str);

private:
	// crc table
	static const unsigned long m_dwCrcTable[256];

	// *THE* CRC 32
	unsigned long m_dwCrc32;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
