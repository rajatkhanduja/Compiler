#include <commons.hpp>
#include <xtoLL1.hpp>
#include <Terminal_NonTerminal.hpp>
using namespace std;

int main(int argc, char* argv[])
{
	assert(argc > 1);
	
	Grammar G_scanned;
	ScanGrammarFromFile(G_scanned, argv[1]);
	
	G_scanned.GrammarOutput();
	outputTerminals();
	outputNonTerminals();

	if(HasCycles(G_scanned))
		cout<<"The grammar has cycles."<<endl;
	cout<<endl;
	if(HasNonTerminatingRules(G_scanned))
		cout<<"The grammar has non-terminating rules."<<endl;
	
	return 0;
}

int foo()
{
	Rule* r2 = new Rule();
	r2->RuleOutput();
	vector<std::string> tail1;
	tail1.push_back("H");
	tail1.push_back("b");
	tail1.push_back("lsds");
	vector<std::string> tail2;
	tail2.push_back("LDKS");
	tail2.push_back("s");
	tail2.push_back("Glsds");
	vector<std::string> tail3;
	tail3.push_back("sdf");
	tail3.push_back("Ssdfs");
	tail3.push_back("q");
	Rule* r3 = new Rule("A");
	r3->RuleAddTail(tail1);
	r3->RuleAddTail(tail2);
	r3->RuleAddTail(tail3);
	r3->RuleOutput();
	cout<<r3->RuleNTails()<<endl;
	return 0;
}
