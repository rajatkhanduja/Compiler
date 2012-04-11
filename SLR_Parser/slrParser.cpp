// vim:ts=8:noexpandtab
/* This file defines the functions/methods of the slrParser class. */

#include <slrParser.h>
#include <Terminal_NonTerminal.hpp>
#include <xtoLL1.hpp>
#include <vector>

using std::vector;

static Rule * startRule;

slrParser::slrParser (char * grammarFileName)
{
	ScanGrammarFromFile (slrGrammar, grammarFileName);

	// Add the rule S' -> S to create the augmented grammar
	Rule startSymRule (augmentedStartSymbol);
	startSymRule.RuleAddTail (vector<string> (1, augmentedStartSymbol));
	slrGrammar.GrammarAddRule (startSymRule);
	slrGrammar.GrammarSetStartSymbol (augmentedStartSymbol);

	startRule = &startSymRule;

	// Construct Canonical Collection
	constructCanonicalCollection ();
}

slrParser::ItemSet* slrParser::rule2ItemSet(Rule& rule,ItemSet& result)
{
	unsigned int i, n;
	for ( i = 0, n = rule.RuleNTails(); i < n; i++)
	{
		result.insert (rule2Item (rule, i));	
	}
	return &result;
}

slrParser::Item slrParser::rule2Item (Rule& rule, unsigned int ruleIndex)
{
	return (make_pair (rule.RuleHead(), 
			make_pair (string(),rule.RuleTail(0)[ruleIndex])));
}

void slrParser::constructCanonicalCollection ()
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

slrParser::ItemSet* slrParser::ItemSetsClosure (const ItemSet& items, 
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
			if ( isNonTerminal ((itr->second).second) )
			{
				ruleIndex = slrGrammar.GrammarFindRule(
						(itr->second).second);
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
						}
					}
				}
			}
		}
	}

	return &result;
}
