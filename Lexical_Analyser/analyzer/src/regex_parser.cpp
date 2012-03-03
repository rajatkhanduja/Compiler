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
