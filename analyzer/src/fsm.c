/* File that defines the functions required for finite state machines */

#include <fsm.h>
#include <debug.h>
#include <set.h>
#include <assert.h>

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
		state_t *next_state;
		ALLOC_STATE (next_state);

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

	ALLOC_STATE_LINK(tmp);
	
	
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

	assert (fsm->start_state.links = create_transition (c, sym, NULL));

	fsm->accept_state = fsm->start_state.links->next_state;

	if ( !fsm->start_state.links )
	{
		return 1;
	}
	
	return 0;
}


int repeat_NFA (fsm_t *fsm1, fsm_t *fsm2)
{
	// Second argument is just for consistency.
}


int concat_NFA (fsm_t *fsm1, fsm_t *fsm2)
{
	// Link from the first's final state to the start state of the next.
	if (!fsm1->accept_state)
	{
		ALLOC_STATE(fsm1->accept_state);
	}
	link_states ( fsm1->accept_state, &(fsm2->start_state), 'a', EPSILON);
	fsm1->accept_state->is_final_state = 0;
	fsm1->accept_state = fsm2->accept_state;
}

/* Function to check for epsilon transitions from ONE state and add them to the set of 
 * new states. 
 */
int epsilon_state_transitions (state_t *cur_state, tree_t *set_new_states)
{
	state_link_t *link = cur_state->links;
	int count = 0;

	while (link)
	{
		if (EPSILON == link->sym)
		{
			// Add the next_state to the set of states
			insert_element (set_new_states, link->next_state);
			count++;
		}

		link = link->next;
	}

	return count;
}

/* Function to perform epsilon transitions on the entire set using function
 * for states 
 */
void epsilon_set_transitions (tree_t *cur_states)
{
	state_t *state;
	int count = 0;

	tree_t *new_states;
	tree_t *new_states_closure;

	ALLOC_TREE (new_states);

	FOR_EACH (cur_states, state, epsilon_state_transitions (state, new_states));

	do
	{
		ALLOC_TREE (new_states_closure);
		FOR_EACH (new_states, state, epsilon_state_transitions (state, new_states_closure));
		union_sets (cur_states, new_states); // new_states tree is deleted
		new_states = new_states_closure;		
	} while ( new_states_closure->state_addr );
}

/* Function to tell if a character is valid for a special NFA symbols */
int accept (char c, enum special sym)
{
	switch (sym)
	{
		case DOT : 
			return 1;
		case EPSILON : 
			return ( c == special_symbols [EPSILON]) ? 1 : 0;
		case CAPITAL_LETTERS : 
			return ( c >= 'A' && c <= 'Z' ) ? 1 : 0;
		case SMALL_LETTERS : 
			return ( c >= 'a' && c <= 'z' ) ? 1 : 0;
		case DIGITS :
			return ( c >= '0' && c <= '9' ) ? 1 : 0;
		default :
			return 0;
	}
}

/* Function to find new states given initial state and transition character
 * The new states are inserted into the set (tree)
 */
void new_states (state_t *cur_state, char c, tree_t *new_states)
{
	state_link_t *link = cur_state->links;

	while (link)
	{
		if ( ( link->sym != NONE && accept (c, link->sym) )
		   ||
		   ( ( link->sym == NONE )&& (c == link->c) ) )

		{
			insert_element (new_states, link->next_state);
		}

		link = link->next;
	}
}

int simulate_NFA (fsm_t *fsm, char *str)
{
	state_t *state = &(fsm->start_state); 
	unsigned int longest_match = 0;		// Stores the length of the longest match
	unsigned int i = 0;
	tree_t *active_states, *old_states = NULL;
	ALLOC_TREE (active_states);

	epsilon_state_transitions (state, active_states);
	char ch;

	while ( (ch = *(str+i) ) != '\0' )
	{
		if (old_states)
			free(old_states);

		old_states = active_states;
		ALLOC_TREE(active_states);

		// Find set of new states.
		FOR_EACH (old_states, state, new_states(state, ch, active_states));

		// Perform epsilon transitions for all
		epsilon_set_transitions (active_states);

		if ( find (active_states, fsm->accept_state) )
		{
			longest_match = i;
		}
		i++;
	}

	return longest_match;
}
