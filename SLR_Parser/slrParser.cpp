// vim:ts=8:noexpandtab
/* This file defines the functions/methods of the slrParser class. */

#include <slrParser.h>
#include <xtoLL1.hpp>

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

/* Function to convert return an item given a rule. */
slrParser::Item slrParser::rule2Item (Rule& rule)
{
	return (make_pair (rule.RuleHead(), 
			make_pair (string(),rule.RuleTail(0)[0])));
}

void slrParser::constructCanonicalCollection ()
{
	// Start with inserting the set of item containing only S'->S
	ItemSet tmpSet;
	tmpSet.insert (	rule2Item (*startRule));
									
}
