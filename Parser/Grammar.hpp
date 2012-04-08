#include <commons.hpp>
#include <Rule.hpp>
#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

using namespace std;

class Grammar
{
	private:
		std::string startSym;		// start symbol of the grammar.
		vector<Rule> rules;

	public:
		Grammar();
		Grammar(vector<Rule>);
		void GrammarSetStartSymbol();
		std::string GrammarStartSymbol();
		void GrammarAddRule(Rule);
		void GrammarRemoveRule(int);
		void GrammarRemoveRule(Rule);
		int GrammarFindRule(Rule);
		int GrammarFindRule(std::string);
		Rule GrammarRule(int);
		vector<Rule> GrammarAllRules();
		int GrammarNRules();
		bool GrammarHasEpsilonProductions();
		void GrammarOutput();
		bool operator==(Grammar*);
};

#endif
