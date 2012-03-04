#include <iostream>
#include <regex_parser.h>
#include <fsm.h>

int main (int argc, char *argv[])
{
//	RegexParser r ("abc");

	FSM *fsm1  = new FSM ('c');
	FSM *fsm2  = new FSM ('a');

//	fsm1->concatenate (*fsm2);
//	fsm1 = fsm1->repeat ();

	*fsm1 |= *fsm2;

	std::cout << *fsm1;
	std::cout << fsm2->simulate ("a")<<std::endl;
	
	return 0;
}
