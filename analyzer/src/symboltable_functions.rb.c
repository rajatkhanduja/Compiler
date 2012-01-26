#include <symboltable.rb.h>

int st_ninstances(ste* e)
{
	int i;
	for(i = 0; e->linenumber[i] != -1; i++)
		;
	return i;
}

ste* st_find(char* t)
{
	hll* p = hs_find(t);
	if(p)
		return p->sentry;
	else
		return NULL;
}

inline static void st_link(hll* n, ste* e)
{
	n->sentry = e;
}

ste* st_insert(ste* const h, char* t, int y, int l)
{
	ste* e;
	hll* n = hs_insert(t);
	if(n->sentry)
	{
		e = n->sentry;
		int lenlinenumber = st_ninstances(e);
		int temp[lenlinenumber + 1];
		memcpy(temp, e->linenumber, (size_t) lenlinenumber);
		temp[lenlinenumber - 1] = y;
		temp[lenlinenumber] = -1;
		e->linenumber = temp;

		return h;
	}
	else
	{
		e = (ste*) malloc(sizeof(ste));
		e->id = h->id + 1;
		strcpy(e->tokenname, t);
		e->tokentype = y;
		int temp[2];
		temp[0] = y;
		temp[1] = -1;
		e->linenumber = temp;
		e->pnext = h;
		
		st_link(n, e);

		return e;
	}
}

inline void st_printentry(ste* e, FILE* f)
{
	fprintf(f,
		"SYMBOL TABLE ENTRY\n\
		id : %d\n\
		tokenname : %s\n\
		tokentype : %d\
		linenumber : ",
		e->id,
		e->tokenname,
		e->tokentype);
	int i, n;
	for(i = 0; (n = e->linenumber[i]) != -1; i++)
		fprintf(f, "%d, ", n);
	fprintf(f, "-1\n");	
}

void st_printtable(ste* const h, FILE* f)
{
	fprintf(f, "\n### SYMBOL TABLE ###\n");
	ste* e = h;
	do
		st_printentry(e, f);
	while(e = h->pnext);
}

