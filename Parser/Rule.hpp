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

#include <commons.hpp>
using namespace std;

class Rule
{
	private:
		std::string head;
		vector<std::string> tail;
	
	public:
		Rule();
		Rule(const std::string, vector<std::string>);
		std::string RuleHead();
		vector<std::string> RuleTail();
		void RuleAddTail(std::string);
		void RuleRemoveTail(std::string);
		void RuleRemoveTail(int);
		int RuleFindTail(const std::string);
		int RuleNTails();
		void RuleOutput();
		bool operator==(Rule);
};
