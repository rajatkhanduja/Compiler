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
		void operator==(Grammar);
};
