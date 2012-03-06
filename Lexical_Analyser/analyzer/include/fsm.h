#ifndef FSM_INCLUDED
#define FSM_INCLUDED

#include <list>
#include <string>
#include <set>
#include <iostream>

using std::list;
using std::string;
using std::set;

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
				this->sym = NONE;
			};
			StateLink (State *nextState, special sym)
			{
				this->nextState = nextState;
				this->sym = sym;
			}

			bool accept (const char c) const;
		//	~StateLink();
		};


		struct State
		{
			bool isFinalState;
			list <StateLink> links;	// List of links to other states.
			State () { isFinalState = false; }
			State * createTransition (char c, special sym, State * nextState = NULL);
			set<State*> epsilonClosure ();
			set<State*> move (char c);
		};

	public :
		FSM ();
		FSM (const char c, const special sym = NONE);	// Construct FSM to match "c".
		FSM (const FSM& fsm);		// (Shallow) Copy Constructor
	//	~FSM();
		FSM * operator += (FSM& rhs); 
		void concatenate (FSM& fsm);	// Same as +=
		FSM * repeat ();		// Modify the fsm to accept repetitions of accepted string. '*' operator.
		FSM * operator | (FSM& rhs);

		/* Simulate the NFA */
		int simulate (const string testString);

		/* Stream operators */
		friend std::ostream& operator << (std::ostream& o, const FSM& fsm);

	private:
		State startState;
		State *acceptState;

		set<State*> epsilonClosure (const set<State*>& curStates);
		set<State*> move (set <State*>& curStates, const char c);
};

#endif // End of file
