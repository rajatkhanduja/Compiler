// vim:ts=8:noexpandtab
/* This file declares the slrParser class and its methods. */
#ifndef SLR_PARSER_INCLUDED
#define SLR_PARSER_INCLUDED

#include <LexicalAnalyser.h>
#include <LR0Automaton.h>

class SLRParser 
{
	public:
		// Public Data structures
		enum Action { None, Shift, Reduce, Accept};
		struct ActionVal 	// Unable to use Union
		{
			Rule reduceRule;
			ItemSet* shiftTo;
		};
		typedef pair<Action, ActionVal> ActionArgPair;

		// Constructor
		SLRParser (char * grammarFile);

		/* Call this function to parse. The output is printed directly.*/
		void parse ();
	private:
		// Private variables
		map<ItemTerminalPair, ActionArgPair> ACTION; 
		LR0Automaton lr0automaton;
		ItemSet * startSet;
		ItemSet * curSet;
		LexicalAnalyser lex;
		
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
					Action action, Rule* reduceRule,
					ItemSet* shiftTo);
};

#endif	// End of file
