/* This program defines some functions and macros to help in debugging programs. */

#ifdef DEBUG_MODE

#include <assert.h>

#define CHECK_CONDITION(EXPR,STR) assert (EXPR)	// Checks condition and exits program if false

#else

#include <stdio.h>

/* The following checks for a condition and if false, prints an error, but doesn't exit */
#define CHECK_CONDITION(EXPR,STR) if (! (EXPR) ) { fprintf (stderr, "Error on line %s in %s\n" #STR, __LINE__, __FILE__);}

#endif	
// End of file
