#include <iostream>
#include <regex_parser.h>
#include <fsm.h>

int main (int argc, char *argv[])
{
	RegexParser r ("abc");

	FSM fsm1 ('c');
	FSM fsm2 ('a');

	fsm1 += fsm2;
	
	return 0;
}
