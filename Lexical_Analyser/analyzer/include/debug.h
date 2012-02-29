/* This program defines some functions and macros to help in debugging programs. */

#include <assert.h>

#ifdef DEBUG_MODE

#include <stdio.h>
#include <time.h>

FILE * error_file;
time_t raw_time;
struct tm * timeinfo;

#define DEBUG_INIT(filename) \
do \
{\
   error_file = fopen (filename, 'a'); \
   time (&raw_time); timeinfo = localtime (&raw_time); \
   fprintf (error_file, "Beginning logging at %s\n", asctime (timeinfo));\
} while (0)


#define CHECK_CONDITION(EXPR,STR) assert (EXPR)	// Checks condition and exits program if false
#define DEBUG_MSG(MSG)
#else

#include <stdio.h>

/* The following checks for a condition and if false, prints an error, but doesn't exit */
#define CHECK_CONDITION(EXPR,STR) if (! (EXPR) ) { fprintf (stderr, "Error on line %d in %s: " #STR "\n", __LINE__, __FILE__);}
#define DEBUG_INIT(filename) do {} while (0)
#define DEBUG_MSG(filename)  do {} while (0)

#endif	
// End of file
