#ifndef FSM_INCLUDE_H
#define FSM_INCLUDE_H
/*
 * IMPORTANT :
 * Following symbols are mapped to 'enum special' in fsm construction as shown
 *  '^'	-> EPSILON 
 *  '.'	-> DOT
 *  '%' -> SMALL_LETTERS {[a-z]}
 *  '$' -> CAPITAL_LETTERS {[A-Z]}
 *  '#' -> DIGITS
 */

#include <stdlib.h>

enum special 
{	
	NONE = 0,		// No special symbol
	DOT,
	EPSILON,
	CAPITAL_LETTERS,
	SMALL_LETTERS,
	DIGITS
};	

#define MAX_ENUM DIGITS

typedef struct state_link_t
{
	char c;				// The character for which the state link exists
	enum special sym;		// enum special type indicating the special symbol being used like (dot) 
	struct state_t *next_state;		// The state to which it leads
	struct state_link_t *next;		// Another state link
} state_link_t;

typedef struct state_t
{
	char is_final_state;
	state_link_t *links;		// List of links to other states.
} state_t;

typedef struct fsm_t
{
	state_t start_state;
	state_t *accept_state;
	
} fsm_t;

/* Some useful macros */
#define ALLOC_FSM(ptr) ptr = (fsm_t *)   malloc ( sizeof (fsm_t) ); \
		       ptr->accept_state = NULL;
#define ALLOC_STATE(ptr) ptr = (state_t *) malloc ( sizeof (state_t)); \
			 ptr->links = NULL;
#define ALLOC_STATE_LINK(ptr) ptr = (state_link_t *) malloc (sizeof (state_link_t)); \
			      ptr->next_state = NULL;  ptr->next = NULL; 

/* Function to check if a character is a special symbol or not. 
 * It returns the apt 'enum special', NONE (=0) for not a special symbol.
 */
int is_special_symbol (char c);

/* Function to create a transition to a new state. The transition is returned,
 * which has the pointer to the next state. The new state, like all states, also
 * requires pointer to 'parent_fsm'. 
 */
state_link_t * create_transition (char c, enum special sym, state_t *next_state);

/* Function to create a NFA for a single character 
 * Arguments :
 * 	fsm : pointer to resultant fsm.
 *	c   : character
 *	sym : indicates if it is a special symbol
 * Returns 0 on success, non-zero otherwise
 */
int single_char_NFA (fsm_t *fsm, char c, enum special sym);

/* Function to concatenate two NFAs. The result is "fsm1 + fsm2" ['+' represents
 * the concatenate operation, here]
 * Return value is non-negative in case of error, zero in case of success.
 * Resultant NFA is pointed to by fsm1.
 */
int concat_NFA (fsm_t *fsm1, fsm_t *fsm2);

/* Function to repeat ... TODO : Complete this */
int repeat_NFA (fsm_t *fsm1, fsm_t *fsm2);

/* Function to simulate a NFA 
 * It returns the length of the longest match.
 */
int simulate_NFA (fsm_t *fsm, char *str);

/* Function to copy NFA */
void deep_copy (fsm_t *fsm1, fsm_t *fsm2); 
#endif //End of file
