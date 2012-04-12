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

/* Function that checks if the item is ready to be accepted 
 * i.e. it checks if the dot is the last thing in the rule or that 
 * the vector containing the post-dot symbols is empty.
 */
inline bool isAcceptReady (const Item& item)
{
	return (item.second.second.size() == 0);
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
	ACTION.insert ( make_pair( 
		make_pair (curItemSet, terminal),
		make_pair (action, newAction)));
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

			if ( !symbol.compare (string()))
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
