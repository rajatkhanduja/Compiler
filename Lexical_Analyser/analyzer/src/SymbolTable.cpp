#include <SymbolTable.h>
#include <cctype>

SymbolTableElement::SymbolTableElement(string lexeme, string token, unsigned int row, unsigned int col)
{
	this->lexeme = lexeme;
	this->token  = token;
	this->addPosition (row, col);
}

void SymbolTableElement::addPosition (unsigned int row, unsigned int col)
{
	Position tmp_position;
	tmp_position.row = row;
	tmp_position.col = col;
	positions.push_back (tmp_position);
}

string SymbolTableElement::getToken () const
{
	return this->token;
}

string SymbolTableElement::getLexeme () const
{
	return this->lexeme;
}

static inline unsigned int tableIndex (const string& lexeme)
{
	return (tolower(lexeme[0] - 'a'));
}

SymbolTableElement * SymbolTable::findLexeme (const string lexeme)
{
	unsigned int n = tableIndex(lexeme);
	list <SymbolTableElement>::iterator iter, iter_end;

	for ( iter = index[n].begin(), iter_end = index[n].end(); iter != iter_end; iter++)
	{
		if ( iter->lexeme == lexeme)
		{
			return &(*iter);
		}
	}

	return NULL;
}

void SymbolTable::insert (const string lexeme, const string token, unsigned int row, unsigned int col)
{
	SymbolTableElement * element = findLexeme (lexeme);

	if (element != NULL)
	{
		element->addPosition (row, col);
	}
	else
	{
		index[tableIndex(lexeme)].push_back (SymbolTableElement(lexeme, token, row, col));
	}
}
