// vim:ts=8:noexpandtab
/* The execution of the slr Parser starts in this file. */
#include <iostream>
#include <SLRParser.h>

#define NARGS 4

using std::ifstream;

void usage (char *name)
{
	std::cerr << "Usage :-\n";
	std::cerr << name << " lexicalRulesFile"
			  << " grammarRulesFile"
			  << " inputFile\n";
	return;			  
}

int main (int argc, char * argv[])
{
	if (argc < NARGS )
	{
		std::cerr << "Some arguments missing\n";
		usage (argv[0]);
		exit (1);
	}
	try
	{
		SLRParser slrParser (argv[1], argv[2]);
		std::cout << slrParser.canonicalCollection2String ();

		ifstream inputFile (argv[3]);
		slrParser.parse (inputFile);
	}
	catch (string exception)
	{
		std::cerr << exception << std::endl;
	}

	return 0;
}
