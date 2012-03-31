// vim:ts=8:noexpandtab
/* The execution of the LexicalAnalyser starts in this file. 
 * 
 * Required arguments :-
 * arg1  :- lexical rules
 * arg2  :- file with input program
 */

#include <iostream>
#include <fstream>
#include <RegexParser.h>
#include <vector>
#include <FSM.h>
#include <LexicalAnalyser.h>

#define N_CMD_ARGS 2

using std::ifstream;

enum errorCodes { InsufficientArg = 1, InvalidArg };

using std::vector;

int main (int argc, char *argv[])
{
	LexicalAnalyser lexer;

	// Check number of arguments
	if (argc != N_CMD_ARGS)
	{
	return InsufficientArg;
	}

	ifstream rulesFile (argv[1]);
	ifstream inputFile (argv[2]);
	string regex, token;

	while (true)
	{
		rulesFile >> regex;
		rulesFile >> token;

		std::cerr << regex << " " << token << std::endl;

		lexer.addRule (regex, token);
    
		if ( rulesFile.eof())
			break;
  	}

	return 0;
}
