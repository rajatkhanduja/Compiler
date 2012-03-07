#include <FSM.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
FSM::State * FSM::State::createTransition (char c, special sym, State *nextState)
{
	/* Here, it is assumed that there is only one accept state */
	isFinalState = false;

	if (NULL == nextState)
		nextState = new State;
	
	StateLink transition;

	if (NONE == sym)
		links.push_back (StateLink (nextState, c));
	else
		links.push_back (StateLink (nextState, sym));

	return nextState;
}

FSM::FSM ()
{
	startState.isFinalState = true;
	acceptState  = &startState;
}

FSM::FSM (const char c, const special sym)
{
	/* Construct a FSM for a character 'c' or special symbol. */
	acceptState = startState.createTransition (c, sym);
	acceptState->isFinalState = true;
}

FSM::FSM (const FSM& fsm)
{
	startState  = fsm.startState;
	acceptState = fsm.acceptState;
}

/* Function to concatenate the passed FSM. */
void FSM::concatenate (FSM& rhs)
{
	/* Create an epsilon-transition from the acceptState of first fsm
	 * to that of the start state of the second fsm.
	 */
	acceptState->createTransition ('e', EPSILON, &(rhs.startState));
	acceptState->isFinalState = false;

	/* The accept state of the second becomes the accept state of the 
	 * entire fsm.
	 */
	acceptState = rhs.acceptState;
	rhs.acceptState->isFinalState = true;
}

/* This function is an alternative to concatenate(FSM &) */
FSM * FSM::operator+= (FSM& fsm)
{
	this->concatenate (fsm);
	return this;
}

/* This function returns the fsm that accepts the repetition of strings 
 * accepted by given fsm.
 */
FSM * FSM::repeat ()
{
	/* Create a new (empty) FSM. */
	FSM *startFSM = new FSM();
	FSM *endFSM   = new FSM();

	/* Loop back from the accept state of the fsm to the start state */
	acceptState->createTransition ('e', EPSILON, &startState);
	acceptState->isFinalState = true;

	/* Concatenate the endFSM state with fsm */
	this->concatenate (*endFSM);
	/* Concatenate the startFSM with 'this' fsm */
	startFSM->concatenate (*this);

	/* Create a epsilon-transition from startFSM to endFSM */
	startFSM->startState.createTransition ('e', EPSILON, startFSM->acceptState);

	return startFSM;
}

/* This function rerturns the fsm to be equivalent to (fsm1|fsm2) 
 * XXX : Use with caution. This modifies fsm1, but the return value must be 
 * stored back in fsm1 for this to be effective.
 *
 * DO NOT try something like "fsm1 | fsm1", this will ave 
 */
FSM * FSM::operator | (FSM& rhs)
{
	/* Create a new (empty) start and end FSM */
	FSM *startFSM = new FSM();
	FSM *endFSM   = new FSM();

	/* Concatenate one of the two fsm-s with the start and end */
	this->concatenate (*endFSM);
	startFSM->concatenate (*this);

	/* Create a transition to the start state of other fsm */
	rhs.concatenate (*endFSM);

	/* Create a transition from the end state of other fsm */
	startFSM->startState.createTransition ('e', EPSILON, &(rhs.startState));
	return startFSM;
}

/* Function to find the epsilon closure of a state. */
set<FSM::State*> FSM::State::epsilonClosure ()
{
	set <State*> newStates;
	list<StateLink>::iterator itr;

	newStates.insert (this);

	for ( itr = this->links.begin (); itr != this->links.end (); itr++)
	{
		if ( EPSILON == itr->sym )
		{
			newStates.insert (itr->nextState);
		}
	}

	return newStates;
}

/* Function to find the epsilon closure of a set of states */
set<FSM::State*> FSM::epsilonClosure (const set<FSM::State*> &curStates)
{
	set<State*> newStates, tempStates;

	set<State*>::iterator itr;
	
	for (itr = curStates.begin (); itr != curStates.end (); itr++)
	{
		tempStates = (*itr)->epsilonClosure();
		newStates.insert (tempStates.begin(), tempStates.end());
	}

	tempStates.clear ();

	set <State*> tempStates2, tempStates3;

	/* Here onwards curStates is used just as another variable */
	tempStates2 = newStates;
	do
	{
		for (itr = tempStates2.begin(); itr != tempStates2.end (); itr++)
		{
			tempStates = (*itr)->epsilonClosure();
			
			set<State*>::iterator itr2;

			for (itr2 = tempStates.begin(); itr2 != tempStates.end(); itr2++)
			{
				if ( *itr == *itr2 )
				{
					continue;
				}

				if ( newStates.find (*itr2) == newStates.end() )
				{
					newStates.insert (*itr2);
					tempStates3.insert (*itr2);
				}
			}
		}
		tempStates2 = tempStates3;
		tempStates3.clear ();
	}while (!tempStates2.empty ());
	
	return newStates;
}

