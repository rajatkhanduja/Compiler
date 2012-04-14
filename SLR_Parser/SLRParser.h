// vim:ts=8:noexpandtab
/* This file declares the slrParser class and its methods. */
#ifndef SLR_PARSER_INCLUDED
#define SLR_PARSER_INCLUDED

#include <LexicalAnalyser.h>
#include <LR0Automaton.h>
#include <stack>
#include <ParserFunctions.hpp>

using std::stack;

class SLRParser 
{
	public:
		// Public Data structures
		enum Action { None, Shift, Reduce, Accept};
		struct ActionVal 	// Unable to use Union
		{
			const Item* reduceRule;
			ItemSet* shiftTo;
		};
		typedef pair<Action, ActionVal> ActionArgPair;

		//Public static variables
		static const string UnexpectedTokenException;

		// Constructor
		SLRParser (char * lexFile, char * grammarFile);

		/* Call this function to parse. The output is printed directly.*/
		void parse (ifstream& inputFile);
		
		/* Function to return the actionTable as a string. 
		 * Useful to print the actionTable.
		 */
		string actionTable2String();

		/* Function to return the GOTO table as a string. */
		string gotoTable2String();

		/* Function to print the canonicalCollection of LR0Automaton */
		string canonicalCollection2String ();

		/* Returns the entire set of Follows as a string */
		string followSet2String();
	private:
		// Private variables
		map<ItemTerminalPair, ActionArgPair> ACTION; 
		map <ItemSet*, int> itemSetStates;	// Number for each itemset
		LR0Automaton lr0automaton;
		ItemSet * startSet;
		LexicalAnalyser lex;
		stack<ItemSet*> parseStack;
		FirstSet firstSet;
		FollowSet followSet;
		
		// Private functions

		/* Function to construct the ACTION table.
		 * This function populates the ACTION variable.
		 */
		void constructActionTable ();

		/* This function finally adds the ACTION to the table. 
		 * This function is also responsible for detecting if the grammar
		 * is not SLR(1). It throws an exception (still TODO).
		 */
		void addToActionTable (ItemSet* curItemSet, const string& Terminal,
					Action action, const Item* reduceRule,
					ItemSet* shiftTo);

		/* Function to generate a mapping from the ItemSets to numbers */
		void generateItemSet2NumMapping();

		/* Function to convert ActionArgPair to string. Requires
		 * access to the itemSetStates function, and hence needs to be 
		 * a part of the class.
		 */
		string actionArgPair2String(const ActionArgPair& pair);

		string getNextToken_();

};


#endif	// End of file
