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

static void insertOperator (operation op, string &finalRegex)
{

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
			insertOperator ( (operation) operatorVal, finalRegex);
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
}

string RegexParser::internalRegex (string inputRegex)
{
	string tempRegex = insertConcatSymbol (inputRegex);

	return tempRegex;
}