/* Function to check if the transition is valid for the character */
bool FSM::StateLink::accept (const char c) const
{
	if (NONE == sym)
	{
		if (this->c == c)
			return true;
		else
			return false;
	}
	else
	{
		/* Check for operator matches */
		switch (sym)
		{
			case EPSILON         : return false;
			case DOT             : return true;
			case SMALL_LETTERS   : return (islower (c));
			case CAPITAL_LETTERS : return (isupper (c));
			case DIGITS          : return (isdigit (c));
			default              : assert (0); return false;
		}
	}
}

/* Function returns the set of states that are reached by traversing 
 * the links that match the character from the state.
 */
set<FSM::State*> FSM::State::move (char c)
{
	list <StateLink>::iterator itr, itr_end;
	set<State*> nextStates;

	for (itr = links.begin(), itr_end = links.end(); itr != itr_end; itr++)
	{
		if (itr->accept (c))
		{
			nextStates.insert (itr->nextState);
		}
	}

	return nextStates;
}

/* Function that returns the set obtained by moving on the NFA from the 
 * start states taken from the given curStates.
 */
set<FSM::State*> FSM::move (set <FSM::State*>& curStates, const char c)
{
	set<FSM::State*> nextStates, tempStates;
	set<FSM::State*>::iterator itr, itr_end;

	for (itr = curStates.begin (), itr_end = curStates.end (); itr != itr_end; itr++)
	{
		tempStates = (*itr)->move (c);
		nextStates.insert (tempStates.begin(), tempStates.end());
	}
	
	return nextStates;
}


/* Returns -1 for no match.
 * Returns  0 if 'empty' string matches.
 * Returns  length of match for a string match
 */
int FSM::simulate (const string testString)
{
	set <State*> curStates = epsilonClosure (startState.epsilonClosure());
	set <State*> newStates;

	unsigned int i, longestMatch = -2;
	
	// In case empty string matches.
	if (find (curStates.begin(), curStates.end(), acceptState) != curStates.end())
	{
		longestMatch = -1;
	}
	
	/* Traverse the entire string */
	for ( i = 0; i < testString.length (); i++)
	{
		/* Find the new states by moving on the NFA and using epsilon
		 * closure. */
		newStates = epsilonClosure (move (curStates, testString[i]));
		if (newStates.count (acceptState))
		{
			longestMatch = i;
		}

		curStates = newStates;
	}

	return (longestMatch + 1);
}

std::ostream& operator << (std::ostream& o, const FSM& fsm)
{
	/* Print a transition table of sorts */
	vector <const FSM::State*> statesList;

	unsigned int i;

	statesList.push_back (&(fsm.startState));
	

	for ( i = 0; i < statesList.size(); i++)
	{
		const FSM::State * addr = statesList[i];
		list <FSM::StateLink>::const_iterator itr;

		for (itr = addr->links.begin (); itr != addr->links.end (); itr++)
		{
			o << addr << " - ";

			if (FSM::NONE == itr->sym)
				o << itr->c;
			else
			{	
				switch (itr->sym)
				{
					case FSM::EPSILON         : o << "(e)"; break;
					case FSM::DOT             : o << "(.)"; break;
					case FSM::SMALL_LETTERS   : o << "(%)"; break;
					case FSM::CAPITAL_LETTERS : o << "($)"; break;
					case FSM::DIGITS          : o << "(#)"; break;
					default                   : o << itr->sym << " " << itr->c; assert(0) ;
				}
			}
		
			o << " - " << itr->nextState << std::endl;
			if (find (statesList.begin(), statesList.end(), itr->nextState) == statesList.end ())
			{
				statesList.push_back (itr->nextState);
			}
		}

	}

	o << "Final state : " << fsm.acceptState << std::endl;

	return o;
}

/*
FSM::StateLink::~StateLink()
{
	
}
*/

/* FSM Destructor. This should ensure that only the reference to the 
 * states is deleted and the states themselves are left intact.
 * This is required as destructors are called often when using vectors, 
 * whenever it is popped out of the vector ( required in RegexParser).
 */
/*
FSM::~FSM ()
{
	
} */
