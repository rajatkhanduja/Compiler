#include <hashtable.rxg.h>

void hs_initialize()
{
	int i;
	for(i = 0; i < 26; i++)				//for all the 26 characters
	{
		hashtable[i]->hash_alpha = (char)(65 + i);	//assigns A + i to the array
		hashtable[i]->hnext = NULL;
	}
}


hll* hs_find(char* s)
{
	char hash_char_find = hs_enhash(s);			//returns the first character
	int i;
	for ( i=0; i<26; i++)
	{
		if(hashtable[i]->hash_alpha == hash_char_find)	//finds a match in the hash table s[0]
		{
			hll *head = hashtable[i]->hnext;	//points to the head of link list of hash entry
			while(head!=NULL)			
			{
				if(!strcmp(s, head->tokenname))	//if finds a token match returns the node
				{
					return head;
				}
				else
					head = head->lnext;	//incrementation
			}
		}
	}
	
	return NULL;						//return NULL if no match is found
}

hll* hs_insert(char* s)
{
	char hash_char_insert = hs_enhash(s);			//returns the first character
	int i;
	hll *check = hs_find(s);				//checks if the given token already exists in the ahsh table or not
	if(check == NULL)				
	{
		hll *temp = malloc ( sizeof(hll) );		// create a new node with that token name
		temp->sentry = NULL;				
		temp->lnext = NULL;
		strcpy(temp->tokenname, s);			
		
		for ( i=0; i<26; i++)
		{
			if(hashtable[i]->hash_alpha == hash_char_insert)	// finds match in the hash table s[0]
			{
				hll *head = hashtable[i]->hnext;		// points to the head of link list of hash entry
				if(head == NULL)
				{
					head = temp;				// first entry to the link list added as node
				}
				else
				{
					while(head->lnext!=NULL)
						head = head->lnext;		
					head = temp;				// last entry in the link list added as node
				}
				return head;					// return node
				
			}
		}
	}
	else
		return check;					//if token exists the node pointing to the token obtained via hs_find() is returned. 

}

char hs_enhash(char* s)
{	
	if(islower(s[0]))					// if the first character of the token is lowercase, it is converted to upppercase.
		toupper(s[0]);
	return s[0];
}

