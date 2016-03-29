#include "../precompiled/stdafx.h"
#include "md5_base.h"
#include "digest.h"
#include "md5.h"

//////////////////////////////////////////////////////////////////////////
// Default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CDigestMd5::CDigestMd5()
{
}

CDigestMd5::~CDigestMd5()
{
}

//////////////////////////////////////////////////////////////////////////
// Initialize() : Does whatever initialization needed to compute CRC32
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::Initialize()
{
	if(m_bInitialized)
	{
		// already initialized
		return false;
	}
	else
	{
		// initialize state
		md5_init(&m_State);

		// reset digest
		memset(m_Digest, 0, sizeof(m_Digest));

		// initialize
		m_bInitialized = true;
		m_bFinished = false;

		// ok
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
// Append() : Adds a string to the actual md5
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::Append(const std::string & str)
{
	// hey! initialize me first!
	if(!m_bInitialized) return false;

	// md5 append
	md5_append(&m_State, (const md5_byte_t *) str.c_str(), str.length());

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Append() : Adds a string to the actual md5
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::Append(unsigned char *ptr, unsigned long length)
{
	// hey! initialize me first!
	if(!m_bInitialized) return false;

	// md5 append
	md5_append(&m_State, (const md5_byte_t *) ptr, length);

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Finish() : Computes the final MD5 and closes session
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::Finish()
{
	// hey! initialize me first!
	if(!m_bInitialized) return false;

	// save md5
	md5_finish(&m_State, (md5_byte_t *) m_Digest);

	// close session
	m_bInitialized = false;
	m_bFinished = true;

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// GetDigest() : Get the digest in binary format
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::GetDigest(char **ptr)
{
	// stupid!
	if(!ptr) return false;

	// we need to have a crc32
	if(!m_bFinished) return false;

	// allocate space for the digest
	*ptr = new char[16];
	if(!(*ptr)) return false;

	// copy the digest
	memcpy(*ptr, m_Digest, 16);

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// GetDigest() : Get the digest in binary format
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::GetDigest(std::string & str)
{
	// we need to have a crc32
	if(!m_bFinished) return false;

	// copy the digest
	str.assign((const char *) m_Digest, 16);

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// GetDigestHex() : Get the digest in hex format
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::GetDigestHex(char **ptr)
{
	// stupid!
	if(!ptr) return false;

	// we need to have a crc32
	if(!m_bFinished) return false;

	// allocate space for the digest
	*ptr = new char[32 + 1];
	if(!(*ptr)) return false;

	// copy the digest
	for(int i = 0; i < 16; ++i)	sprintf((*ptr+(i*2)), "%02x", (int)m_Digest[i]);
	
	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// GetDigestHex() : Get the digest in hex format
//////////////////////////////////////////////////////////////////////////

bool CDigestMd5::GetDigestHex(std::string & str)
{
	// we need to have a crc32
	if(!m_bFinished) return false;

	// format the digest;
	std::stringstream ss;
	for(int i = 0; i < 16; ++i)
	{
		ss << std::hex << std::setw(2) 
		   << std::setfill('0') << (int)m_Digest[i];
	}

	// copy the digest
	str = ss.str();

	// ok
	return true;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
