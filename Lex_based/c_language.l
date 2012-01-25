%{
#include "c_language.h"
%}

%%
 /* Delimiters */
\n			{
				nlines++;
				printf ("\n");
	        	} 

 /* Comments */
"/*".*"*/"		/* Do nothing. */


 /* Operators */
[ \t\n]*"+"[ \t\n]*	{
				PRINT_AND_RETURN (PLUS, OP);
			}
[ \t\n]*"-"[ \t\n]*	{
				PRINT_AND_RETURN (MINUS, OP);
			}
[ \t\n]*"*"[ \t\n]*	{
				PRINT_AND_RETURN (ASTERISK, OP);
			}
[ \t\n]*"/"[ \t\n]*	{
				PRINT_AND_RETURN (DIV, OP);
			}
[ \t\n]*"++"[ \t\n]*	{ 
				PRINT_AND_RETURN (INC, OP);
			}
[ \t\n]*"--"[ \t\n]*	{ 
				PRINT_AND_RETURN (DEC, OP);
			}
[ \t\n]*"="[ \t\n]*	{ 	
				PRINT_AND_RETURN (ASSIGNMENT, OP);
			}
[ \t\n]*"=="[ \t\n]*	{	
				PRINT_AND_RETURN (EQ, OP);
			}
[ \t\n]*">"[ \t\n]*	{
				PRINT_AND_RETURN (GT, OP);
			}
"!"[ \t\n]*	        { 
				PRINT_AND_RETURN (NOT, OP);
			}

 /* Brackets */
[ \t\n]*"{"[ \t\n]*	{
				PRINT_AND_RETURN (LCB, OP);
			}
			
 /* Brackets */
[ \t\n]*"}"[ \t\n]*	{
				PRINT_AND_RETURN (RCB, OP);
			}
			
 /* Brackets */
[ \t\n]*"["[ \t\n]*	{
				PRINT_AND_RETURN (LSB, OP);
			}
			
 /* Brackets */
[ \t\n]*"]"[ \t\n]*	{
				PRINT_AND_RETURN (RSB, OP);
			}

 /* Brackets */
[ \t\n]*"("[ \t\n]*	{
				PRINT_AND_RETURN (LB, OP);
			}

 /* Brackets */
[ \t\n]*")"[ \t\n]*	{
				PRINT_AND_RETURN (RB, OP);
			}

 /* Keywords */
break			{ 
				PRINT_AND_RETURN (BR, KEYWORD);
			}
continue		{
				PRINT_AND_RETURN (CONT, KEYWORD);
			}
if			{
				PRINT_AND_RETURN (IF, KEYWORD);
			}
else			{
				PRINT_AND_RETURN (ELSE, KEYWORD);
			}
while 			{
			 	PRINT_AND_RETURN (WHILE, KEYWORD);
			}
int			{
				PRINT_AND_RETURN (INT, KEYWORD);
			}
char			{
		        	PRINT_AND_RETURN (CHAR, KEYWORD);
			}
float			{
				PRINT_AND_RETURN (FLOAT, KEYWORD);
			}       
void			{
                        	PRINT_AND_RETURN (VOID, KEYWORD);
			}
return 			{
				PRINT_AND_RETURN (RET, KEYWORD);
			}

 /* Identifier */
[a-zA-Z_][_a-zA-Z0-9]*	{
				printf ("ID");
			}

 /* Numeric Constants */
(\+|\-|"")[0-9]*	{
				printf ("CONST_INT");
			}
(\+|\-|"")[0-9]*\.[0-9]* {
				printf ("CONST_FLOAT");
			}
 /* Literals */
\".*\"			{
				printf ("STR");
				// Simply print the entire string
			//	printf ("%s", yytext);	
			}
\'.\'			{
				printf ("LIT");
			}
%%        
