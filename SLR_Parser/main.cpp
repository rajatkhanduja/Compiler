// vim:ts=8:noexpandtab
/* The execution of the slr Parser starts in this file. */
#include <iostream>
#include <SLRParser.h>

#define NARGS 4

using std::ifstream;
using std::ofstream;

string debugFile = "debug";

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
		
		// Open a file for dumping debugging statements.
		ofstream out(debugFile.c_str());
		out << slrParser.canonicalCollection2String ();
		out << "\n=============\n";
		out << slrParser.actionTable2String();
		out << "\n=============\n";
		out << slrParser.gotoTable2String();

		// Parse
		ifstream inputFile (argv[3]);
		slrParser.parse (inputFile);
		std::cerr << "Done\n";
	}
	catch (string exception)
	{
		std::cerr << exception << std::endl;
		std::cerr << "Exception\n";
	}

	return 0;
}
