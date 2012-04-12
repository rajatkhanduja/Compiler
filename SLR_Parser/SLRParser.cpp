// vim:ts=8:noexpandtab
/* This file defines the slrParser class. */
#include <SLRParser.h>

SLRParser::SLRParser (char * grammarFile) : lr0automaton (grammarFile)
{
	startSet = lr0automaton.startItemSet ();
	constructActionTable ();
}

void SLRParser::constructActionTable ()
{
	
}
