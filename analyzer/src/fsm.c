/* File that defines the functions required for finite state machines */

#include <fsm.h>
#include <debug.h>

char special_symbols[] = { 0, '.', '^', '$', '%', '#' };

int is_special_symbol (char c)
{
	enum special t;

	for ( t = MAX_ENUM; t > NONE; t--)
	{
		if ( c == special_symbols[t] )
		return (t);
	}

	return NONE;
}

/* Function that creates a link to a new state.
 * Arguments :-
 * 	c    -> character for which the transition has to be defined	
 *	type -> numeric id for any special type of 'c'.
 *	next_state -> pointer to the state to be linked to. If NULL, new space 
 *		      allocated
 *
 * The function returns the address of the link (structure) to the new state
 */
state_link_t * create_transition (char c, enum special sym, state_t *next_state)
{
	// Create a new state.
	if ( next_state == NULL )
	{
		state_t *next_state = ALLOC_STATE ();

		if ( !next_state )
		{
			// Unable to allocate space
			CHECK_CONDITION(next_state, "Unable to allocate spacen\n");
			return (NULL);
		}
	}
	

	
	// Create a new link structure
	state_link_t *link = (state_link_t*) malloc (sizeof (state_link_t));
	
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

/* Function to link to given states 
 * Arguments :-
 *	state1 -> first among the two states
 *	state2 -> first state is linked to this state
 * 	c      -> character for the transition
 *	sym    -> special symbol
 *	
 * Return non-zero for failure and zero for success
 */
int link_states (state_t *state1, state_t *state2, char c, enum special sym)
{
	state_link_t *tmp = state1->links;

	while ( tmp )
	{
		tmp = tmp->next;
	}

	tmp = ALLOC_STATE_LINK();
	
	if (!tmp)
	{
		// Space not allocated
		return 1;
	}

	/* Set the values */
	tmp->next = NULL;
	tmp->next_state = state2;
	tmp->c = c;
	tmp->sym = sym;
}

int single_char_NFA (fsm_t *fsm, char c, enum special sym)
{
	if (!fsm)
	{
		return -1;
	}

	fsm->start_state.links = create_transition (c, sym, NULL);

	if ( !fsm->start_state.links )
	{
		return 1;
	}
	
	return 0;
}

int concat_NFA (fsm_t *fsm1, fsm_t *fsm2)
{
	CHECK_CONDITION ( (fsm1->accept_state->links == NULL), "Accept state non-zero");	// for debugging

	// Link from the first's final state to the start state of the next.
	link_states ( fsm1->accept_state, &(fsm2->start_state), 'a', EPSILON);
	fsm1->accept_state->is_final_state = 0;
	fsm1->accept_state = fsm2->accept_state;
}

int simulate_NFA (fsm_table_t *fsm_table, char *str)
{
	
}
