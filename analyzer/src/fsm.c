/* File that defines the functions required for finite state machines */

#include <fsm.h>

/* Function that creates a link to a new state.
 * Arguments :-
 * 	c    -> character for which the transition has to be defined	
 *	type -> numeric id for any special type of 'c'.
 *
 * The function returns the address of the link (structure) to the new state
 */
struct state_link_t * create_transition (char c, enum special sym)
{
	// Create a new state.
	struct state_t *next_state = (struct state_t*) malloc (sizeof (struct state_t))

	if ( !next_state )
	{
		return (NULL);
	}
	
	// Create a new link structure
	struct state_link_t *link = (struct state_link_t*) malloc (sizeof (struct state_link_t));
	
	if ( !link )
	{
		return(NULL);
	}

	// Set values
	link->c          = c;
	link->sym        = sym;
	link->next_state = next_state;

	return (link);
}
