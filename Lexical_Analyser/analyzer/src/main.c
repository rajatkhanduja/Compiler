#include <stdio.h>
#include <debug.h>
#include <string.h>
#include <fsm.h>
#include <regex_to_fsm.h>

/* Arguments :-
 * 	arg1 : <tokens_list_file> 
 *	arg2 : <input_file>
 */

#define MAX_TOKENS 20
#define MAX_TOKEN_LENGTH 50
#define MAX_LINE_LENGTH 100
#define MAX_FILENAME 50

char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];
static int n_tokens = 0;
fsm_t token_NFA[MAX_TOKENS];

void read_tokens (FILE *fp);


int main (int argc, char *argv[])
{	
	// Initiate debugging.
	DEBUG_INIT ("debug_log");

	char token_filename[MAX_FILENAME];

	strncpy (token_filename, argv[1], MAX_FILENAME);

	FILE * fp ;
	fp = fopen (token_filename, "r");

	if ( !fp )
	{
		perror ("fopen : ");
		exit(3);
	}
	fprintf (stderr, "Going to tokenize and form NFAs\n");

	// Reading tokens
	read_tokens (fp);

	int i = 0;

	for (i = 0; i < n_tokens; i++)
	{	
		fprintf (stderr, "simulating %d dfa\n", i);
		printf ("%d\n", simulate_NFA (&token_NFA[i], "bc"));
		printf ("%d\n", simulate_NFA (&token_NFA[i], "abc"));
	}

	printf ("final i is %d\n", i);

	return 0;


}

void read_tokens (FILE *fp)
{
	char line[MAX_LINE_LENGTH];
	char *token, *regex;
	int n_token_ = 0;
	while ( NULL != fgets (line, MAX_LINE_LENGTH - 1, fp) )
	{
		regex = strtok (line, "\t\n");
	
		fprintf (stderr, "REGEX : %s\n", regex);
		
		// Get the token name 
		token = strtok (NULL, "\t\n");
		fprintf (stderr, "TOKEN : %s\n",  token);
		
		strncpy (tokens[n_token_], regex, MAX_TOKEN_LENGTH);

		fprintf (stderr, "About to generate NFA\n", token);
		// Generate NFA.
		create_NFA ( &token_NFA[n_token_], regex);
		fprintf (stderr, "Generated NFA\n", token);
		assert (token_NFA[n_token_].start_state.links);
		n_token_++;
	}
	n_tokens = n_token_;	// Strangely this has to be used. The global variable is accessed 
				// and modified at unimaginable parts of the program 
}
