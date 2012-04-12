// vim:ts=8:noexpandtab
/* The execution of the slr Parser starts in this file. */
#include <iostream>
#include <SLRParser.h>

#define NARGS 4

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

	SLRParser slrParser (argv[1], argv[2]);

	return 0;
}
