#include <regex_parser.h>
#include <iostream>
#include <vector>

using std::vector;

/* Supported REGEX operations with priority. Lower the 'value', higher the priority */ 
enum operation          
{
        STAR,
        CONCAT,
        OR,
        LB,
        RB
        // Keep RB at the lowest priority
};

char operators[] = { '*', '@', '|', '(', ')' };


static vector <char> opStack;


/* This function returns the enum operation for the character corresponding 
 * to the operation. In case of no match, it returns -1.
 */
int isOperator (char c)                                                      
{
        int t;                                                     
        for (t = STAR; t <= RB; t++)
        {
                if ( c == operators[t] )                                     
                {
                        return t;                                            
                }       
        }       
        
        return -1;
}

/* This function accepts a string of regular expression and inserts 
 * concatenation symbol to the string. 
 */
static string insertConcatSymbol (string originalRegex)
{
	string finalRegex;

	char prevCh = '*';
	char concatSymbol = operators[CONCAT];
	unsigned int i;
	int op;

	for ( i = 0; i < originalRegex.length(); i++)
	{
		if ( originalRegex[i] == '\\' && prevCh != '\\')
		{
			// Next character is escaped
			prevCh = '\\';
			continue;
		}

		if ( prevCh == '\\')
		{
			// Not operator but escaped character
			finalRegex.push_back (concatSymbol);
			finalRegex.push_back ('\\');
			finalRegex.push_back (originalRegex[i]);

			// For next iteration to not confuse it with operator
			prevCh = 'a';	// Any character without sp meaning
			continue;
		}

		if ( (op = isOperator (originalRegex[i])) > -1 )
		{
			
			if ( op == LB)
			{
				if ( isOperator (prevCh ) == -1)
				{
					finalRegex.push_back (concatSymbol);
				}
			}

			finalRegex.push_back (originalRegex[i]);
		}
		else if ( (op = isOperator (prevCh)) > -1 )
		{
			// Previous character was an operator.
			if ( op == RB )
			{
				finalRegex.push_back (concatSymbol);
			}
			finalRegex.push_back (originalRegex[i]);
		}
		else
		{
			finalRegex.push_back (concatSymbol);
			finalRegex.push_back (originalRegex[i]);
		}

		prevCh = originalRegex[i];
	}

	std::cerr << finalRegex << std::endl;
	return (finalRegex);
}

/* This function is called when an operator is encountered in the given regex.
 * If the priority of the operator on the top of the stack is lower than that of 
 * the new operator, the operator is popped, along with the required characters 
 * from the 'symbol_stack'. 
 */
static char * insert_operator(enum operation op, char *final_regex) 
{
	enum operation top;

	#define insert_op_string(op) opStack[++opStack_top] = operators[op];

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

	while ( opStack_top > -1 
		&& 
		(( top = is_operator (opStack[opStack_top])) < op))
	{
		if ( top == LB && op == RB)
		{
			opStack_top--;
			return (final_regex);
		}
		else if ( top != LB )
		{
			*final_regex++ = opStack[opStack_top];
			opStack_top--;
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

static string infix2postfix (string originalRegex)
{
	string finalRegex
	char ch, prevCh;
	int operatorVal;

	insertConcatSymbol (originalRegex);

	for ( i = 0; i < originalRegex.length (); i++)
	{	
		ch = originalRegex[i];
		if ( (operatorVal = isOperator (ch)) != -1 && prevCh != '\\')
		{
			final_regex = insert_operator (operatorVal, final_regex);
		}
		else
		{
			*final_regex++ = ch;
		}
		if ( prevCh == ch && prevCh == '\\')
		{
			// Change it to any character without special meaning
			ch = 'a';
		}
		prevCh = ch;
	}

	enum operation op;
	// Empty the stacks.
	while ( opStack_top > -1)
	{
		*final_regex++ = opStack[opStack_top--];
	}

	*final_regex = '\0';
}


RegexParser::RegexParser (string regex)
{
	inputRegexString = regex;
	regexString = internalRegex (regex);
}

string RegexParser::internalRegex (string inputRegex)
{
	string tempRegex = insertConcatSymbol (inputRegex);

	return tempRegex;
}
