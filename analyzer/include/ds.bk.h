#define NODE constant_table
#define KEY literal

#include "hash.h"

/* N => size of input buffer */
#define N 4096

/* We maintain two buffers of length N, and a lexemebegin and forward pointer
 * to read input from the source program */

char ibuf1[N], ibuf2[N];

char *lexemebegin, *forward;

/* A table of constants is used in addition to the symbol table to store all the literals */

struct ctable
{
	char* literal;
	//#TODO type field
	struct constant_table* next;
};

#undef NODE
#undef KEY
