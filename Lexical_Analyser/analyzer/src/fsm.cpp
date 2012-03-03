#include <fsm.h>

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

FSM::FSM (char c, special sym)
{
	/* Construct a FSM for a character 'c' or special symbol. */
	acceptState = startState.createTransition (c, sym);
}

FSM::FSM (FSM& fsm)
{
	startState  = fsm.startState;
	acceptState = fsm.acceptState;
}

void FSM::operator += (FSM& rhs)
{
	/* Create an epsilon-transition from the acceptState of first fsm
	 * to that of the start state of the second fsm.
	 */
	acceptState->createTransition ('e', EPSILON, &(rhs.startState));

	/* The accept state of the second becomes the accept state of the 
	 * entire fsm.
	 */
	acceptState = rhs.acceptState;
}
