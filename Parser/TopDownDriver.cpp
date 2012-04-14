#include <TopDownDriver.hpp>

TopDownDriver::TopDownDriver()
{
	// Empty
}

void TopDownDriver::Drive(Grammar& CFG, char* tokenizedFile)
{
	ifstream in;
	in.open(tokenizedFile);
	
	std::cerr << "File to parse " << tokenizedFile << "\n";

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


	firstSet.RemoveDuplicatesFromFirst();

	std::cerr << "FIRST computation done.\n";
	//#############################  FIRST ##################################

	//############################# FOLLOW #############################################	
	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		followSet.Follow(firstSet, Gsym, CFG);

		// To handle right recursion
		followSet.ProcessDependencyList();
	}

	followSet.RemoveDuplicatesFromFollow();

	std::cerr << "FOLLOW coputation done.\n";
	//############################# FOLLOW #############################################

	NonRecursivePredictiveParser parser(CFG, firstSet, followSet);
	parser.PrintTable();

	std::string line;	

	std::cerr << "Parsing started\n";

	while (in.good())
	{
		getline(in, line);
		std::cerr << "At line :: " << line << "\n";
		parser.PrepareInput(line);
		//parser.ParseInput(CFG);
		parser.ClearInput();	
	}

	std::cerr << "Parsing Complete\n";	
	in.close();	

}


int main(int argc, char* argv[])
{
	assert(argc > 2);
	
	Grammar G_scanned, G;
	G.GrammarSetStartSymbol("E");	
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
