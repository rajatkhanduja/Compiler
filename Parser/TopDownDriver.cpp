#include <TopDownDriver.hpp>

TopDownDriver::TopDownDriver()
{
	// Empty
}

void TopDownDriver::Drive(Grammar& CFG, char* tokenizedFile)
{
	ifstream in;
	in.open(tokenizedFile);
	int numTerminals = NTerminals();
	int numNonTerminals = NNonTerminals();
	int i;
	std::string Gsym;
	FirstSet firstSet;
	FollowSet followSet; 

	//############################# FIRST ##################################
	for ( i = 0; i < numTerminals; i++ )
	{
		Gsym = getTerminal(i);
		firstSet.First(Gsym, CFG);	
	}   	

	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		firstSet.First(Gsym, CFG);
	}


	firstSet.RemoveDuplicatesFromFollow();
	//#############################  FIRST ##################################

	//############################# FOLLOW #############################################	
	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		followSet.Follow(firstSet, Gsym, CFG);
		followSet.ProcessDependencyList();
	}

	followSet.RemoveDuplicatesFromFollow();
	//############################# FOLLOW #############################################
	
	NonRecursivePredictiveParser parser;
	std::string line;	

	while (in.good())
	{
		getline(in, line);
		parser.PrepareInput(line);
		parser.ParseInput(CFG);
		parser.ClearInput();	
	}
	
	in.close();	

}


int main(int argc, char** argv)
{
	assert(argc > 2);

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

	TopDownDriver topDownDriver;
	topDownDriver.Drive(G, argv[2]); 

	return 0;
}
