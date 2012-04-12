/*
 * A rule has a structure:
 *	head: <string>
 *	tail(s): <vector of strings>
 * so that rules of the following type can be accomodated:
 *	A->Bc|cDB
 * (I don't think the datastructure Rule has to differentiate between terminals and non-terminals)
 * Rules of the following type cannot be accomodated (yet):
 *	AbB->cD|DDB
 */
#ifndef RULE_HPP
#define RULE_HPP
#include <commons.hpp>
#include <Terminal_NonTerminal.hpp>
using namespace std;

class Rule
{
	private:
		std::string head;
		vector<vector<std::string> > tails;
	
	public:
		// constructor initializes class members to \
		empty values.
		Rule();
		// constructor initializes the head of the \
		rule to the argument passed. tails is not \
		initialized.
		Rule(const std::string);
		// returns the head of the rule.
		std::string RuleHead();
		// returns the tail of the rule at the \
		index position of the argument passed.

		//#TODO
		void RuleCleanAll(const std::string head);
		//#TODO

		vector<std::string> RuleTail(int);
		// returns all the tails of the rule.
		vector<vector<std::string> > RuleTails();
		// adds the argument passed as a tail \
		at the back of the tails if it does not \
		exist already.
		void RuleAddTail(vector<std::string>);
		// looks for the presence of the argument \
		in the tails and removes it if a match is found.
		void RuleRemoveTail(vector<std::string>);
		// the tail corresponding to the index \
		position in the argument is removed if \
		such a position exists.
		void RuleRemoveTail(int);
		// looks for the argument in tails and \
		returns the index of the tail if a match \
		is found.
		int RuleFindTail(vector<std::string>);
		// returns the number of tails the rule has.
		int RuleNTails();
		// outputs the rule.
		void RuleOutput();
		// returns the index of an epsilon production \
		in tails of the rule.
		int RuleFindEpsilonProduction();
		// checks whether the tail passed as index is a \
		terminal production or not.
		bool RuleIsTerminalProduction(int);
		// returns whether the rule has a terminal \
		production in tails of the rule or not. \
		A terminal production is a production consisting \
		entirely of terminals.
		bool RuleHasTerminalProduction();
		// checks whether the tail passed as index is a \
		left recursive production or the rule or not.
		bool RuleIsLeftRecursiveProduction(int);
		// compares two vector<std::string>s \
		and returns the truth value.
		bool equals(vector<std::string>, vector<std::string>);
		// compares two Rules and returns the \
		truth value.
		bool operator==(Rule);
};

#endif
