#include <commons.hpp>
#include <xtoLL1.hpp>
#include <Terminal_NonTerminal.hpp>
using namespace std;

int main(int argc, char* argv[])
{
	assert(argc > 1);
	
	Grammar G_scanned, G;
	ScanGrammarFromFile(G_scanned, argv[1]);
	G_scanned.GrammarOutput();
	outputTerminals();
	outputNonTerminals();

	assert(!HasCycles(G_scanned));
	assert(!HasNonTerminatingRules(G_scanned));

	G = G_scanned;
	EliminateLeftRecursion(G);
	G.GrammarOutput();

	return 0;
}
