#include <iostream>
#include <RegexParser.h>
#include <vector>
#include <FSM.h>

using std::vector;

int main (int argc, char *argv[])
{
	RegexParser r ("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|z)*");
	
	std::cout << r.match("abc") << std::endl;

	FSM *fsm1  = new FSM ('b');
	FSM *fsm2  = new FSM ('c');
	
	fsm1 = fsm1->repeat();
	fsm1->concatenate (*fsm2);

	fsm2 = new FSM('a');
	fsm2->concatenate (*fsm1);
	
	std::cout << fsm2->simulate ("abbbbc")<<std::endl;

	return 0;
}
