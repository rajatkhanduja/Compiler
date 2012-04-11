// vim:ts=8:noexpandtab
/* This file declares the slrParser class and its methods. 
 *
 * IMPORTANT :-
 * The rules for defining the grammar are the same as that of the LL1 grammar
 * as slrParser simply uses the same class. The only thing that one needs to 
 * keep in mind is that the slrParser::augmentedStartSymbol (defined at the end 
 * of this file) should not be used in the grammar file.
 */

#ifndef SLR_PARSER
#define SLR_PARSER

#include <Grammar.hpp>
#include <set>
#include <map>
#include <string>

using std::set;
using std::map;
using std::multimap;
using std::string;

class slrParser 
{
	public:
		// Constructor
		slrParser (char * grammarFileName);

		/* Function to parse, prints the output */
		void parse ();

	private: 
		// Private Data structures
		enum Action { None, Shift, Reduce, Accept};
		
		typedef pair<string, string> ItemBody; 
		typedef pair<string, ItemBody> Item;
		typedef set<Item> ItemSet; 
		typedef pair<Item*, string> ItemTerminalPair;
		typedef pair<Action, Item*> ActionArgPair;

		// Private static variables
		static const string augmentedStartSymbol;

		// Private (non-static) variables
		Grammar slrGrammar;
		set<ItemSet> canonicalCollection;
		map<ItemTerminalPair, Item*> GOTO;
		map<ItemTerminalPair, ActionArgPair> ACTION; 

		// Private functions

		/* Function to compute the canonical collection using slrGrammar.
		 * The result is stored in the variable canonicalCollection.
		 */
		void constructCanonicalCollection ();
		
		/* Function to convert a rule to an Item */
		Item rule2Item (Rule& rule);
		
		ItemSet * ItemSetsClosure (ItemSet& items, ItemSet);

};

// Declare the augmentedStartSymbol to be complex so that it 
// is highly unlikely that it is used in the grammar.
const string slrParser :: augmentedStartSymbol = "StArT5yMb0l";	

#endif	// End of file
