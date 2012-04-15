#include <commons.hpp>
#include <xtoLL1.hpp>
#include <Terminal_NonTerminal.hpp>
using namespace std;

int main(int argc, char* argv[])
{
	assert(argc > 1);
	
	Grammar G_scanned, G;
	ScanGrammarFromFile(G_scanned, argv[1]);
	cout<<"Grammar scanned from file."<<endl;
	G_scanned.GrammarOutput();
	outputTerminals();
	outputNonTerminals();

	assert(!HasCycles(G_scanned));
	cout<<"Grammar has been checked for no cycles."<<endl;
	assert(!HasNonTerminatingRules(G_scanned));
	cout<<"Grammar has been checked for no non-ternminating rules."<<endl;

	G = G_scanned;
	EliminateLeftRecursion(G);
	cout<<endl<<"Left recursion eliminated from the grammar."<<endl;
	G.GrammarOutput();

	LeftFactorize(G);
	cout<<"Grammar left factorized."<<endl;
	G.GrammarOutput();

	return 0;
}
