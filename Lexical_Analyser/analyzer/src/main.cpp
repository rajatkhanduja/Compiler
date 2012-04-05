// vim:ts=8:noexpandtab
/* The execution of the LexicalAnalyser starts in this file. 
 * 
 * Required arguments :-
 * arg1  :- lexical rules
 * arg2  :- file with input program
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <RegexParser.h>
#include <vector>
#include <FSM.h>
#include <LexicalAnalyser.h>

#define N_CMD_ARGS 3

using std::ifstream;
using std::vector;
using std::istringstream;

enum errorCodes { InsufficientArg = 1, InvalidArg };


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

	string lexeme, line;

	while (true)
	{
		getline (inputFile, line);
		istringstream lexemeStream (line);
//		std::cout << line << std::endl;
		while (lexemeStream >> lexeme)
		{
			std::cout << lexer.tokenize (lexeme) << " ";
		}
		std::cout << std::endl;

		if ( inputFile.eof())
			break;
	}

	return 0;
}
