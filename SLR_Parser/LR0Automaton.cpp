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

LR0Automaton::ItemSet* LR0Automaton::rule2ItemSet(Rule& rule,ItemSet& result)
{
	unsigned int i, n;
	for ( i = 0, n = rule.RuleNTails(); i < n; i++)
	{
		result.insert (rule2Item (rule, i));	
	}
	return &result;
}

LR0Automaton::Item LR0Automaton::rule2Item (Rule& rule, unsigned int ruleIndex)
{
	return (make_pair (rule.RuleHead(), 
			make_pair (vector <string> (),
				rule.RuleTail(ruleIndex))));
}

void LR0Automaton::constructCanonicalCollection ()
{
	// Start with inserting the set of item containing only S'->S
	ItemSet startSet, tmpSet;
	rule2ItemSet (*startRule, startSet);
	bool newElemAdded = true;

	set<ItemSet>::iterator itr, itr_end;
	/* Verify if this remains constant 
	ItemSet::iterator itr_end = canonicalCollection.end();
	*/

	canonicalCollection.insert (*(ItemSetsClosure (startSet, tmpSet)));

	while (newElemAdded)
	{
		newElemAdded = false;
		for (itr = canonicalCollection.begin(), 
		     itr_end = canonicalCollection.end();
		     itr != itr_end; itr++)
		{
			
		}
	}
}

LR0Automaton::ItemSet* LR0Automaton::ItemSetsClosure (const ItemSet& items, 
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
					}
				}
			}
		}
	}

	return &result;
}

LR0Automaton::ItemSet* LR0Automaton::goTo (ItemSet* I, const string& X)
{
	/* First check GOTO */
	map<ItemTerminalPair, ItemSet*>::iterator itr;
	if ( (itr = GOTO.find (make_pair (I, X))) != GOTO.end ())
	{
		return itr->second;	
	}
	
	ItemSet * result = new ItemSet();
	ItemSet tmpSet;
	Item tmpItem;

	ItemSet::iterator itemItr;
	for ( itemItr = I->begin(); itemItr != I->end(); itemItr++)
	{
		if ( ! X.compare (itemItr->second.second[0]))
		{
			
		}
	}

	return (ItemSetsClosure (tmpSet, *result));
}
