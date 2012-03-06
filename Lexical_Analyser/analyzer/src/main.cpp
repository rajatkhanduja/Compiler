#include <iostream>
#include <regex_parser.h>
#include <vector>
#include <fsm.h>

using std::vector;

int main (int argc, char *argv[])
{
	RegexParser r ("ab*c");
	
	std::cout << r.match("ac") << std::endl;

	FSM *fsm1  = new FSM ('b');
	FSM *fsm2  = new FSM ('c');
	
	fsm1 = fsm1->repeat();
	fsm1->concatenate (*fsm2);

	fsm2 = new FSM('a');
	fsm2->concatenate (*fsm1);
	
	std::cout << fsm2->simulate ("abbbbc")<<std::endl;

	return 0;
}
