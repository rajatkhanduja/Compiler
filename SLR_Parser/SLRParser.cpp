// vim:ts=8:noexpandtab
/* This file defines the slrParser class. */
#include <SLRParser.h>
#include <Terminal_NonTerminal.hpp>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::stringstream;

const string SLRParser::UnexpectedTokenException = string ("Unexpected token received.");

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
		std::cerr << "Adding " << curItemSet << " -> " << shiftTo << "\n";
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
				std::cerr << "Adding "  << (*itr) << " " 
							<< symbol << " to table\n";
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
				for (strItr = follow.begin(); 
					strItr != follow.end(); strItr++)
				{
					if ( isTerminal (*strItr))
					{
						tmpSym = *strItr;
						std::cerr << "Adding " 
								<< tmpSym
								<< " to table\n";
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
			std::cerr << "Parse token " << token << "\n";
			if ( ACTION.count (actionKey) )
			{
				actionVal = ACTION[actionKey];
			}
			else
			{
				// Required symbol is empty in the table.
				std::cerr << "Unexpected token : " << token << "\n";
				throw UnexpectedTokenException;
			}
				

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

			token = lex.getNextToken();
		}
	}
	catch (string lexException)
	{
		assert(lexException.compare(LexicalAnalyser::NoInputFileException));
	}

}

string SLRParser::canonicalCollection2String ()
{
	return lr0automaton.canonicalCollection2String ();
}

void SLRParser::generateItemSet2NumMapping()
{
	map<ItemTerminalPair, ActionArgPair>::iterator itr;
	for (itr = ACTION.begin(); itr != ACTION.end(); itr++)
	{
		if (! itemSetStates.count(itr->first.first) )
		{
			itemSetStates[itr->first.first] = -1;
			std::cerr << itr->first.first << " : " << itemSetStates[itr->first.first] << "\n";
		}
	}
//	std::cerr << "Size of ACTION :" << ACTION.size() << "\n";
//	std::cerr << "Size of itemSetStates :" << itemSetStates.size() << "\n";
	// Need to iterate through the generated list to be able to assign numbers
	map<ItemSet*, int>::iterator itr1;
	int counter = 1;
	for (itr1 = itemSetStates.begin(); itr1 != itemSetStates.end();
		itr1++, counter++)
	{
		itr1->second = counter;
	}

	return;
}

string SLRParser::actionArgPair2String (const ActionArgPair& pair)
{
	stringstream output;
	if (Reduce == pair.first)
	{
		output << "r";
	}
	else if (Shift == pair.first)
	{
//		std::cerr << "itemSetStates [inBefore] :" << pair.second.shiftTo << "\n";
		map<ItemSet*, int>::iterator itr = itemSetStates.find(pair.second.shiftTo);
		if (itr == itemSetStates.end())
			output << "s" << pair.second.shiftTo;
		else
			output << "s" << itr->second;
;
	}
	std::cerr << "itemSetStates [inAfter] :" <<itemSetStates.size() << "\n";
	return output.str();
}

string SLRParser::actionTable2String ()
{
//	map <ItemSet*, int> itemSetStates;

	// First create a map of itemSets to be able to enumerate them.
	generateItemSet2NumMapping();

	// print all Terminals. (top of the table)
	stringstream output;
	int i;
	const int n = NTerminals();
	for ( i = 0; i < n; i++)
	{
		output << "\t" << getTerminal(i);
	}
	output << "\n";

	// Start printing the values for each of the ItemSet
	map<ItemTerminalPair, ActionArgPair>::iterator itr, iActionTableItr;
	map<ItemTerminalPair, ActionArgPair>::const_iterator actionItrEnd = 
								ACTION.end();
	map<ItemSet*, int>::iterator itr1;
	map<ItemSet*, int>::iterator itrStart = itemSetStates.begin(), 
					itrEnd   = itemSetStates.end(); 
	int tmpCounter = 1;
	for (itr1 = itrStart; itr1 != itrEnd; itr1++)
	{
		output << itr1->first << "->" << itr1->second;
		for ( i = 0; i < n; i++)
		{
			iActionTableItr =  ACTION.find (make_pair (itr1->first,
							getTerminal(i)));
			if (iActionTableItr != actionItrEnd)
			{
				output  << "\t"
					<< actionArgPair2String(
					iActionTableItr->second); 
//				std::cerr << "itemSetStates [inAfter] :" <<itemSetStates.size() << "\n";
			}
			else
			{
				output << "\t" << "-";
			}
						
		}
		output << "\n";
	}

//	std::cerr << output.str();
	return output.str();
}
