#include <commons.hpp>
#include <Rule.hpp>
using namespace std;

class Grammar
{
	private:
		vector<Rule> rules;
		/* Added by Bharat Khatri */
		map<string, bool> isTerm;
		string startSym;	// Start Symbol.
		/* Added by Bharat Khatri */
	
	public:
		Grammar();
		Grammar(vector<Rule>);
		void GrammarAddRule(Rule);
		void GrammarRemoveRule(int);
		void GrammarRemoveRule(Rule);
		int GrammarFindRule(Rule);
		/* Added by Bharat Khatri */
		vector<Rule> GrammarFindRule(string head);
		vector<Rule> GrammarAllRules();
		/* Added by Bharat Khatri */
		int GrammarNRules();
		void GrammarOutput();
		bool operator==(Grammar*);
};
