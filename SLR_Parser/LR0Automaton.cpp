// vim:ts=8:noexpandtab
/* This file defines the functions/methods of the LR0Automaton class. */

#include <LR0Automaton.h>
#include <Terminal_NonTerminal.hpp>
#include <xtoLL1.hpp>
#include <vector>

using std::vector;

static Rule * startRule;

// This function can be called after the grammar has been initialized
void LR0Automaton::initialize ()
{
	// Add the rule S' -> S to create the augmented grammar
	Rule startSymRule (augmentedStartSymbol);
	startSymRule.RuleAddTail (vector<string> (1, augmentedStartSymbol));
	slrGrammar.GrammarAddRule (startSymRule);
	slrGrammar.GrammarSetStartSymbol (augmentedStartSymbol);

	startRule = &startSymRule;

	// Construct Canonical Collection
	constructCanonicalCollection ();
}


LR0Automaton::LR0Automaton (char * grammarFileName)
{
	ScanGrammarFromFile (slrGrammar, grammarFileName);
	initialize ();
}

LR0Automaton::LR0Automaton (const Grammar& grammar)
{
	slrGrammar = grammar;
	initialize ();
}

/* Function to convert a rule (a particular tail) to an Item
 * This function returns an item such that the 'dot' is 
 * right in the beginning.
 */
Item rule2Item (Rule& rule, unsigned int ruleIndex)
{
	return (make_pair (rule.RuleHead(), 
			make_pair (vector <string> (),
				rule.RuleTail(ruleIndex))));
}
/* Function to convert a rule to an ItemSet using rule2Item
 * This function needs to be passed an ItemSet for results
 * A pointer to the same variable (result) is returned.
 */
ItemSet* rule2ItemSet(Rule& rule,ItemSet& result)
{
	unsigned int i, n;
	for ( i = 0, n = rule.RuleNTails(); i < n; i++)
	{
		result.insert (rule2Item (rule, i));	
	}
	return &result;
}

/* Function to take closure given an ItemSet 
 * This function needs to be passed an ItemSet for results
 * A pointer to the same variable (result) is returned.
 */
ItemSet* ItemSetsClosure (const ItemSet& items, Grammar &slrGrammar,
				ItemSet& result)
{
	result.insert (items.begin(), items.end());
	ItemSet::iterator itr, itr_end;

	bool newElemAdded = true;	
	Rule rule;
	int ruleIndex;

	while (newElemAdded)
	{
		newElemAdded = false;
		
		for (itr = result.begin(), itr_end = result.end();
			itr != itr_end; itr++)
		{
			if ( isNonTerminal ((itr->second).second[0]) )
			{
				ruleIndex = slrGrammar.GrammarFindRule(
						(itr->second).second[0]);
				if ( ruleIndex >= 0)
				{
					rule = slrGrammar.GrammarRule (
								ruleIndex);
					ItemSet fromRules;
					rule2ItemSet (rule, fromRules);
					
					ItemSet::iterator itr1, itr1_end;
					itr1 = fromRules.begin ();
					itr1_end = fromRules.end ();

					while (itr1 != itr1_end)
					{
						if (result.find (*itr) == 
							result.end ())
						{
							result.insert (*itr);
							newElemAdded = true;
						}
						itr++;
					}
				}
			}
		}
	}

	return &result;
}

/* Function to return the set of symbols that will have 
 * a corresponding value in GOTO. Basically, all the symbols that follow
 * the 'dot', i.e. the first string in the second vector in Item.
 */
set<string> requiredSymbols (const ItemSet& itemSet)
{
	ItemSet::iterator itr, itr_end;
	set<string> symbols;

	for (itr = itemSet.begin(), itr_end = itemSet.end();
		itr != itr_end; itr++)
	{
		if (itr->second.second.size() > 0)
			symbols.insert (itr->second.second[0]);
	}

	return symbols;
}

void LR0Automaton::constructCanonicalCollection ()
{
	// Start with inserting the set of item containing only S'->S
	ItemSet startSet, *tmpSet = new ItemSet();
	rule2ItemSet (*startRule, startSet);
	bool newElemAdded = true;

	set<ItemSet*>::iterator itr, itr_end;
	/* Verify if this remains constant 
	ItemSet::iterator itr_end = canonicalCollection.end();
	*/

	canonicalCollection.insert (
		(ItemSetsClosure (startSet, slrGrammar, *tmpSet)));

	while (newElemAdded)
	{
		newElemAdded = false;
		for (itr = canonicalCollection.begin(), 
		     itr_end = canonicalCollection.end();
		     itr != itr_end; itr++)
		{
			/* For each symbol, check GOTO */
			set<string> symGoTo = requiredSymbols (**itr);
			set<string>::iterator strSetItr = symGoTo.begin();
			set<string>::iterator strSetItrEnd = symGoTo.end();
			while (strSetItr != strSetItrEnd)
			{
				ItemSet * gotoSet = goTo (*itr, *strSetItr);

				/* If gotoSet is not empty and is not in
				 * in canonicalCollection. */
				if ( !gotoSet->empty() && 
					(canonicalCollection.find (*itr) !=
						canonicalCollection.end()) )
				{
					canonicalCollection.insert (gotoSet);
				}
				itr++;
			}
		}
	}
}


static Item shiftDot (const Item& item)
{
	Item result(item);
	
	// Shift the 'dot' virtually by erasing the first element
	// from the second part of the item and inserting into the first part
	result.second.first.push_back (result.second.second[0]);
	result.second.second.erase (result.second.second.begin());

	return result;
}

ItemSet* LR0Automaton::goTo (ItemSet* I, const string& X)
{
	/* First check GOTO */
	map<ItemTerminalPair, ItemSet*>::iterator itr;
	if ( (itr = GOTO.find (make_pair (I, X))) != GOTO.end ())
	{
		return itr->second;	
	}
	
	ItemSet tmpSet, *result = new ItemSet();
	Item tmpItem;

	ItemSet::iterator itemItr;
	for ( itemItr = I->begin(); itemItr != I->end(); itemItr++)
	{
		if ( ! X.compare (itemItr->second.second[0]))
		{
			// Store the changed item in tmpItem
			tmpItem = shiftDot (*itemItr);

			// Insert tmpItem to tmpSet
			tmpSet.insert (tmpItem);
		}
	}

	return (ItemSetsClosure (tmpSet, slrGrammar, *result));
}	

