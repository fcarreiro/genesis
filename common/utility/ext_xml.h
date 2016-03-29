#ifndef __EXT_XML_H__
#define __EXT_XML_H__

//////////////////////////////////////////////////////////////////////////
// Ext : Inside new namespace, do not crowd global namespace
//////////////////////////////////////////////////////////////////////////

namespace ext {

//////////////////////////////////////////////////////////////////////////
// xml_parse_file() : Parses a file despite the source being a pack or not
//////////////////////////////////////////////////////////////////////////

inline xmlDocPtr xml_parse_file(const std::string & filename)
{
	int size;
	CFile file;
	char *file_buffer;

	// open file
	file.Open(filename, FILE_AUTO, FILE_BINARY);
	if(!file.IsOpen()) return NULL;

	// allocate memory for the whole file
	size = file.Size();
	file_buffer = new char[size];
	if(!file_buffer) return NULL;

	// read whole xml file
	file.Read(file_buffer, size);

	// close file file
	file.Close();

	// PHASE 2: parse file file
	xmlDocPtr doc = xmlParseMemory(file_buffer, size);

	// free buffer
	delete [] file_buffer;
	return doc;
}

//////////////////////////////////////////////////////////////////////////
// xml_get_string() : Gets a string from a xml node
//////////////////////////////////////////////////////////////////////////

inline std::string xml_get_string(xmlDocPtr doc, xmlNodePtr node, 
								  const std::string & key)
{
	// check all the nodes
	while(node != NULL)
	{
		// compare actual node with requested key
		if(xmlStrcmp(node->name, (const xmlChar *) key.c_str()) == 0)
		{
			xmlChar *xml_key = xmlNodeListGetString(doc, node->children, 1);
			std::string value = (const char *) xml_key;
			xmlFree(xml_key);

			// we found it
			return value;
		}

		// go to next node
		node = node->next;
	}

	// key not found
	return std::string("");
}

//////////////////////////////////////////////////////////////////////////
// xml_get_double() : Gets a double from a xml node
//////////////////////////////////////////////////////////////////////////

inline double xml_get_double(xmlDocPtr doc, xmlNodePtr node, 
							 const std::string & key)
{
	// check all the nodes
	while(node != NULL)
	{
		// compare actual node with requested key
		if(xmlStrcmp(node->name, (const xmlChar *) key.c_str()) == 0)
		{
			xmlChar *xml_key = xmlNodeListGetString(doc, node->children, 1);
			double value = xmlXPathCastStringToNumber(xml_key);
			xmlFree(xml_key);

			// we found it
			return value;
		}

		// go to next node
		node = node->next;
	}

	// key not found
	return 0.0;
}

//////////////////////////////////////////////////////////////////////////
// xml_get_float() : Gets a float from a xml node
//////////////////////////////////////////////////////////////////////////

inline float xml_get_float(xmlDocPtr doc, xmlNodePtr node, 
						   const std::string & key)
{
	return (float) xml_get_double(doc, node, key);
}

//////////////////////////////////////////////////////////////////////////
// xml_get_int() : Gets an integer from a xml node
//////////////////////////////////////////////////////////////////////////

inline int xml_get_int(xmlDocPtr doc, xmlNodePtr node, 
					   const std::string & key)
{
	return (int) xml_get_double(doc, node, key);
}

//////////////////////////////////////////////////////////////////////////
// xml_get_prop_string() : Gets a property string from a xml node
//////////////////////////////////////////////////////////////////////////

inline std::string xml_get_prop_string(xmlDocPtr doc, xmlNodePtr node, 
					const std::string & key, const std::string & prop)
{
	// check all the nodes
	while(node != NULL)
	{
		// compare actual node with requested key
		if(xmlStrcmp(node->name, (const xmlChar *) key.c_str()) == 0)
		{
			xmlChar *xml_key = xmlGetProp(node, (const xmlChar *) prop.c_str());
			std::string value = (const char *) xml_key;
			xmlFree(xml_key);

			// we found it
			return value;
		}

		// go to next node
		node = node->next;
	}

	// key not found
	return std::string("");
}

//////////////////////////////////////////////////////////////////////////
// xml_get_prop_double() : Gets a property double from a xml node
//////////////////////////////////////////////////////////////////////////

inline double xml_get_prop_double(xmlDocPtr doc, xmlNodePtr node, 
				const std::string & key, const std::string & prop)
{
	// check all the nodes
	while(node != NULL)
	{
		// compare actual node with requested key
		if(xmlStrcmp(node->name, (const xmlChar *) key.c_str()) == 0)
		{
			xmlChar *xml_key = xmlGetProp(node, (const xmlChar *) prop.c_str());
			double value = xmlXPathCastStringToNumber(xml_key);
			xmlFree(xml_key);

			// we found it
			return value;
		}

		// go to next node
		node = node->next;
	}

	// key not found
	return 0.0;
}

//////////////////////////////////////////////////////////////////////////
// xml_get_prop_float() : Gets a property float from a xml node
//////////////////////////////////////////////////////////////////////////

inline float xml_get_prop_float(xmlDocPtr doc, xmlNodePtr node, 
			const std::string & key, const std::string & prop)
{
	return (float) xml_get_prop_double(doc, node, key, prop);
}

//////////////////////////////////////////////////////////////////////////
// xml_get_prop_int() : Gets a property integer from a xml node
//////////////////////////////////////////////////////////////////////////

inline int xml_get_prop_int(xmlDocPtr doc, xmlNodePtr node,
		const std::string & key, const std::string & prop)
{
	return (int) xml_get_prop_double(doc, node, key, prop);
}

//////////////////////////////////////////////////////////////////////////
// Ext : Extended Template Library namespace end
//////////////////////////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
