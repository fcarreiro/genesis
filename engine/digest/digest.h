#ifndef __DIGEST_H__
#define __DIGEST_H__

//////////////////////////////////////////////////////////////////////////
// CDigest class
//////////////////////////////////////////////////////////////////////////

class CDigest
{
public:
	// default constructor
	CDigest() : m_bInitialized(false), m_bFinished(false) { }
	virtual ~CDigest() { }

public:
	// initializes the Digesting machine
	virtual bool Initialize() = 0;

	// appends a string
	virtual bool Append(const std::string & str) = 0;

	// appends an unknown ptr
	virtual bool Append(unsigned char *ptr, unsigned long length) = 0;

	// finishes the Digest
	virtual bool Finish() = 0;

	// get the digest in binary format
	virtual bool GetDigest(char **ptr) = 0;

	// get the digest in binary format
	virtual bool GetDigest(std::string & str) = 0;

	// get the digest in hex format
	virtual bool GetDigestHex(char **ptr) = 0;

	// get the digest in hex format
	virtual bool GetDigestHex(std::string & str) = 0;

protected:
	// are we ready to go?
	bool m_bInitialized;

	// do we have a digest to show?
	bool m_bFinished;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
