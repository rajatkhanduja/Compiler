// vim:ts=8:noexpandtab
/* This file defines the functions for Lexical Analyser class */

#include <LexicalAnalyser.h>
#include <cassert>

const string LexicalAnalyser::NoInputFileException = string ("No Input File");
const string LexicalAnalyser::NoMoreTokenException = string ("No More Token");

LexicalAnalyser::LexicalAnalyser ()
{
	inputFile = NULL;
}

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

void LexicalAnalyser::readRules (ifstream& rulesFile)
{
	string regex, token;
	while (true)
	{
		rulesFile >> regex;
		if ( rulesFile.eof())
			break;
		rulesFile >> token;
		addRule (regex, token);
		if ( rulesFile.eof())
			break;
  	}
}

void LexicalAnalyser::setInputFile (ifstream * inputFile)
{
	this->inputFile = inputFile;
	inputFileEOF = false;
	curLine = col = 0;
	line = NULL;
}

string LexicalAnalyser::getNextToken ()
{
	static bool lineEOF;
	if (NULL == inputFile)
	{
		throw NoInputFileException;
	}
	
	// Check if the 'line' variable holds any content.
	if (!line || lineEOF)
	{
		if (inputFileEOF)
		{	
			std::cerr << "No Token found\n";
			throw NoMoreTokenException;
		}
		
		string tmpLine;
		do
		{
			getline (*inputFile, tmpLine);
//			std::cerr << tmpLine.size ();
		} while (tmpLine.size () <= 0 && !inputFile->eof());

		if ( line )
		{
			delete line;
		}
		
		line = new istringstream (tmpLine);
		lineEOF = false;

		curLine++;
		
		if (inputFile->eof())
		{
			inputFileEOF = true;
		}
	}
	
	// If it does, read from it.
	string lexeme;
	*line >> lexeme;
	if (line->eof())
		lineEOF = true;
	if (lexeme.size () <= 0)
	{
		if (lineEOF)
		{
//			std::cerr << "[LEX] recursive call\n";
			return getNextToken();
		}
		else
		{
			assert (0);
		}
	}
	string token = tokenize (lexeme);

	col =  line->tellg();	// Line break because of compiler warning 
	col -= lexeme.length() - 1;
	addToSymbolTable (lexeme, token, curLine, col);
	
//	std::cerr << "[LexicalAnalyser] Found token : " << token << "\n";
	
	return token;
}
