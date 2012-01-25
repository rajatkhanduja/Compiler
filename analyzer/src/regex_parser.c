/* This file defines the functions required to parse regular expressions */

#include <stdio.h>		// for showing error message
#include <regex_parser.h>
#include <assert.h>

/* Supported REGEX operations with priority. Lower the 'value', higher the priority */
enum operation		
{
	STAR,
	CONCAT,
	OR,
	LB,
	RB
	// Keep LB and RB at the lowest priority
};

/* Define literals for the operators */
char operators[] = { '*', '@', '|', '(', ')' };


/* Two stacks for postfix notation */
static char symbol_stack[MAX_REGEX_LEN];
static int symbol_stack_top;		// Indicates the top most element
static char op_stack[MAX_REGEX_LEN];
static int op_stack_top;		// Indicates the top most element

/* This function returns the enum operation for the character corresponding to the 
 * operation. In case of no match, it returns -1
 */
static int is_operator (char c)
{
	enum operation t; 
	for (t = STAR; t <= RB; t++)
	{
		if ( c == operators[t] )
		{
			return t;
		}
	}

	return -1;
}

/* This function is called when an operator is encountered in the given regex.
 * If the priority of the operator on the top of the stack is lower than that of 
 * the new operator, the operator is popped, along with the required characters 
 * from the 'symbol_stack'. 
 */
static void insert_operator(enum operation op, char *final_regex) 
{
	enum operation top;

	#define insert_op_string(op) op_stack[++op_stack_top] = operators[op];

	if ( op == LB )
	{
		insert_op_string (LB);
		return;
	}

	while ( op_stack_top > -1 && symbol_stack_top > -1 && ( top = op_stack[op_stack_top]) < op)
	{
		
		printf ("StUcK !!! \n");
		if ( top == STAR ) 
		{
			*final_regex++ = symbol_stack[symbol_stack_top--];
			*final_regex++ = operators[STAR]; 
		}
		else if ( top == LB && op == RB)
		{
			op_stack_top--;
		}
		else
		{
			*final_regex++ = symbol_stack[symbol_stack_top--];
			*final_regex++ = operators[op_stack[op_stack_top--]];
		}		
	}

	
	switch (op)
	{
		case STAR 	: insert_op_string (STAR)  ; break;
		case CONCAT 	: insert_op_string (CONCAT); break;
		case OR		: insert_op_string (OR)    ; break;

		#ifdef DEBUG_MODE
		default 	: assert (0);	// This should never be reached.
		#else
		default 	: printf ("Unknown operator. Skipping and hoping for the best %d.\n", op);
		#endif
	}

	return;
}

void infix2postfix (char *original_regex, char *final_regex)
{
	symbol_stack_top = op_stack_top = -1;

	char ch;
	int operator_val;

	while ( ch = *original_regex++ )
	{	
		if ( (operator_val = is_operator (ch)) != -1)
		{
			insert_operator (operator_val, final_regex);
		}
		else
		{
			symbol_stack[++symbol_stack_top] = ch;
		}
		printf ("Stuck here\n");
	}

	enum operation op;
	// Empty the stacks.
	while ( op_stack_top > -1 && symbol_stack_top > -1)
	{
		printf ("2 Stuck here\n");
		op = op_stack[op_stack_top];
		
		if ( op == STAR )
		{
			*final_regex++ = symbol_stack[symbol_stack_top--];
			*final_regex++ = operators[STAR];
		}
		else
		{
			*final_regex++ = symbol_stack[symbol_stack_top--];
			*final_regex++ = symbol_stack[symbol_stack_top--];
			*final_regex++ = op;
			op_stack_top--;
		}
	}

	*final_regex = '\0';
}
