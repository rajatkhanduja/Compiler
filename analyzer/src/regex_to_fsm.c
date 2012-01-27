/* This file contains the functions required to convert regular expressions to fsm 
 * Author : Rajat Khanduja
 */

#include <regex_to_fsm.h>
#include <regex_parser.h>



// Stack of fsm
fsm_t stack_fsm[MAX_STATES];
int stack_top = 0;

#define PUSH(val) \
{\
	stack_fsm[stack_top++] = val; \
\}

#define POP() (stack_fsm[--stack_top]);

char NFA_operators[] = { '*', '@', '|' };



/* The order is determined by enum operation (in regex_parser.h) */
int (*op_functions[]) (fsm_t *, fsm_t *) = 
{
	&repeat_NFA,
	&concat_NFA
};



unsigned int create_NFA (fsm_t *fsm, char *regular_expression)
{
	char final_regex[MAX_STATES];
	
	// Convert to postfix.
	infix2postfix (regular_expression, final_regex);

	fsm_t *tmp = NULL;
	char ch, ch_prev;
	int (*func_ptr) (fsm_t*, fsm_t*) = NULL;
	fsm_t tmp_fsm1, tmp_fsm2;

	int op, i = 0;
	#define CREATE_SINGLE_CHAR_NFA_AND_PUSH(_c, _sym) \
	{\
		fprintf (stderr, "Creating single state\n");\
		single_char_NFA (&stack_fsm[stack_top], _c, _sym);\
		stack_top++;\
	}
	
	fprintf (stderr, "Regular expression changed to NFA format.\n");
	
	ch_prev = 'a';
	while ( (ch = final_regex[i++]) != '\0' )
	{
		fprintf (stderr, "Reading generated regex\n");
		if ( ch == '\\')
		{
			if ( ch_prev == '\\' )
			{
				CREATE_SINGLE_CHAR_NFA_AND_PUSH (ch, NONE);
			}
			else
			{
				ch_prev = ch;
				continue;
			}
		}
		else if ( (op = is_operator (ch) ) > -1)
		{
			CHECK_CONDITION ( op < LB, "Error with operator\n" );
			if ( ch_prev == '\\' )
			{
				// Escaped character and not special
				CREATE_SINGLE_CHAR_NFA_AND_PUSH (ch, NONE);
			}
			else
			{
				// OPERATOR
				if ( op == STAR )
				{
					// TODO : IMPLEMENT THIS
					//repeat (&stack_fsm[stack_top - 1], NULL); 	
				}
				else
				{
					fprintf (stderr, "ASFASD\n");
					tmp_fsm1 = POP();
					tmp_fsm2 = POP();
					//fn_ptr = (int *) NFA_operators[op];
					func_ptr = op_functions[op];
					fprintf (stderr, "1231adsfASFASD\n");
					(*func_ptr)(&tmp_fsm1, &tmp_fsm2);
				}
			}

		}
		else
		{
			// TODO :: Add more if-else
			CREATE_SINGLE_CHAR_NFA_AND_PUSH (ch, NONE);
		}

		ch_prev = ch;
	}
}
