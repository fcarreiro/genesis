#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

//////////////////////////////////////////////////////////////////////////
// Language string codes
//////////////////////////////////////////////////////////////////////////

enum language_string
{
	STR_CONNECT,
	STR_CONNECT_TO_SERVER,
	STR_DISCONNECT,
	STR_CREATE_CHARACTER,
	STR_DELETE_CHARACTER,
	STR_OPTIONS,
	STR_CREDITS,
	STR_EXIT,
	STR_GO_BACK,
	STR_USERNAME,
	STR_PASSWORD,
	STR_LAST
};

//////////////////////////////////////////////////////////////////////////
// CLanguagePack class
//////////////////////////////////////////////////////////////////////////

class CLanguagePack
{
public:
	// default constructor & destructor
	CLanguagePack();
	~CLanguagePack();

	// overloaded open constructor
	CLanguagePack(const std::string & strFilename);

public:
	// load language pack
	bool Load(const std::string & strFilename);

	// free language pack
	void Free();

	// returns the requested string
	std::string GetString(language_string id) { return m_Strings[id]; }

	// returns true if we have sth loaded
	bool IsLoaded() const { return (m_Strings.size() != 0); }

private:
	// database typedef
	typedef std::map<int, std::string> STRING_DB;

private:
	// language database
	STRING_DB m_Strings;
};

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
