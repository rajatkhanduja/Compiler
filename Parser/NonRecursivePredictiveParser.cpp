#include "NonRecursivePredictiveParser.hpp"

void NonRecursivePredictiveParser::ParseInput(Grammar CFG)
{
	int ip = 0;	/* input pointer */
	TableKey <string, string> key;
	vector<Rule> foundRule;
	vector<Rule>::iterator itr;
	vector<string> its;
	vector<string> tail;

	vector<string>::iterator X = (this->parserStack).end() - 1;	/* Top of the stack */	
	while ( *X != ENDMARKER )
	{
		key.SetKey(*X, (this->input)[ip]);
		foundRule = (this->parsingTable).TableFind(key);  

		if ( *X == (this->input)[ip] )	/* (*X) is terminal and is equal to the CURRENT input symbol. */
		{
			(this->parserStack).pop_back();
			ip++;
		}
		else if ( CFG.isTerminal(*X) )
		{
			std::cerr << "Error in parsing." << std::endl;
		}
		else if ( foundRule.begin() == foundRule.end() )	/* i.e M[X,a] is empty */
		{
			std::cerr << "Error in parsing. No rule found in Parsing Table" << std::endl;
		}
		else if ( foundRule.begin() != foundRule.end() )
		{
			
			(this->parserStack).pop_back();		/* Pop X from the stack */

			for ( itr = foundRule.begin(); itr != foundRule.end(); itr++ )
			{
				//#TODO Current implementation for just one rule. Hence nothing here.
			}
			itr = foundRule.begin();
			tail = (*itr).tail;	//#TODO tail has to be a PUBLIC member function.
			for ( its = tail.end() - 1; its != tail.begin() - 1; its-- )
			{
					(this->parserStack).push_back(*its);
			}
		}

		X = *((this->parserStack).end() - 1);	/* Set 'X' to top of the stack. */	
	}
}
