#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memmgr.h"

//////////////////////////////////////////////////////////////////////////
// No macros
//////////////////////////////////////////////////////////////////////////

#ifdef	new
#undef	new
#endif

#ifdef	delete
#undef	delete
#endif

//////////////////////////////////////////////////////////////////////////
// Allocation structure
//////////////////////////////////////////////////////////////////////////

typedef struct TAllocation {
	void		*address;
	char		file[256];
	int			line;
	size_t		size;
	TAllocation	*next;
} TAllocation;

//////////////////////////////////////////////////////////////////////////
// Allocations Database
//////////////////////////////////////////////////////////////////////////

int				alloc_line;
size_t			alloc_size;
char			alloc_file[256];
bool			checking = false;
unsigned long	max_alloc = 0;
unsigned long	current_alloc = 0;
TAllocation		dbAllocations = {0};

//////////////////////////////////////////////////////////////////////////
// Overriden "new"
//////////////////////////////////////////////////////////////////////////

void* operator new(size_t size)
{
	void *address = malloc(size);
	TAllocation *ptr = &dbAllocations;

	// if we are not checking dont add it to the list
	if(!checking) return address;

	// find last one
	while(ptr->next != NULL) ptr = ptr->next;

	// fill last item with info
	ptr->address = address;
	ptr->line = alloc_line;
	ptr->size = size;
	ptr->next = NULL;
	strcpy(ptr->file, alloc_file);

	// allocate another item
	ptr->next = (TAllocation *) malloc(sizeof(TAllocation));
	memset(ptr->next, 0, sizeof(TAllocation));

	// add to allocation count
	current_alloc += size;
	if(current_alloc > max_alloc) max_alloc = current_alloc;

	// return allocated mem
	return address;
}

//////////////////////////////////////////////////////////////////////////
// Overriden "new []"
//////////////////////////////////////////////////////////////////////////

void* operator new[] (size_t size)
{
	void *address = malloc(size);
	TAllocation *ptr = &dbAllocations;

	// if we are not checking dont add it to the list
	if(!checking) return address;

	// find last one
	while(ptr->next != NULL) ptr = ptr->next;

	// fill last item with info
	ptr->address = address;
	ptr->line = alloc_line;
	ptr->size = size;
	ptr->next = NULL;
	strcpy(ptr->file, alloc_file);

	// allocate another item
	ptr->next = (TAllocation *) malloc(sizeof(TAllocation));
	memset(ptr->next, 0, sizeof(TAllocation));

	// add to allocation count
	current_alloc += size;
	if(current_alloc > max_alloc) max_alloc = current_alloc;

	// return allocated mem
	return address;
}

//////////////////////////////////////////////////////////////////////////
// Overriden "delete"
//////////////////////////////////////////////////////////////////////////

void operator delete (void *p)
{
	// check for "delete NULL"
	if(p == NULL) return;

	TAllocation *ptr = &dbAllocations;
	TAllocation *last_ptr = NULL;

	// find by address
	for(;;)
	{
		// if we find it
		if(ptr->address == p)
		{
			// save next list
			TAllocation *next = ptr->next;

			// remove from current allocation
			current_alloc -= ptr->size;

			// free mem
			free(ptr->address);

			// free db item
			if(ptr != &dbAllocations) free(ptr);
			else memset(ptr, 0, sizeof(TAllocation));

			// set the last to the next
			if(last_ptr) last_ptr->next = next;

			// as we have successfully deleted everything we return
			return;
		}

		// go on to the next one
		if(ptr->next != NULL)
		{
			last_ptr = ptr;
			ptr = ptr->next;
		}
		else
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Overriden "delete []"
//////////////////////////////////////////////////////////////////////////

void operator delete[] (void *p)
{
	// check for "delete NULL"
	if(p == NULL) return;

	TAllocation *ptr = &dbAllocations;
	TAllocation *last_ptr = NULL;

	// find by address
	for(;;)
	{
		// if we find it
		if(ptr->address == p)
		{
			// save next list
			TAllocation *next = ptr->next;

			// remove from current allocation
			current_alloc -= ptr->size;

			// free mem
			free(ptr->address);

			// free db item
			if(ptr != &dbAllocations) free(ptr);
			else memset(ptr, 0, sizeof(TAllocation));

			// set the last to the next
			if(last_ptr) last_ptr->next = next;

			// as we have successfully deleted everything we return
			return;
		}

		// go on to the next one
		if(ptr->next != NULL)
		{
			last_ptr = ptr;
			ptr = ptr->next;
		}
		else
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Tracing routine for "new, new []"
//////////////////////////////////////////////////////////////////////////

void do_new(const char *file, const int line)
{
	alloc_line = line;
	strcpy(alloc_file, file);
}

//////////////////////////////////////////////////////////////////////////
// Tracing routine for "delete, delete []"
//////////////////////////////////////////////////////////////////////////

void do_delete(const char *file, const int line)
{
	alloc_line = 0;
	memset(alloc_file, 0, sizeof(alloc_file));
}

//////////////////////////////////////////////////////////////////////////
// Checking routines
//////////////////////////////////////////////////////////////////////////

void leak_check_start()
{
	// start checking
	checking = true;
}

void leak_check_end()
{
	FILE *f;
	int k = 0, u = 0;
	TAllocation *ptr;

	// not checking anymore
	checking = false;

	// open leak log
	f = fopen("leak.log", "w");
	if(!f) return;

	// info header
	fprintf(f, "--------------------------------------------------------------------\n");
	fprintf(f, " "__DATE__ " " __TIME__ " : Memory leak report:\n");
	fprintf(f, "--------------------------------------------------------------------\n\n");
	fprintf(f, "Maximum memory allocation: %u bytes.\n\n", max_alloc);
	fprintf(f, "Leak check started...\n");

	// dump leaks
	for(ptr = &dbAllocations; ptr != NULL; ptr = ptr->next)
	{
		if(ptr->line != 0)
		{
			// dump record
			fprintf(f, "! [%#010x] %s (%i) : %i bytes.\n", ptr->address, ptr->file, ptr->line, ptr->size);
			k++;
		}
		else if(ptr->address != NULL)
		{
			fprintf(f, "~ [%#010x] <unknown> : %i bytes.\n", ptr->address, ptr->size);
			u++;
		}
	}

	// brief
	fprintf(f, "Leak check finished...\n\n");
	fprintf(f, "--------------------------------------------------------------------\n");
	fprintf(f, " %i memory leaks found!\n %i possible real leaks, %i possible unallocated globals.\n", k + u, k, u);
	fprintf(f, "--------------------------------------------------------------------\n\n");

	// close log
	fclose(f);
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////
