#include "commons.hpp"
#include "Table.hpp"
#include "Grammar.hpp"

/* At each step of a top-down parse, the key-problem is that of determining the production to be applied for a non-terminal, say A. Once an A-production is
 * chosen, the rest of the parsing process consists of " MATCHING " THE TERMINAL SYMBOLS IN THE PRODUCTION BODY WITH THE INPUT STRING.
 *  
 * Whereas Recursive Descent Parsing may require backtracking to find the correct production, Predictive parsing is a special case of Recursive Descent Parsing
 * where no backtracking is required. 
 * 
 * Predictive parsing chooses the correct A-production by looking ahead at the input a fixed number of symbols.
 *
 * The class of grammars foe which we can construct predictive parsers looking k symbols ahead in the input is called the LL(k) class.
 *
 * During Top-Down Parsing, FIRST and FOLLOW help us to choose which production to apply, based on the NEXT input symbol.
 * */

class NonRecursivePredictiveParser
{
	private:
		string input;	/* '$' terminated string */
		Table <TableKey<string, string>> parsingTable;
		vector<string> parserStack;
	public:
		NonRecursivePredictiveParser();
		void ParseInput(Grammar CFG);
};
