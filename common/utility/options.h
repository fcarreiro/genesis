#ifndef __OPTIONS_H__
#define __OPTIONS_H__

//////////////////////////////////////////////////////////////////////////
// Options type
//////////////////////////////////////////////////////////////////////////

enum TOptionType {
	OPT_NONE,
	OPT_INT,
	OPT_STRING,
	OPT_FLOAT
};

//////////////////////////////////////////////////////////////////////////
// COption class
//////////////////////////////////////////////////////////////////////////

class COption
{
public:
	// default constr
	COption()
	{
		m_ubType = OPT_NONE;
		m_pPtr = NULL;
	}

	// string constr
	COption(const std::string & str)
	{
		m_ubType = OPT_STRING;
		m_pPtr = new std::string;
		if(!m_pPtr) return;
		*((std::string *)m_pPtr) = str;
	}

	// int constr
	COption(int i)
	{
		m_ubType = OPT_INT;
		m_pPtr = new int;
		if(!m_pPtr) return;
		*((int *)m_pPtr) = i;
	}

	// float constr
	COption(float f)
	{
		m_ubType = OPT_FLOAT;
		m_pPtr = new float;
		if(!m_pPtr) return;
		*((float *)m_pPtr) = f;
	}

	// destructor
	~COption()
	{
		Free();
	}

public:
	// set string option
	void SetString(const std::string & str)
	{
		m_ubType = OPT_STRING;
		delete m_pPtr;
		m_pPtr = new std::string;
		if(!m_pPtr) return;
		*((std::string *)m_pPtr) = str;
	}

	// set int option
	void SetInt(int i)
	{
		m_ubType = OPT_INT;
		delete m_pPtr;
		m_pPtr = new int;
		if(!m_pPtr) return;
		*((int *)m_pPtr) = i;
	}

	// set float option
	void SetFloat(float f)
	{
		m_ubType = OPT_FLOAT;
		delete m_pPtr;
		m_pPtr = new float;
		if(!m_pPtr) return;
		*((float *)m_pPtr) = f;
	}

	// free
	void Free()
	{
		// delete
		if(m_ubType == OPT_STRING) delete (std::string *) m_pPtr;
		if(m_ubType == OPT_INT) delete (int *) m_pPtr;
		if(m_ubType == OPT_FLOAT) delete (float *) m_pPtr;

		// reset
		m_pPtr = NULL;
		m_ubType = OPT_NONE;
	}

	// get string
	std::string GetString() const {
		if(m_pPtr && m_ubType == OPT_STRING)
		return *((std::string *) m_pPtr);
		else return std::string(""); }

	// get int
	int GetInt() const {
		if(m_pPtr && m_ubType == OPT_INT)
		return *((int *) m_pPtr);
		else return 0; }

	// get float
	float GetFloat() const {
		if(m_pPtr && m_ubType == OPT_FLOAT)
		return *((float *) m_pPtr);
		else return 0.0f; }

private:
	// variable pointer
	void *m_pPtr;

	// type pointer
	TOptionType m_ubType;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
