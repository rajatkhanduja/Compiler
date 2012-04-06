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
	unsigned int row = 0, col;

	while (true)
	{
		getline (inputFile, line);
		row++;
		istringstream lexemeStream (line);
		while (lexemeStream >> lexeme)
		{
			token = lexer.tokenize (lexeme);
			std::cout << token << " ";

			if ( !token.compare ("ID"))
			{
				// Put IDs into symbol table
				col = lexemeStream.tellg ();
				col -= lexeme.length () - 1;
				lexer.addToSymbolTable (lexeme, token, row,
							col);
			}
		}
		std::cout << std::endl;

		if ( inputFile.eof())
			break;
	}

	return 0;
}
