#include <stdio.h>
#include <debug.h>
#include <fsm.h>
#include <regex_parser.h>

int main (int argc, char *argv[])
{	

	char input[100];
	char postfix[100];

	while ( 1 )
	{
		scanf ("%s", input);

		infix2postfix (input, postfix);

		printf ("postfix_string is --%s--\n", postfix);
	}

}
