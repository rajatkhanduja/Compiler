#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLENtokenname	50
#define DEBUG		1

#define	er(e, x)					\
	do						\
	{						\
		perror("ERROR IN: " #e "\n");		\
		fprintf(stderr, "%d\n", x);		\
		exit(-1);				\
	}						\
	while(0)

typedef	struct symbol_table_entry
	{
		int id;
		char tokenname[MAXLENtokenname];
		int tokentype;
		int* linenumber;	// I hope to make it point to an \
					always-sorted array increasing in \
					size dynamically; call the function \
					st_ninstances() to get the number of \
					lines containing the token contained \
					by the symbol table entry.

		struct symbol_table_entry* pnext;
	}
	ste;

