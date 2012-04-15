// vim:ts=8:noexpandtab
/* This file defines the slrParser class. */
#include <SLRParser.h>
#include <Terminal_NonTerminal.hpp>
#include <fstream>
#include <sstream>
#include <list>

using std::ifstream;
using std::stringstream;
using std::list;

const string SLRParser::UnexpectedTokenException = string ("Unexpected token received.");

string SLRParser::followSet2String ()
{
	stringstream output;
	string nonTerm;
	int i;
	for ( i = 0; i < NNonTerminals (); i++)
	{
		nonTerm = getNonTerminal(i);
		output  << "Follow (" << nonTerm
			<< ") :";
		list<string> follow = followSet.Follow (firstSet, nonTerm,
					lr0automaton.slrGrammar);
		list<string>::iterator itr;

		for ( itr = follow.begin(); itr != follow.end(); itr++)
		{
			output << " " << *itr;
		}
		output << "\n";
	}
	
	return output.str();
}

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
		firstSet.First(Gsym, lr0automaton.slrGrammar);
	}


	firstSet.RemoveDuplicatesFromFirst();
	//#############################  FIRST ##################################

	//############################# FOLLOW #############################################	
	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		followSet.Follow(firstSet, Gsym, lr0automaton.slrGrammar);

		// To handle right recursion
		followSet.ProcessDependencyList();
	}

	followSet.RemoveDuplicatesFromFollow();
	//############################# FOLLOW #############################################
	
	// Add ENDMARKER
	addTerminal (string("$"));

	// Print the FOLLOWS.
	std::cerr << "================ FOLLOWS =============\n";
	std::cerr << followSet2String();
	std::cerr << "======================================\n";
	
	generateItemSet2NumMapping();
	constructActionTable ();
}


/* Function that checks if the item is ready to be reduced
 * i.e. it checks if the dot is the last thing in the rule or that 
 * the vector containing the post-dot symbols is empty. Also, the head should not
 * be the start symbol of the augmented grammar.
 */
inline bool isReduceReady (const Item& item)
{
	return (item.second.second.size() == 0 &&
		item.first.compare (LR0Automaton::augmentedStartSymbol));
}

/* Function that checks if the item is ready to be accepted.
 * This test involves first testing if the item is ready to be reduced and 
 * then if the producing symbol is that start symbol of the augmented grammar.
 * This assumes that the only occurrence of this start symbol is sufficient to 
 * indicate that it can be accepted. 
 */
inline bool isAcceptReady (const Item& item)
{
	return (item.second.second.size() == 0 &&
		!(item.first.compare(LR0Automaton::augmentedStartSymbol)));
}

