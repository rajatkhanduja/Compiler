#include <stdlib.h>
#include <string.h>
#include "stable.h"
#include "token.h"

struct NODE_hash_table 
{
	struct NODE **start; /* NODE is a macro used to customize hash_table */
	size_t size;
};

int NODE_find_entry (struct NODE_hash_table *htable, char *key, size_t size )
{
	/* Get an index for the key */
	size_t index = hash (key) % htable->size;

	if ( htable->start[index] != NULL ) 
	{
		/* Find the matching key in the chain, if any */
		struct NODE *it = htable->start[index];

		while ( it != NULL ) 
		{
			if ( strcmp(key,it->KEY) == 0 ) /* KEY is a macro */
				return 1;

			it = it->next;
		}
	}

	return 0;
}


struct NODE* NODE_installID (struct NODE **htable, char *key)
{
	struct NODE  *it;

	/* Get an index for the key */
	size_t index = hash (key) % (*htable)->size;

	
	/* Search for any duplicate keys */
	it = (*htable)->start[index];

	while ( it != NULL ) 
	{
		if ( strcmp(key,it->KEY) == 0 ) 
		{
			/* Stop the insertion if a duplicate is found */
			return it;
		}

		it = it->next;
	}
	

	/* Create a new node for the chain */
	it = malloc ( sizeof *it );

	if ( it == NULL )
  		return 0;

	/* Attach the new node to the front of the chain */
	strcpy(it->KEY,key);
	it->next = (*htable)->start[next];
	(*htable)->start[index] = it;

	return it;
}


#ifndef HASH_FUNC
#define HASH_FUNC
unsigned hash(char* string)
{       
	unsigned int index = 0; char* it = string;
	while ( *it++ != '\0' )
    	{       
        	index += atoi(*(it - 1));
      	}       
	return index;
}
#endif
