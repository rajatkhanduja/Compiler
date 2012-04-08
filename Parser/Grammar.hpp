#include <commons.hpp>
#include <Rule.hpp>
using namespace std;

class Grammar
{
	private:
		vector<Rule> rules;

		//#TODO Added by Bharat Khatri
		
		// We need to fill this when the Grammar object is initialized; i.e in the constructor.
		// This is a simple map comprising of all the symbols in all the rules ( terminals + non-terminals ).
		// It's structure would be ( <symbol-name>, [true,false] ). 
		// The second field would be boolean true if <symbol-name> is a terminal or boolean false otherwise.
		//
		// NOTE :: epsilon i.e '#' and end marker i.e '$' are neither TERMINALS nor NON-TERMINALS and should be kept out of the 'isTerm' data structure.
		map<string, bool> isTerm;
		string startSym;		// start symbol of the grammar.
		
		//# Added by Bharat Khatri
	
	public:
		Grammar();
		Grammar(vector<Rule>);
		void GrammarAddRule(Rule);
		void GrammarRemoveRule(int);
		void GrammarRemoveRule(Rule);
		int GrammarFindRule(Rule);
		int GrammarFindRule(std::string);
		Rule GrammarRule(int);
		
		// Added by Bharat Khatri
		
		// This would return a vector of all the rules in the grammar so that they could be traversed in a sequence.
		vector<Rule> GrammarAllRules()
		{
			return this->rules;
		}

		// Added by Bharat Khatri
		
		int GrammarNRules();
		void GrammarOutput();
		bool operator==(Grammar*);
};
