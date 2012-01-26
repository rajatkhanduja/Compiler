#ifndef FSM_INCLUDE_H
#define FSM_INCLUDE_H

#include <queue.h>		// For list of states that follow

struct state_link_t
{
	char c;		// The character for which the state link exists
	struct state_t *next_state;	// The state to which it leads
	struct state_link_t *next;	// Another state link
};

struct state_t
{
	char isFinalState;
	struct state_link_t *links;	// List of links to other states.
};

struct fsm_t
{
	struct state_t start_state;
};

#endif //End of file
