#ifndef __MANAGER_H__
#define __MANAGER_H__

//////////////////////////////////////////////////////////////////////////
// Ext : Inside new namespace, do not crowd global namespace
//////////////////////////////////////////////////////////////////////////

namespace ext {

//////////////////////////////////////////////////////////////////////////
// manager class
//////////////////////////////////////////////////////////////////////////

template <typename T>
class manager
{
public:
	// destructor
	~manager();

	// adds an item to the database
	void Add(const std::string & name, T *ptr);

	// removes an item (if not needeed anymore)
	void Remove(const std::string & name);

	// removes an item (if not needeed anymore)
	void Remove(T *ptr);

	// finds an item
	T* Find(const std::string & name) const;

	// finds an item
	bool Find(T* ptr, const std::string & name) const;

	// clears the database
	void Clear();

	// returns true if empty
	bool IsEmpty() const { return m_Database.empty(); }

	// returns the number of items
	int Size() const { return m_Database.size(); }

private:
	// database type
	typedef std::map<const std::string, T*> MANAGER_DB;

private:
	// holds all the items
	MANAGER_DB m_Database;
};

//////////////////////////////////////////////////////////////////////////
// manager default destructor
//////////////////////////////////////////////////////////////////////////

template <typename T>
manager<T>::~manager()
{
	// clears all the unfreed items
	Clear();
}

//////////////////////////////////////////////////////////////////////////
// Add() : Adds an item to the database
//////////////////////////////////////////////////////////////////////////

template <typename T>
void manager<T>::Add(const std::string & name, T *ptr)
{
	// insert item into database
	m_Database.insert(std::make_pair(name, ptr));
}

//////////////////////////////////////////////////////////////////////////
// Remove() : Removes an item (if not needeed anymore)
//////////////////////////////////////////////////////////////////////////

template <typename T>
void manager<T>::Remove(const std::string & name)
{
	// removes the item
	m_Database.erase(name);
}

//////////////////////////////////////////////////////////////////////////
// Remove() : Removes an item (if not needeed anymore)
//////////////////////////////////////////////////////////////////////////

template <typename T>
void manager<T>::Remove(T *ptr)
{
	MANAGER_DB::iterator it;

	// find the item by value
	it = ext::find_value(m_Database, ptr);

	// erase "it" if it exists
	if(it != m_Database.end()) m_Database.erase(it);
}

//////////////////////////////////////////////////////////////////////////
// Find() : Finds an item
//////////////////////////////////////////////////////////////////////////

template <typename T>
T* manager<T>::Find(const std::string & name) const
{
	MANAGER_DB::const_iterator it;

	// find it
	it = m_Database.find(name);

	// return the pointer if it exists or NULL
	if(it != m_Database.end()) return it->second;
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Find() : Finds an item
//////////////////////////////////////////////////////////////////////////

template <typename T>
bool manager<T>::Find(T* ptr, const std::string & name) const
{
	MANAGER_DB::const_iterator it;

	// find the item by value
	it = ext::find_value(m_Database, ptr);

	// return the name if we found it
	if(it != m_Database.end())
	{
		name = it->first;
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// Clear() : Clears the database
//////////////////////////////////////////////////////////////////////////

template <typename T>
void manager<T>::Clear()
{
	MANAGER_DB::const_iterator it;

	// if theres nothing to clear we return
	if(m_Database.empty()) return;

	// delete all the items
	for(it = m_Database.begin(); it != m_Database.end(); ++it)
	{
		delete it->second;
	}

	// clear map
	m_Database.clear();
}

//////////////////////////////////////////////////////////////////////////
// Ext : Extended Template Library namespace end
//////////////////////////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
