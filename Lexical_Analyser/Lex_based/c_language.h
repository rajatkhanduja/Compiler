/* Defining macros for states */
#define INSIDE_STRING   0b001

/* Defining and initializing variables */
int nlines = 0;         // Number of lines
int state  = 0;         // Stores the state, such as inside/outside comment/string

/* Defining enum for tokens for keywords and operators */
enum type_of_token { OP, KEYWORD };
typedef enum { BR = 1,
               CONT,
               IF,
               ELSE,
               WHILE, 
               RET,
               INT,
               CHAR,
               FLOAT,
               VOID,
               PLUS,
               MINUS,
               DIV,
               ASTERISK,
               INC,
               DEC,
               ASSIGNMENT,
               EQ,
               GT,
	       LCB,
	       RCB,
	       LB,
	       RB,
	       LSB,
	       RSB,
               NOT
               } token;

#define PRINT_AND_RETURN(STRING,TYPE) \
	if (TYPE == OP)\
		printf (" " #STRING " "); \
	else if (TYPE == KEYWORD)\
		printf (#STRING);\
	return (STRING);
