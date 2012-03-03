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

	acceptState->createTransition ('e', EPSILON, &startState);
	acceptState->isFinalState = true;

	this->concatenate (*endFSM);
	startFSM->concatenate (*this);

	startFSM->startState.createTransition ('e', EPSILON, startFSM->acceptState);

	std::cerr << startFSM->startState.links[0].nextState->links[0].nextState->links[1].nextState<<std::endl;
	std::cerr << startFSM->startState.links[1].nextState->links.size ()<<std::endl;
	return startFSM;
}

void FSM::operator |= (FSM& rhs)
{
	std::cerr << this->startState.links[0].nextState->links[0].nextState->links[1].nextState <<std::endl;
	std::cerr << this->startState.links[1].nextState <<std::endl;
}
