// vim:ts=8:noexpandtab
/* This file defines the slrParser class. */
#include <SLRParser.h>
#include <Terminal_NonTerminal.hpp>

SLRParser::SLRParser (char * grammarFile) : lr0automaton (grammarFile)
{
	startSet = lr0automaton.startItemSet ();
	constructActionTable ();
}

/* Function that returns the first symbol that occurs after dot. */
inline string postDotSymbol (const Item& item)
{
	return (item.second.second.size() > 0) ? item.second.second[0] : string();
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
				Action action, Rule* reduceRule, ItemSet* shiftTo)
{
	ActionVal newAction;

	if (action == Shift)
	{
		newAction.shiftTo = shiftTo;
	}
	else if (action == Reduce)
	{
		newAction.reduceRule = *reduceRule;
	}
	
	// Insert into the ACTION table
	// TODO : Throw exception when the insertion creates a conflict.
	ItemTerminalPair tmpPair = make_pair (curItemSet, terminal);

	if ( ACTION.find (tmpPair) == ACTION.end())
	{
		ACTION.insert (make_pair(tmpPair, make_pair (action, newAction)));
	}
	else
	{
		// Throw an exception.
		throw string ("Cannot create SLR table for grammar.");
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
			string symbol = postDotSymbol (*itemSetItr);

			if (isTerminal (symbol))
			{
				addToActionTable (curItemSet, symbol, 
						Shift, NULL, 
						lr0automaton.goTo ((*itr),
							symbol));
			}

			if ( isReduceReady (*itemSetItr))
			{
				// TODO : Use FOLLOW and complete this part 
			}

			if ( isAcceptReady (*itemSetItr) )
			{
				addToActionTable (curItemSet, symbol, Accept, 
							NULL, NULL);
			}
		}
	}
}
