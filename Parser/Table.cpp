#include "xtoLL1.hpp"
#include "Table.hpp"

using namespace std;

template<class Row, class Column>
TableKey<Row, Column>::TableKey(Row rowIndex, Column columnIndex)
{
	this.rowIndex = rowIndex;
	this.columnIndex = columnIndex;
}

template<class Row, class Column>
void 
TableKey<Row, Column>::SetKey(Row rowIndex, Column columnIndex)
{
	this.rowIndex = rowIndex;
	this.columnIndex = columnIndex;
}

template<class Row, class Column>
pair<Row, Column> 
TableKey<Row, Column>::GetKey()
{
	return pair<string, string>(this.Nterm, this.term);
}


template<class TableKey>
void 
Table<TableKey>::TableInsert(TableKey key, Rule rule)
{
	(this->table).insert( pair<TableKey,Rule>(key, rule) );

}

template<class TableKey>
vector<Rule> 
Table<TableKey>::TableFind(TableKey key)
{
	vector<Rule> retval;
	/* multimap class has an inbuilt 'class' iterator ;
	   so, multimap<TableKey, Rule>::iterator is a class type.
	*/

       
	multimap<TableKey, Rule>::iterator it;
	pair< multimap<TableKey, Rule>::iterator, multimap<TableKey, Rule>::iterator > iterBoundPair; 
      
	/* Returns the bounds of a range that includes all the elements in the container with a key that compares equal to x. */
	iterBoundPair = (this->table).equal_range(key);

	for ( it = iterBoundPair.first; it != iterBoundPair.second; it++ )
	{
		retval.push_back((*it).second);
	}

	return retval;
}

//###### EXPLICIT INSTANTIATIONS ##########
template class TableKey < string, string >;
template class Table < TableKey<string, string> >;
