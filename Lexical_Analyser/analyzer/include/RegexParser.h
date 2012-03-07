#ifndef REGEX_PARSER_INCLUDED
#define REGEX_PARSER_INCLUDED

/* Class to parse regular expressions and generate a NFA for the 
 * expression. 
 */

#include <string>
#include <FSM.h>

using std::string;

class RegexParser
{
	public:
		RegexParser (string regex);
		int match (string pattern);	// true if pattern matches regexString.
		
	private:
		string inputRegexString;	// The string by user
		string regexString;		// Internal form.
		FSM fsm;
		
		// Private functions
		string internalRegex (string inputRegex);
		void generateFSM (FSM& fsm);
};

#endif // End of file
