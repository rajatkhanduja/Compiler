// vim:ts=8:noexpandtab
/* This file defines the functions for Lexical Analyser class */

#include <LexicalAnalyser.h>

void LexicalAnalyser::addRule (const string regex, const string token)
{
	RegexParser * parser = new RegexParser (regex);
	
	lexicalRules.push_back (make_pair(parser, token));
}

string LexicalAnalyser::tokenize (const string& lexeme)
{
	vector <pair<RegexParser*, string> >::iterator itr, itr_end;

	int longestMatch = -1, tmp;
	string token("");
	
	for ( itr = lexicalRules.begin(), itr_end = lexicalRules.end(); itr != itr_end; itr++)
	{
		tmp = itr->first->match (lexeme);

		if ( tmp >= 0 )
		{
			if (tmp > longestMatch)
			{
				longestMatch = tmp;
				token = itr->second;
			}
		}
	
	}

	if (!token.compare (""))
		return lexeme;	
	else
		return token;
}

list <SymbolTableElement::Position> 
LexicalAnalyser::getPositions (const string& lexeme)
{
	return symTable.getPositions (lexeme);
}

void LexicalAnalyser::addToSymbolTable (const string& lexeme, const string& token, const int& line, const int& col)
{
	symTable.insert (lexeme, token, line, col);
}

