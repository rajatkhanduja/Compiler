#ifndef FSM_INCLUDED
#define FSM_INCLUDED

#include <list>
#include <string>

using std::list;
using std::string;

/*
 * IMPORTANT :
 * Following symbols are mapped to 'enum special' in fsm construction as shown
 *  '^'	-> EPSILON 
 *  '.'	-> DOT
 *  '%' -> SMALL_LETTERS {[a-z]}
 *  '$' -> CAPITAL_LETTERS {[A-Z]}
 *  '#' -> DIGITS
 */


class FSM
{
	
	public:
		enum special 
		{	
			NONE = 0,		// No special symbol
			DOT,
			EPSILON,
			CAPITAL_LETTERS,
			SMALL_LETTERS,
			DIGITS
		};	
		
		/* Change this constant if new symbol is added after DIGITS*/
		static const int MAX_ENUM = DIGITS;
	
	private:
		struct State;
		struct StateLink
		{
			char c;			// The character for which the state link exists
			special sym;		// enum special type indicating the special symbol being used like (dot) 
			State *nextState;	// The state to which it leads
			StateLink () { nextState = NULL; sym = NONE;}
			StateLink (State *nextState, char c)
			{
				this->nextState = nextState;
				this->c = c;
			};
			StateLink (State *nextState, special sym)
			{
				this->nextState = nextState;
				this->sym = sym;
			}
		};

		struct State
		{
			bool isFinalState;
			list <StateLink> links;	// List of links to other states.
			State () { isFinalState = false; }
			State * createTransition (char c, special sym, State * nextState = NULL);
		};

	public :
		FSM (char c, special sym = NONE);	// Construct FSM to match "c".
		FSM (FSM& fsm);			// (Shallow) Copy Constructor

		void operator += (FSM& rhs); 

		int simulate (string testString);

	private:
		State startState;
		State *acceptState;
};

#endif // End of file
