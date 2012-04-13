// vim:ts=8:noexpandtab
/* This file defines the slrParser class. */
#include <SLRParser.h>
#include <Terminal_NonTerminal.hpp>
#include <fstream>

using std::ifstream;

SLRParser::SLRParser (char * lexFile, char * grammarFile) 
	: lr0automaton (grammarFile)
{
	// Read Lex rules.
	ifstream lexRulesFile (lexFile);
	lex.readRules (lexRulesFile);

	startSet = lr0automaton.startItemSet ();
	
	int numTerminals = NTerminals();
	int numNonTerminals = NNonTerminals();
	int i;
	string Gsym;

	//############################# FIRST ##################################
	for ( i = 0; i < numTerminals; i++ )
	{
		Gsym = getTerminal(i);
		firstSet.First(Gsym, lr0automaton.slrGrammar);	
	}   	

	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		std::cerr << "sym : " << Gsym << std::endl;
		firstSet.First(Gsym, lr0automaton.slrGrammar);
	}
	//#############################  FIRST ##################################

	//############################# FOLLOW #############################################	
	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		followSet.Follow(firstSet, Gsym, lr0automaton.slrGrammar);
	}
	//############################# FOLLOW #############################################
	constructActionTable ();
}


/* Function that checks if the item is ready to be reduced
 * i.e. it checks if the dot is the last thing in the rule or that 
 * the vector containing the post-dot symbols is empty.
 */
inline bool isReduceReady (const Item& item)
{
	return (item.second.second.size() == 0);
}

/* Function that checks if the item is ready to be accepted.
 * This test involves first testing if the item is ready to be reduced and 
 * then if the producing symbol is that start symbol of the augmented grammar.
 * This assumes that the only occurrence of this start symbol is sufficient to 
 * indicate that it can be accepted. 
 */
inline bool isAcceptReady (const Item& item)
{
	return (isReduceReady(item) &&
		(!item.first.compare(LR0Automaton::augmentedStartSymbol)));
}

void SLRParser::addToActionTable (ItemSet* curItemSet, const string& terminal, 
				Action action, const Item* reduceRule, 
				ItemSet* shiftTo)
{
	ActionVal newAction;

	if (action == Shift)
	{
		newAction.shiftTo = shiftTo;
	}
	else if (action == Reduce)
	{
		newAction.reduceRule = reduceRule;
	}
	
	// Insert into the ACTION table
	ItemTerminalPair tmpPair = make_pair (curItemSet, terminal);

	if ( ACTION.find (tmpPair) == ACTION.end())
	{
		ACTION.insert (make_pair(tmpPair, make_pair (action, newAction)));
	}
	else
	{
		// Throw an exception.
		throw string ("Cannot create SLR table for grammar. Conflict when inserting ").append (terminal);
	}
}

void SLRParser::constructActionTable ()
{
	set<ItemSet*>::iterator itr, itr_end;

	// Iterate through the canonicalCollection in lr0automaton
	for ( 	itr = lr0automaton.canonicalCollection.begin(),
		itr_end = lr0automaton.canonicalCollection.end();
		itr != itr_end; itr++)
	{
		ItemSet * curItemSet = *itr;

		// Iterate through each Item in the ItemSet
		ItemSet::iterator itemSetItr, itemSetItrEnd;
		for (	itemSetItr = (*itr)->begin(),
			itemSetItrEnd = (*itr)->end(); itemSetItr != itemSetItrEnd;
			itemSetItr++)
		{
			string symbol = LR0Automaton::postDotSymbol (*itemSetItr);

			if (isTerminal (symbol))
			{
				addToActionTable (curItemSet, symbol, 
						Shift, NULL, 
						lr0automaton.goTo ((*itr),
							symbol));
			}

			if ( isReduceReady (*itemSetItr))
			{
				// Use FOLLOW and complete this part 
				string tmpSym = itemSetItr->first;
				vector<string> follow = followSet.Follow (
							firstSet, tmpSym,
							lr0automaton.slrGrammar);

				vector<string>::iterator strItr;
				std::cerr << "Follow : (" << tmpSym << "):\n" ;
				for (strItr = follow.begin(); 
					strItr != follow.end(); strItr++)
				{
					std::cerr << *strItr << std::endl;
					if ( isTerminal (*strItr))
					{
						addToActionTable (curItemSet,
								tmpSym, 
								Reduce, 
								&(*itemSetItr),
								NULL);
					}
				}
			}

			if ( isAcceptReady (*itemSetItr) )
			{
				addToActionTable (curItemSet, symbol, Accept, 
							NULL, NULL);
			}
		}
	}
}

template <class T> void emptyStack (stack<T>& s)
{
	while (!s.empty())
	{
		s.pop();
	}
}

/* Function that returns head of the production in the Item. */
string ItemHead (const Item* item)
{
	return item->first;
}

void printItem (Item* item)
{
	
}

void SLRParser::parse (ifstream& inputFile)
{
	lex.setInputFile (&inputFile);

	// Ensure the stack is empty.
	emptyStack (parseStack);

	// Insert the startSet into the stack.
	parseStack.push (startSet);

	try
	{
		string token = lex.getNextToken ();
		ItemTerminalPair actionKey;
		ActionArgPair actionVal;
		while (true)
		{
			actionKey = make_pair (parseStack.top(), token);
			actionVal = ACTION[actionKey];

			if ( Shift == actionVal.first )
			{
				// Shift
				parseStack.push (actionVal.second.shiftTo);
			}

			else if ( Reduce == actionVal.first )
			{
				// Reduce
				int i, n;
				n = actionVal.second.reduceRule->second.first.size()
				+ actionVal.second.reduceRule->second.second.size();
				
				for (i = 0; i < n; i++)
				{
					parseStack.pop();
				}
 				
				parseStack.push(lr0automaton.goTo (
					parseStack.top(), ItemHead(
						actionVal.second.reduceRule)));
	//			printItem (actionVal.second.reduceRule);

			}
			else if ( Accept == actionVal.first )
			{
				// Accept
				break;
			}
			else
			{
				// Error
				assert (0);
			}
		}
	}
	catch (string lexException)
	{
		assert(lexException.compare(LexicalAnalyser::NoInputFileException));
	}

}

void SLRParser::printCanonicalCollection ()
{
	lr0automaton.printCanonicalCollection ();
}
