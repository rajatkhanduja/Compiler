// vim:ts=8:noexpandtab
/* This file declares the LR0Automaton class and its methods. 
 *
 * IMPORTANT :-
 * The rules for defining the grammar are the same as that of the LL1 grammar
 * as LR0Automaton simply uses the same class.The only thing that one needs to
 * keep in mind is that the LR0Automaton::augmentedStartSymbol (defined at the * end 
 * of this file) should not be used in the grammar file.
 */

#ifndef LR0AUTOMATON_INCLUDED
#define LR0AUTOMATON_INCLUDED

#include <Grammar.hpp>
#include <set>
#include <map>
#include <string>

using std::set;
using std::map;
using std::multimap;
using std::string;

typedef pair<vector<string>, vector <string> > ItemBody; 
typedef pair<string, ItemBody> Item;
typedef set<Item> ItemSet; 
typedef pair<ItemSet*, string> ItemTerminalPair;

void printItem (const Item& item);
void printItemSet (const ItemSet& items);

class SLRParser;

class LR0Automaton 
{
	friend class SLRParser;
	public:
		// Public static variables
		static const string augmentedStartSymbol;

		//Static functions
		/* Function that returns the first symbol that occurs after dot. */
		static string postDotSymbol (const Item& item);

		// Constructor
		LR0Automaton (char * grammarFileName);
		LR0Automaton (const Grammar& grammar);

		/* Function that returns the ItemSet with which the
		 * parsing begins. 
		 */
		ItemSet* startItemSet ();

		void printCanonicalCollection (void);
	private: 
		// Private (non-static) variables
		Grammar slrGrammar;
		set<ItemSet*> canonicalCollection;
		map<ItemTerminalPair, ItemSet*> GOTO;
		ItemSet startSet;

		// Private functions

		void initialize ();
		/* Function to compute the canonical collection using 
		 * slrGrammar. The result is stored in the variable 
		 * canonicalCollection.
		 */
		void constructCanonicalCollection ();
		
		/* Function that returns the value of GOTO for 
		 * ItemSet*I and symbol (terminal or non-terminal) X
		 */
		ItemSet* goTo (ItemSet* I, const string& X);
};
#endif	// End of file
