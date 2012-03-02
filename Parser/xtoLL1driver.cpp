#include <commons.hpp>
#include <Grammar.hpp>
using namespace std;

int main()
{
	Rule* r2 = new Rule();
	r2->RuleOutput();
	vector<std::string> temp;
	temp.push_back("HKD");
	temp.push_back("bLK");
	temp.push_back("lsdskKJL");
	Rule* r3 = new Rule("A", temp);
	r3->RuleOutput();
	cout<<r3->RuleNTails()<<endl;
	return 0;
}
