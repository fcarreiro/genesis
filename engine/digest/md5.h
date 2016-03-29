#ifndef __MD5_H__
#define __MD5_H__

//////////////////////////////////////////////////////////////////////////
// MD5 class
//////////////////////////////////////////////////////////////////////////

class CDigestMd5 : public CDigest
{
public:
	// default constructor
	CDigestMd5();
	virtual ~CDigestMd5();

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
	// md5 state
	md5_state_t m_State;

	// md5 digest
	unsigned char m_Digest[16];
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
