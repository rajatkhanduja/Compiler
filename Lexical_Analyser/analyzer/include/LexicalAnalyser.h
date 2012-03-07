/* File that declares the Lexical Analyser class and its functions */
#ifndef LEXICAL_ANALYSER_INCLUDED
#define LEXICAL_ANALYSER_INCLUDED

#include <RegexParser.h>
#include <SymbolTable.h>
#include <vector>
#include <map>

using std::vector;
using std::pair;

class LexicalAnalyser
{
	public:
		LexicalAnalyser ();

		// Function to add a rule.
		void addRule (const string regex, const string token);

		// Function to return the SymbolTable
		SymbolTable getSymbolTable () const;

		/* Tokenize an input string (lexeme)
		 * This function uses the given rules and returns
		 * the token. If there is no token, an empty string is
		 * returned.
		 */
		string tokenize (const string lexeme);

	private:
		vector <pair<RegexParser*, string> > lexicalRules;
		SymbolTable symTable;

		/* Vector is used instead of a map to allow the order of 
		 * importance be decided by the order of insertion of the
		 * rules. */
};

#endif	// End of file
