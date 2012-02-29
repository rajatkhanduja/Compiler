#include <hashtable.rxg.h>

void hs_initialize(void)
{
	int i;
	for(i = 0; i < LENaz; i++)
		hashtable[i] = NULL;
}

char hs_enhash(char* s)
{	
	char c = s[0];
	// add error check for non-characters
	if(islower(c))
		toupper(c);
	return c - 'A';		// returns the corresponding index of the hashtable.
}

hll* hs_find(char* s)
{
	hll* p = hashtable[hs_enhash(s)];
	while(p)
	{
		if(!strcmp(s, p->tokenname))
			return p;
		p = p->lnext;
	}
	return NULL;
}

hll* hs_insert(char* s)
{
	hll *p = hs_find(s);
	if(!p)
	{
		hll* q = hashtable[hs_enhash(s)];
		p = (hll*) malloc(sizeof(hll));
		p->sentry = NULL;				
		p->lnext = NULL;
		strcpy(p->tokenname, s);			
		
		while(!q->lnext)
			q = q->lnext;		
		
		q->lnext = p;
	}
	return p;
}

