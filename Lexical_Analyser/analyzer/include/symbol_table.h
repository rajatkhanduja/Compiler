/* This file declares the SymbolTable class and its functions */
#ifndef SYMBOL_TABLE_INCLUDE
#define SYMBOL_TABLE_INCLUDE

#include <list>
#include <string>

using std::list;
using std::string;

class SymbolTable;

class SymbolTableElement
{
	friend class SymbolTable;

	public:
		// Constructor
		SymbolTableElement (const string lexeme, const string token,
			const unsigned int row, const unsigned int col);
		
		// Public functions
		void addPosition (unsigned int row, unsigned int col);
		string getToken () const;
		string getLexeme() const;


	private:
		struct Position
		{
			unsigned int row;
			unsigned int col;
		};
		list<Position> positions;
		string lexeme;
		string token;
		
};

class SymbolTable 
{
	public:
		// Public functions

		/* Function to find a lexeme. 
		 * It accepts a string (lexeme) and returns a 
		 * SymbolTableElement for the same.
		 */
		SymbolTableElement * findLexeme(const string lexeme);

		/* Function to insert a lexeme and token 
		 * This function first searches the SymbolTable using
		 * findLexeme and inserts a new element if necessary.
		 */
		void insert (const string lexeme, const string token, unsigned int row, unsigned int col);

	private:
		list<SymbolTableElement> index[26];	// Indices according to first character of the lexeme.

};

#endif	// End of file

