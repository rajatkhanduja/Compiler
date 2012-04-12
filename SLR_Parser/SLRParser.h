// vim:ts=8:noexpandtab
/* This file declares the slrParser class and its methods. */
#ifndef SLR_PARSER_INCLUDED
#define SLR_PARSER_INCLUDED

#include <LR0Automaton.h>

class slrParser 
{
	public:
		// Public Data structures
		enum Action { None, Shift, Reduce, Accept};
		typedef pair<Action, Item*> ActionArgPair;

	private:
}

#endif	// End of file
