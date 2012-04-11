#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP
#include <commons.hpp>
#include <Rule.hpp>

using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

class Grammar
{
	private:
		std::string startSym;		// start symbol of the grammar.
		vector<Rule> rules;

	public:
		// constructor initializes startSym \
		to empty value. rules is not initialized.
		Grammar();
		// constructor initializes its class \
		members to the values passed in the \
		arguments.
		Grammar(std::string, vector<Rule>);
		// sets startSym to the head of the \
		first rule encountered in the .grammar \
		file being read.
		void GrammarSetStartSymbol(string startSymbol = string ("") );
		// returns startSym of the grammar.
		std::string GrammarStartSymbol();
		// adds the Rule in the argument at the \
		back of the vector of Rules if it is not \
		already present.
		void GrammarAddRule(Rule);
		// removes the rule at the index position \
		given in the argument if such a position \
		exists.
		void GrammarRemoveRule(int);
		// removes the Rule in argument if such a \
		Rule exists in the grammar.
		void GrammarRemoveRule(Rule);
		// looks for the argument in rules \
		returns the index of the rule if a \
		match is found.
		int GrammarFindRule(Rule);
		// looks for the argument in heads \
		of the rules in rules and returns \
		index of the rule if a match is found.
		int GrammarFindRule(std::string);
		// returns the rule at the index \
		position given in the argument if \
		such a position exists.
		Rule GrammarRule(int);
		// TODO remove this function as it is \
		no longer needed.
		vector<Rule> GrammarAllRules();
		// returns the number of rules in the \
		grammar.
		int GrammarNRules();
		// returns whether the grammar has any \
		rule with epsilon production(s) or not.
		bool GrammarHasEpsilonProductions();
		// prints out every rule in the grammar.
		void GrammarOutput();
		// compares two Grammars and returns \
		the truth value.
		bool operator==(Grammar*);
};

#endif