void SLRParser::addToActionTable (ItemSet* curItemSet, const string& terminal, 
				Action action, const Item* reduceRule, 
				ItemSet* shiftTo)
{
	ActionVal newAction;

	newAction.shiftTo    = shiftTo;
	newAction.reduceRule = reduceRule;
	
	// Insert into the ACTION table
	ItemTerminalPair tmpPair = make_pair (curItemSet, terminal);
	map<ItemTerminalPair, ActionArgPair>::iterator itr;

	if ( (itr = ACTION.find (tmpPair)) == ACTION.end() || 
		(itr->second.first == action && 
		itr->second.second.shiftTo == shiftTo &&
		itr->second.second.reduceRule == reduceRule))
	{
		ACTION.insert (make_pair(tmpPair, make_pair (action, newAction)));
	}
	else
	{
		stringstream conflictPoint;
		if ( (itr->second.first == Reduce ) ^ (action == Reduce) )
		{
			conflictPoint << "Shift-reduce conflict at ";
		}
		else
		{
			conflictPoint << "Reduce-reduce conflict at ";
		}
		conflictPoint << "(" << itemSetStates[curItemSet] << "," << terminal << ")\n";
		conflictPoint << "Item set :- \n" << itemSet2String (*curItemSet) << "\n"; 
		
		// Throw an exception.
		throw string ("Cannot create SLR table for grammar.\n ").append (conflictPoint.str());
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
//			std::cerr << item2String(*itemSetItr) << "\n";
			string symbol = LR0Automaton::postDotSymbol (*itemSetItr);

			if (isTerminal (symbol))
			{
/*				std::cerr << "Adding ("  << itemSetStates[curItemSet] << " ," 
							<< symbol << ") to table\n";*/
				addToActionTable (curItemSet, symbol, 
						Shift, NULL, 
						lr0automaton.goTo ((*itr),
							symbol));
			}

			if ( isReduceReady (*itemSetItr))
			{
				// Use FOLLOW and complete this part 
				string tmpSym = itemSetItr->first;
				list<string> follow = followSet.Follow (
							firstSet, tmpSym,
							lr0automaton.slrGrammar);
				follow.sort ();
				follow.unique();

			
				list<string>::iterator strItr;
				for (strItr = follow.begin(); 
					strItr != follow.end(); strItr++)
				{
	//				std::cerr << *strItr << "\n";
					if ( isTerminal (*strItr))
					{
						tmpSym = *strItr;
	/*					std::cerr << "Adding " << itemSetStates[curItemSet] << " ," 
								<< tmpSym
								<< " to table\n";*/
						addToActionTable (curItemSet,
								tmpSym, 
								Reduce, 
								&(*itemSetItr),
								NULL);
					}
				}
	//			std::cerr << std::endl;
			}

			if ( isAcceptReady (*itemSetItr) )
			{
//				std::cerr << "Adding accept state\n";
				addToActionTable (curItemSet, string ("$"), Accept, 
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

string SLRParser::getNextToken_()
{
	try 
	{
		return lex.getNextToken ();
	}
	catch (string exception)
	{
		if (!exception.compare (
			LexicalAnalyser::NoMoreTokenException))
		{
			std::cerr << "Error caught\n"; 
			return string("$");
		}	
		else
		{
			std::cerr << "SEVERE Error caught\n"; 
			throw;
		}
			
	}
}

stack<Item> SLRParser::parse (ifstream& inputFile)
{
	stack<Item> reductions;

	lex.setInputFile (&inputFile);

	// Ensure the stack is empty.
	emptyStack (parseStack);

	// Insert the startSet into the stack.
	parseStack.push (startSet);

	string token;
	ItemTerminalPair actionKey;
	ActionArgPair actionVal;
	token = getNextToken_();
	while (true)
	{
		actionKey = make_pair (parseStack.top(), token);
		std::cerr << "Parse token " << token << "\n";
		std::cerr << "Stack top "  << itemSetStates[parseStack.top()]
			  << "\n";
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
			token = getNextToken_();
		}

		else if ( Reduce == actionVal.first )
		{
			// Reduce
			int i, n;
			n = actionVal.second.reduceRule->second.first.size()
			+ actionVal.second.reduceRule->second.second.size();

			std::cerr << item2String(*actionVal.second.reduceRule)
				  << "\n";
			reductions.push (*actionVal.second.reduceRule);
			
			for (i = 0; i < n; i++)
			{
				parseStack.pop();
				std::cerr << "Popped\n";
			}
			
			parseStack.push(lr0automaton.goTo (
				parseStack.top(), ItemHead(
					actionVal.second.reduceRule)));
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

	std::cerr << "Parsed\n";
	return reductions;
}

string SLRParser::canonicalCollection2String ()
{
	return lr0automaton.canonicalCollection2String ();
}

void SLRParser::generateItemSet2NumMapping()
{
	int i;
	for (i = 0; i < lr0automaton.states.size(); i++)
	{
		itemSetStates[lr0automaton.states[i]] = i + 1;
////		std::cerr << "[genI2NM] Added " << lr0automaton.states[i] <<"\n";
	}

	return;
}

string SLRParser::actionArgPair2String (const ActionArgPair& pair)
{
	stringstream output;
	if (Reduce == pair.first)
	{
		output << "r " << item2String (*(pair.second.reduceRule));
	}
	else if (Shift == pair.first)
	{
		map<ItemSet*, int>::iterator itr = 
				itemSetStates.find(pair.second.shiftTo);
		if (itr == itemSetStates.end())
			output << "s" << pair.second.shiftTo;
		else
			output << "s" << itr->second;
;
	}
	else if (Accept == pair.first)
	{
		output << "a";
	}
	return output.str();
}

string SLRParser::actionTable2String ()
{
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
	int j;
	for (j = 0; j < lr0automaton.states.size(); j++)
	{
		output << j + 1;
		for ( i = 0; i < n; i++)
		{
			iActionTableItr =  ACTION.find (make_pair (
						lr0automaton.states[j],
						getTerminal(i)));
			if (iActionTableItr != actionItrEnd)
			{
				output  << "\t"
					<< actionArgPair2String(
					iActionTableItr->second); 
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

string SLRParser::gotoTable2String (void)
{
	int i;
	stringstream output;

	for ( i = 0; i < NNonTerminals(); i++)
	{
		output << "\t" << getNonTerminal (i);
	}
	for ( i = 0; i < NTerminals(); i++)
	{
		output << "\t" << getTerminal (i);
	}
	output << "\n";
	for ( i = 0; i < lr0automaton.states.size(); i++)
	{
		output << i + 1;
		int j;
		for ( j = 0; j < NNonTerminals(); j++)
		{
			map<ItemSet*, int>::iterator itr =
				itemSetStates.find(lr0automaton.GOTO[
					make_pair (
						lr0automaton.states[i],
						getNonTerminal(j))]);
			output << "\t";
			if ( itr != itemSetStates.end())
			{
				output << (itr->second);
			}
			else
			{
				output << "-";
			}
		}
		for ( j = 0; j < NTerminals(); j++)
		{
			map<ItemSet*, int>::iterator itr =
				itemSetStates.find(lr0automaton.GOTO[
					make_pair (
						lr0automaton.states[i],
						getTerminal(j))]);
			output << "\t";
			if ( itr != itemSetStates.end())
			{
				output << (itr->second);
			}
			else
			{
				output << "-";
			}
		}
		output << "\n";
	}

	return output.str();
}


string parseStack2String (stack<Item> reductions)
{
#define POPULATE_MAP(sym) \
	if ( !grammarSymbols.count (sym))\
	{\
		grammarSymbols[sym] = 0; \
	}
#define PRINT_PRODUCTION(v,itr,output) \
	for ( itr = v.begin (); itr != v.end(); itr++) \
	{\
		output  << "\"" << head.str() << "\""\
			<< " -> "; \
		output << "\"" << *itr << "_" << rightSide[*itr] << "\";";\
		if ( isNonTerminal(*itr) ) \
		{\
			leftSide[*itr].push(rightSide[*itr]);\
		}\
		rightSide[*itr] += 1;\
		output << "\n"; \
	}

	map<string, int> rightSide;
	map<string, stack<int> > leftSide;
	stringstream output; 
	while (reductions.size())
	{
		Item rule = reductions.top ();
		reductions.pop();
		stringstream head;
		if ( ! leftSide[rule.first].size() )
		{
			leftSide[rule.first].push(0);
			rightSide[rule.first]++;
		}
			
		head << rule.first << "_" <<  leftSide[rule.first].top();
		leftSide[rule.first].pop();
		vector<string>::iterator itr;
		PRINT_PRODUCTION (rule.second.first , itr, output);
		PRINT_PRODUCTION (rule.second.second, itr, output);
		output << "\n";
	}

	return output.str();
}
