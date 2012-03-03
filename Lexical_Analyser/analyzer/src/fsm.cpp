#include <fsm.h>
#include <iostream>

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

FSM::FSM (char c, special sym)
{
	/* Construct a FSM for a character 'c' or special symbol. */
	acceptState = startState.createTransition (c, sym);
	acceptState->isFinalState = true;
}

FSM::FSM (FSM& fsm)
{
	startState  = fsm.startState;
	acceptState = fsm.acceptState;
}

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

FSM * FSM::operator+= (FSM& fsm)
{
	this->concatenate (fsm);
	return this;
}

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

FSM * FSM::operator |= (FSM& rhs)
{
	/* Create a new (empty) start and end FSM */
	FSM *startFSM = new FSM();
	FSM *endFSM   = new FSM();

	/* Concatenate one of the two fsm-s with the start and end */
	this->concatenate (*endFSM);
	startFSM->concatenate (*this);

	/* Create a transition to the start state of other fsm */
	startFSM->startState.createTransition ('e', EPSILON, &(rhs.startState));
	/* Create a transition from the end state of other fsm */
	rhs.acceptState->createTransition ('e', EPSILON, &(endFSM->startState));

	return startFSM;
}
