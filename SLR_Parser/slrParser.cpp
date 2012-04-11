// vim:ts=8:noexpandtab
/* This file defines the functions/methods of the slrParser class. */

#include <slrParser.h>
#include <xtoLL1.hpp>

slrParser::slrParser (char * grammarFileName)
{
	ScanGrammarFromFile (slrGrammar, grammarFileName);

	// Add the rule S' -> S to create the augmented grammar
	Rule startSymRule (augmentedStartSymbol);
	startSymRule.RuleAddTail (vector<string> (1, augmentedStartSymbol));
	slrGrammar.GrammarAddRule (startSymRule);
	slrGrammar.GrammarSetStartSymbol (augmentedStartSymbol);
}
