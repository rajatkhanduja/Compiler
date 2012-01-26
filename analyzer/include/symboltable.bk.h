#define NODE stable_entry
#define KEY lexeme

#include "hash.h"

enum flag {N,Y};

struct position
{
	int ln; /* line number */
	int col; /* column */
};


/* The symbol table needs to be hashed based on the lexeme value to
 * enable fast searching across it. */

struct stable_entry
{
//#TODO type field
	struct position pos;
	char* lexeme;
	flag key_or_id;
	char* token_name;
	struct stable_entry *next;
};

struct stable; /* Implemented as a hash table */

/* Token => <Token_Name, Token_Attr>  Token_Attr is a pointer to an entry     
 * in the symbol table. */                                             
void getToken(struct token* TOKEN, struct NODE* lexeme)                   
{							       
	strcpy(TOKEN->name,NODE->token_name);
	TOKEN->attr = lexeme;                                            
}

#undef NODE
#undef KEY
