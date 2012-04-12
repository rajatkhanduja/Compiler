// vim:ts=8:noexpandtab
/* File that declares the Lexical Analyser class and its functions */
#ifndef LEXICAL_ANALYSER_INCLUDED
#define LEXICAL_ANALYSER_INCLUDED

#include <RegexParser.h>
#include <SymbolTable.h>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using std::istringstream;
using std::vector;
using std::pair;
using std::ifstream;
using std::set;

class LexicalAnalyser
{
	public:
		// Public variables
		static const string NoInputFileException;
		static const string NoMoreTokenException;

		LexicalAnalyser ();

		// Function to add a rule.
		void addRule (const string regex, const string token);

		// Function to add a token and lexeme to symbol table
		void addToSymbolTable (const string& lexeme,
				       const string& token, 
				       const int& line, const int& col);

		// Function to get the positions of a symbol in symbol table
		list <SymbolTableElement::Position> 
			getPositions (const string& lexeme);

		// Function to return the SymbolTable
	//	SymbolTable getSymbolTable () const;

		/* Tokenize an input string (lexeme)
		 * This function uses the given rules and returns
		 * the token. If there is no token, input string is
		 * returned.
		 */
		string tokenize (const string& lexeme);

		/* Function to read the rules from a file. */
		void readRules (ifstream& rulesFile);

		/* Function to set the input file. */
		void setInputFile (ifstream * inputFile);

		/* Function to get the next token. This function throws 
		 * exception NoInputFileException when this function is
		 * called without setting up the inputFile. This throws the 
		 * NoMoreTokenException if there are no more tokens.
		 * The token is also inserted into the symbol table.
		 */
		 string getNextToken();


	private:
		SymbolTable symTable;
		ifstream * inputFile;
		bool inputFileEOF;
		unsigned int curLine, col;
		istringstream* line;

		/* Vector is used instead of a map to allow the order of 
		 * importance be decided by the order of insertion of the
		 * rules. */
		vector <pair<RegexParser*, string> > lexicalRules;
};

#endif	// End of file
