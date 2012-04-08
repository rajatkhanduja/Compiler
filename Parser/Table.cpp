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



/* Here we will fill the table according to the algorithm. */
template<class TableKey>
void
Table<TableKey>::PopulateTable(Grammar CFG, FirstSet firstSet, FollowSet followSet)
{
	vector<Rule> grammarRules = CFG.GrammarAllRules();
	vector<Rule>::iterator itr;
	vector<string>::iterator its1, its2;
	vector<string> tail, setFirst, setFollow;
	string head;
	TableKey key;
	
	for ( itr = grammarRules.begin(); itr < grammarRules.end(); itr++ )
	{
		tail = (*itr).tail;	//#TODO tail has to be a public member.
		head = (*itr).head;	//#TODO head has to be a public member.

		// Now the whole tail has to be considered as a SINGLE AGGREGATE symbol.
		setFirst = FirstOfAggSym(firstSet, tail, tail.begin());
		for ( its1 = setFirst.begin(); its1 != setFirst.end(); its1++ )
		{
			if ( *its1 == "#" )		
			{
				//######## Application of Rule(2) ###########.
				setFollow = (followSet.GetFollowSet())[head];
				for ( its2 = setFollow.begin(); its2 != setFollow.end(); its2++ )
				{
					if ( *its2 == "$" )
					{
						key.SetKey(head, "$");
						TableInsert(key, *itr);
					}
					
					else if ( CFG.isTerm[*its2] )	// '$' is not a TERMINAL.
					{
						key.SetKey(head, *its2);
						TableInsert(key, *itr);
					}
				}

			}
			else if ( CFG.isTerm[*its1] )	// Note:: 'epsilon' is not a terminal.
			{
				//####### Application of Rule(1) ###########.

				key.SetKey(head, *its1);//#TODO The type of 'head' and '*its' should depend on the template parameter TableKey unlike strings here.
				TableInsert(key, *itr);
			}	

		}	

	}
	// We need to consider each rule in the form :: A -> [alpha], where [alpha] is an AGGREGATE symbol.
}


//###### EXPLICIT INSTANTIATIONS ##########
template class TableKey < string, string >;
template class Table < TableKey<string, string> >;
