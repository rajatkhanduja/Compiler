/* This file contains the functions required to convert regular expressions to fsm 
 * Author : Rajat Khanduja
 */

#include <regex_to_fsm.h>
#include <regex_parser.h>



// Stack of fsm
fsm_t stack_fsm[MAX_STATES];
int stack_top = -1;

#define PUSH(val) \
{\
	stack_fsm[stack_top++] = val; \
\}

#define POP() (stack_fsm[stack_top--]);
	

/* The order is determined by enum operation (in regex_parser.h) */
int (*op_functions[]) (fsm_t *, fsm_t *)= 
{
	&concat_NFA
};



unsigned int create_NFA (fsm_t *fsm, char *regular_expression)
{
	char final_regex[MAX_STATES];

	// Convert to postfix.
	infix2postfix (regular_expression, final_regex);

	fsm_t *tmp = NULL;

	while ( *final_regex )
	{
		// Create a NFA for a character transition and push to stack.
		tmp = ALLOC_FSM();
	}
}
