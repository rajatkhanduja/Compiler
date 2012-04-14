#include <NonRecursivePredictiveParser.hpp>

void printProduction(Rule rule)
{
	
	string head = rule.RuleHead();
	vector<string> tail = rule.RuleTail(0);	
	vector<string>::iterator it;

	std::cerr << head << " ==> ";
	for ( it = tail.begin(); it < tail.end(); it++ )
	{
		std::cerr << *it << " ";
	}
	std::cerr << std::endl;
}

NonRecursivePredictiveParser::NonRecursivePredictiveParser(Grammar& CFG, FirstSet& firstSet, FollowSet& followSet)
{
	// Empty
	// Prepare the parsingTable
	(this->parsingTable).PopulateTable(CFG, firstSet, followSet);
}

void NonRecursivePredictiveParser::PrintTable()
{
	(this->parsingTable).PrintTable();
}



void NonRecursivePredictiveParser::PrepareInput(string line)
{
	// Tokenize the line [ SPACE SEPERATED LINES ]
	string token;
	istringstream iss(line);
	
	while (iss)
	{
		iss >> token;
		(this->input).push_back(token);
	}
	
	(this->input).push_back("$");		
}


void NonRecursivePredictiveParser::ClearInput()
{
	(this->input).clear();
}


void NonRecursivePredictiveParser::ParseInput(Grammar& CFG)
{
	int ip = 0;	/* input pointer */
	TableKey <string, string> key;
	vector<Rule> foundRule;
	vector<Rule>::iterator itr;
	vector<string>::iterator its;
	vector<string> tail;
	vector<string>::iterator X;
	string endMarker(ENDMARKER);
	(this->parserStack).push_back(endMarker);
	(this->parserStack).push_back(CFG.GrammarStartSymbol());
	
	
	X = (this->parserStack).end() - 1;	/* Top of the stack */
	while ( (*X).compare(ENDMARKER) != 0 )
	{

		key.SetKey(*X, (this->input)[ip]);
		foundRule = (this->parsingTable).TableFind(key);  

		if ( (*X).compare((this->input)[ip]) == 0 )	/* (*X) is terminal and is equal to the CURRENT input symbol. */
		{
			(this->parserStack).pop_back();
			ip++;
		}
		else if ( isTerminal(*X) )
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
			
			tail = (*itr).RuleTail(0);	//#TODO tail has to be a PUBLIC member function.

			// Print the production chosen.
			// Build the depth first tree.
			printProduction(*itr);

			for ( its = tail.end() - 1; its >= tail.begin(); its-- )
			{
					(this->parserStack).push_back(*its);
			}
		}

		X = (this->parserStack).end() - 1;	/* Set 'X' to top of the stack. */	
	}
}
