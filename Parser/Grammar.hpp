#include <commons.hpp>
#include <Rule.hpp>
using namespace std;

class Grammar
{
	private:
		vector<Rule> rules;
	
	public:
		Grammar();
		Grammar(vector<Rule>);
		void GrammarAddRule(Rule);
		void GrammarRemoveRule(int);
		void GrammarRemoveRule(Rule);
		int GrammarFindRule(Rule);
		int GrammarFindRule(std::string);
		Rule GrammarRule(int);
		//vector<Rule> GrammarAllRules();	I am unable to comprehend the use of this function.
		int GrammarNRules();
		void GrammarOutput();
		bool operator==(Grammar*);
};
