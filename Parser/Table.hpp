#include "commons.hpp"
#include "Grammar.hpp"
#include "ParserFunctions.hpp"
#include <map>
using namespace std;

/*
Sequence Containers : 
Vector =>	Dynamic Arrays
List   =>	Doubly Linked Lists

Associative Containers :
Set		=>	Binary Search Trees ( Elements values themselves are the keys )
Multiset	=>	A 'Set' in which two keys can have the same value.
Map		=>	Key is different from the value like a Dict but unlike Set
Multimap	=>	A 'Map' but same key can have more than one value.
*/

/* We need to have delimiters between the terminals and the non-terminals in the
   input grammar file since else we can be short of the number of symbols 
   available for the non-terminals. 

   i.e. we would allow for 'Aa' to be a Non-Terminal and at the same time 'a' being
        a terminal.
*/ 

/* TEMPLATE ALERT */

/*	(1). Make the template definition visible to compiler in the point of instantiation.
 *	     i.e include the template DEFINITIONS in the header file itself. This MAY increase the size of the EXECUTABLE code.
 *	(2). Instantiate the types you need explicitly in a separate compile unit, so that linker can find it. 
 *	     i.e instantiate the types you will require in the template DEFINITION file so that the compiler can actually compile the REQUIRED definitions
 *	     and the linker can subsequently find them ( using their NAMES ).

 *	We will follow the second approach here. 
 **/

/* Key Structure for the Table => (rowIndex, columnIndex) */
template <class Row, class Column>
class TableKey
{
	private:
		Row rowIndex;		
		Column columnIndex;
	public:
		TableKey(Row rowIndex, Column columnIndex);
		void SetKey(Row rowIndex, Column columnIndex);
		pair<Row, Column> GetKey();
};


/* Table is implemented as a multimap (Associative Container) */
template<class TableKey>
class Table
{
	private:
		multimap<TableKey, Rule> table;

	public:
		Table();
		void TableInsert(TableKey key, Rule rule);
		vector<Rule> TableFind(TableKey key);
		void PopulateTable(Grammar CFG, FirstSet firstSet); 
};
