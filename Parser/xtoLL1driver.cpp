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

	if(G_scanned.GrammarHasEpsilonProductions())
		cerr<<"Grammar has epsilon productions."<<endl;
	
	G = G_scanned;
	G.GrammarOutput();

	EliminateDuplicateProductions(G);
	G.GrammarOutput();

	/*
	while(G_scanned.GrammarHasEpsilonProductions())
		EliminateEpsilonProductions(G_scanned);
	*/

	return 0;
}

int main_premerge(int argc, char* argv[])
{
	assert(argc > 1);
	
	Grammar G_scanned, G;
	ScanGrammarFromFile(G_scanned, argv[1]);
	G_scanned.GrammarOutput();
	outputTerminals();
	outputNonTerminals();

	assert(!HasCycles(G_scanned));
	assert(!HasNonTerminatingRules(G_scanned));

	if(G_scanned.GrammarHasEpsilonProductions())
		cerr<<"Grammar has epsilon productions."<<endl;
	
	G = G_scanned;
	G.GrammarOutput();

	EliminateDuplicateProductions(G);
	G.GrammarOutput();

	/*
	while(G_scanned.GrammarHasEpsilonProductions())
		EliminateEpsilonProductions(G_scanned);
	*/

	return 0;
}


