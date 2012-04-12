#include <TopDownDriver.hpp>

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
	//#############################  FIRST ##################################

	//############################# FOLLOW #############################################	
	for ( i = 0; i < numNonTerminals; i++ )
	{
		Gsym = getNonTerminal(i);
		followSet.Follow(firstSet, Gsym, CFG);
	}
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
	string filename("");
	Grammar CFG;
	TopDownDriver topDownDriver;
	topDownDriver.Drive(CFG, (char *)filename.c_str());		

	return 0;
}
