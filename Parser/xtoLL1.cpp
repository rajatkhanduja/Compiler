#include <Rule.hpp>
using namespace std;

int main()
{
	Rule r1, r2, r3;
	r1.RuleOutput();
	r2 = new Rule();
	r2.RuleOutput();
	vector<std::string> temp;
	temp.push_back("HKD");
	temp.push_back("bLK");
	temp.push_back("lsdskKJL");
	r3 = new Rule("A", temp);
	r3.RuleOutput();
	return 0;
}
