// vim:ts=8:noexpandtab
/* This file defines the slrParser class. */
#include <SLRParser.h>

SLRParser::SLRParser (char * grammarFile) : lr0automaton (grammarFile)
{
	constructActionTable ();
}

void SLRParser::constructActionTable ()
{
	
}
