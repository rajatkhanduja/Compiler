#include <commons.rb.h>

typedef struct hash_entry_linklist
{
	char tokenname[MAXLENtokenname];
	ste *sentry;	//include symbol table struct
	struct hash_entry_linklist *lnext;
}hll;

typedef	struct hash_entry
{
	char hash_alpha;
	//hash_ll* hash_ptr;		
	hll *hnext;
	
}hse;

hse *hashtable[26]; 

//function to create hash table with characters a,b....z and pointers assigned NULL
void hs_initialize();

/* hs_find() finds whether hash index for a particular 
character exists or not. If it doesnt it returns NULL. If it does, 
it returns the pointer of hll to the appropriate symbol table.
*/
hll* hs_find(char*);


/* 
   it calls the hs_find() function to check whether it already exists or not.
   If NULL is returned, means it does not exist,
   and a new node is created.
   hs_insert() inserts a new node hll to the linklist whose head is pointed

by the hash index of the existing hash table.*/
hll* hs_insert(char*);


/* This function returns the first chararcter of the token,
just in case the hash function later changes.

Makes the first character capital for the hash function to check. */
char hs_enhash(char*);

