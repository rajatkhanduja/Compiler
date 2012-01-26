/* This file defines the functions required to parse regular expressions */

#include <stdio.h>		// for showing error message
#include <regex_parser.h>
#include <assert.h>
#include <string.h>

static char op_stack[MAX_REGEX_LEN];
static int op_stack_top;		// Indicates the top most element


/* This function returns the enum operation for the character corresponding to the 
 * operation. In case of no match, it returns -1
 */
int is_operator (char c)
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

/* This function accepts a string a regular expression and converts
 * the range elements into 'or' separated expressions
 */
static void convert_range (char *original_regex)
{
	// TODO
}

/* This function accepts a string of regular expression and inserts 
 * concatenation symbol to the string. 
 */
static void insert_concat_sym (char *original_regex)
{
	unsigned int len = strlen (original_regex);
	char temp[(2 * len) + 1];
	char ch_prev = '*';
	unsigned int i, j;
	int op;

	for ( i = 0, j = 0; i < len && original_regex[i]; i++, j++)
	{
		if ( original_regex[i] == '\\' && ch_prev != '\\')
		{
			// Next character is escaped
			ch_prev = '\\';
			j--;
			continue;
		}

		if ( ch_prev == '\\')
		{
			// Not operator but escaped character
			temp[j++] = '@';
			temp[j++] = '\\';
			temp[j] = original_regex[i];

			// For next iteration to not confuse it with operator
			ch_prev = 'a';	// Any character without sp meaning
			continue;
		}

		if ( (op = is_operator (original_regex[i])) > -1 )
		{
			
			if ( op == LB)
			{
				if ( is_operator (ch_prev ) == -1)
				{
					temp[j++] = '@';
				}
			}

			temp[j] = original_regex[i];
		}
		else if ( (op = is_operator (ch_prev)) > -1 )
		{
			// Previous character was an operator.
			if ( op == RB )
			{
				temp[j++] = '@';
			}
			temp[j] = original_regex[i];
		}
		else
		{
			temp[j++] = '@';
			temp[j] = original_regex[i];
		}

		ch_prev = original_regex[i];
	}

	temp[j] = '\0';

	strcpy (original_regex, temp);
}

/* This function is called when an operator is encountered in the given regex.
 * If the priority of the operator on the top of the stack is lower than that of 
 * the new operator, the operator is popped, along with the required characters 
 * from the 'symbol_stack'. 
 */
static char * insert_operator(enum operation op, char *final_regex) 
{
	enum operation top;

	#define insert_op_string(op) op_stack[++op_stack_top] = operators[op];

	if ( op == LB )
	{
		insert_op_string (LB);
		return (final_regex);
	}

	if ( op == STAR )
	{
		*final_regex++ = operators[op];
		return (final_regex);
	}

	while ( op_stack_top > -1 
		&& 
		(( top = is_operator (op_stack[op_stack_top])) < op))
	{
		if ( top == LB && op == RB)
		{
			op_stack_top--;
			return (final_regex);
		}
		else if ( top != LB )
		{
			*final_regex++ = op_stack[op_stack_top];
			op_stack_top--;
		}		
	}

	switch (op)
	{
		case CONCAT 	: insert_op_string (CONCAT); break;
		case OR		: insert_op_string (OR)    ; break;

		#ifdef DEBUG_MODE
		default 	: assert (0);	// This should never be reached.
		#else
		default 	: printf ("Unknown operator. Skipping and hoping for the best %d.\n", op);
		#endif
	}

	return (final_regex);
}

void infix2postfix (char *original_regex, char *final_regex)
{
	op_stack_top = -1;

	char ch, ch_prev;
	int operator_val;

	insert_concat_sym (original_regex);
	printf ("%s : after symbols\n", original_regex);

	while ( ch = *original_regex++ )
	{	
		if ( (operator_val = is_operator (ch)) != -1 && ch_prev != '\\')
		{
			final_regex = insert_operator (operator_val, final_regex);
		}
		else
		{
			*final_regex++ = ch;
		}
		if ( ch_prev == ch && ch_prev == '\\')
		{
			// Change it to any character without special meaning
			ch = 'a';
		}
		ch_prev = ch;
	}

	enum operation op;
	// Empty the stacks.
	while ( op_stack_top > -1)
	{
		*final_regex++ = op_stack[op_stack_top--];
	}

	*final_regex = '\0';
}
