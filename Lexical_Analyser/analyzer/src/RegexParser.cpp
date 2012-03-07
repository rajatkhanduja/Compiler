#include <RegexParser.h>
#include <cassert>
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
static int isOperator (char c)
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

	char prevCh = '@';
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
			if ( op == RB || op == STAR )
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

static void insertOperator (int op, string &finalRegex)
{

	if ( op == LB )
	{
		opStack.push_back (operators[LB]);
		return;
	}

	if ( op == STAR )
	{
		finalRegex.push_back (operators[STAR]);
		return;
	}

	int top;

	while ( opStack.size () > 0 
		&& 
		( (top = isOperator (opStack.back ()) ) < op) )
	{
		if ( top == LB && op == RB )
		{
			opStack.pop_back ();
			return;
		}
		else if ( top != LB )
		{
			finalRegex.push_back (opStack.back ());
			opStack.pop_back ();
		}
	}

	switch (op)
	{
		case CONCAT	: opStack.push_back(operators[CONCAT]); break;
		case OR		: opStack.push_back(operators[OR])    ; break;

		#ifdef DEBUG_MODE
		default		: assert (0);
		#else
		default		: std::cerr<<"Unknown operator. Continuing\n";
#endif
	}
}

static string infix2Postfix (string modifiedRegex)
{
	/* The input string is assumed to be in internal form,
	 * that is it should have concatenate operator at the appropriate 
	 * positons.
	 */

	char ch, prevCh;
	int i, operatorVal;
	string finalRegex;

	for ( i = 0; i < modifiedRegex.length (); i++)
	{
		ch = modifiedRegex[i];

		if ( (operatorVal = isOperator (ch)) != -1 && prevCh != '\\')
		{
			insertOperator ( operatorVal, finalRegex);
		}
		else
		{
			finalRegex.push_back (ch);
		}

		if ( prevCh == ch && prevCh == '\\' )
		{
			// Change it to any character without special meaning
			ch = 'a';
		}

		prevCh = ch;
	}

	operation op;

	// Empty the stack
	while (opStack.size () > 0)
	{
		finalRegex.push_back (opStack.back());
		opStack.pop_back ();
	}

	return finalRegex;

}


RegexParser::RegexParser (string regex)
{
	inputRegexString = regex;
	regexString = internalRegex (regex);
	std::cerr << regexString << std::endl;
	generateFSM (fsm);
}

string RegexParser::internalRegex (string inputRegex)
{
	string finalRegex = insertConcatSymbol (inputRegex);
	finalRegex = infix2Postfix (finalRegex);

	return finalRegex;
}


/* This function uses the internalRegex expression to construct the FSM
 * The internal representation is in Postfix. Using a stack, it can be 
 * evaluated/converted.
 */
void RegexParser::generateFSM(FSM& fsm)
{
	vector<FSM*> stack;
	int op;
	string::iterator itr, itr_end;
	FSM *fsm1, *fsm2;

	for (itr = regexString.begin (), itr_end = regexString.end(); itr != itr_end; itr++)
	{
		if ( (op = isOperator (*itr)) == -1 )
		{
			fsm1 = new FSM(*itr);
			stack.push_back ( fsm1 );
		}
		else
		{
			switch (op)
			{
				case STAR :
					fsm1 = stack.back();
					stack.pop_back();
					fsm1= fsm1->repeat();
					stack.push_back(fsm1);
					break;
				
				case OR :
					fsm2 = stack.back();
					stack.pop_back();
					fsm1 = stack.back();
					stack.pop_back();
					fsm1 = *fsm1 | *fsm2;
					stack.push_back (fsm1);
					break;
				case CONCAT:
					fsm2 = stack.back();
					stack.pop_back();
					fsm1 = stack.back();
					stack.pop_back();
					fsm1->concatenate(*fsm2);
					stack.push_back (fsm1);
					break;
				default:
					assert(0);

			}
		}
	}

	fsm = * (stack.back());
}

/* Function to use the regular expression to match with input String */
int RegexParser::match (string pattern)
{
	return fsm.simulate (pattern);
}
