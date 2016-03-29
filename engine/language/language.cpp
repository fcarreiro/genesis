#include "../precompiled/stdafx.h"
#include "../file_io/file_base.h"
#include "../file_io/package.h"
#include "../file_io/standard.h"
#include "../file_io/file.h"
#include "language.h"
#include "../../common/utility/ext_xml.h"

//////////////////////////////////////////////////////////////////////////
// CLanguagePack default constructor & destructor
//////////////////////////////////////////////////////////////////////////

CLanguagePack::CLanguagePack()
{
}

CLanguagePack::~CLanguagePack()
{
	// free just in case we didnt do it before
	Free();
}

//////////////////////////////////////////////////////////////////////////
// CLanguagePack overloaded constructor
//////////////////////////////////////////////////////////////////////////

CLanguagePack::CLanguagePack(const std::string & strFilename)
{
	// load the file
	Load(strFilename);
}

//////////////////////////////////////////////////////////////////////////
// Load() : Loads the language pack into the database map
//////////////////////////////////////////////////////////////////////////

bool CLanguagePack::Load(const std::string & strFilename)
{
	xmlDocPtr	doc;
	xmlNodePtr	node;

	// destroy previous data
	Free();

	// open and parse options file
	doc = ext::xml_parse_file(strFilename);
	if(!doc) return false;

	// get root node
	node = xmlDocGetRootElement(doc);
	if(!node)
	{
		xmlFreeDoc(doc);
		return false;
	}

	// check for options file
	if(xmlStrcmp(node->name, (const xmlChar *) "language_pack") != 0)
	{
		xmlFreeDoc(doc);
		return false;
	}

	// set to first sub-node
	node = node->children->next;

	// start reading options
	while(node != NULL)
	{
		// we dont want the text inside
		if(xmlStrcmp(node->name, (const xmlChar *) "text") != 0)
		{
			// get option type
			xmlChar *p_id = xmlGetProp(node, (const xmlChar *) "id");
			int id = (int) xmlXPathCastStringToNumber(p_id);
			xmlFree(p_id);

			xmlChar *key = xmlNodeListGetString(doc, node->children, 1);
			m_Strings.insert(std::make_pair((language_string) id, (const char *) key));
			xmlFree(key);
		}

		// go to the next node
		node = node->next;
	}

	// close options file
	xmlFreeDoc(doc);

	// no errors
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Free() : Destroys all the database data
//////////////////////////////////////////////////////////////////////////

void CLanguagePack::Free()
{
	// free the map
	m_Strings.clear();
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
