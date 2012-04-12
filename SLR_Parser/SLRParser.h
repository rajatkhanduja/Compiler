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
		typedef pair<Action, Item*> ActionArgPair;

		// Constructor
		SLRParser (char * grammarFile);

		/* Call this function to parse. The output is printed directly.*/
		void parse ();
	private:
		// Private variables
		map<ItemTerminalPair, ActionArgPair> ACTION; 
		LR0Automaton lr0automaton;
		ItemSet * startSet;
		LexicalAnalyser lex;
		
		// Private functions

		/* Function to construct the ACTION table.
		 * This function populates the ACTION variable.
		 */
		void constructActionTable ();
};

#endif	// End of file
