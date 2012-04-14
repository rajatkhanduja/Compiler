#include <Table.hpp>

using namespace std;


template<class Row, class Column>
TableKey<Row, Column>::TableKey()
{
	// Empty
}

template<class Row, class Column>
TableKey<Row, Column>::TableKey(Row rowIndex, Column columnIndex)
{
	this->rowIndex = rowIndex;
	this->columnIndex = columnIndex;
}

template<class Row, class Column>
void 
TableKey<Row, Column>::SetKey(Row rowIndex, Column columnIndex)
{
	this->rowIndex = rowIndex;
	this->columnIndex = columnIndex;
}

template<class Row, class Column>
pair<Row, Column> 
TableKey<Row, Column>::GetKey()
{
	return pair<string, string>(this->rowIndex, this->columnIndex);
}

// Note this is required because the 'map' data structure requires an operator '<' for COMPARISON of it's elements and since the 'TableInsert' function inserts
// a template type object into the template type table. A generic/specific COMPARISON function is required before any INSERTION can take place.
template<class Row, class Column>
Column
TableKey<Row, Column>::GetColumn() const
{
	return this->columnIndex;
}


template<class Row, class Column> 
bool 
TableKey<Row, Column>::operator<(const TableKey<Row, Column>& key ) const
{
	return ( this->columnIndex > key.GetColumn() );
}


template<class Key>
Table<Key>::Table()
{
	// Empty
}


template<class Key>
void 
Table<Key>::TableInsert(Key key, Rule rule)
{
	pair <Key, Rule> insertPair;
	insertPair = make_pair(key, rule);
	(this->table).insert(insertPair);

}

template<class Key>
vector<Rule> 
Table<Key>::TableFind(Key key)
{
	vector<Rule> retval;
	/* multimap class has an inbuilt 'class' iterator ;
	   so, multimap<Key, Rule>::iterator is a class type.
	*/

	typedef typename multimap<Key, Rule>::iterator TemplateIterator;	// Template iterator	
	TemplateIterator it;
	pair<TemplateIterator, TemplateIterator> iterBoundPair; 
      
	/* Returns the bounds of a range that includes all the elements in the container with a key that compares equal to x. */
	iterBoundPair = (this->table).equal_range(key);

	for ( it = iterBoundPair.first; it != iterBoundPair.second; it++ )
	{
		retval.push_back((*it).second);
	}

	return retval;
}



/* Here we will fill the table according to the algorithm. */
//#TODO Although this is a template function, but it works only when template parameter 'Key' has the form TableKey <string, string> due to the way SetKey() is called
template<class Key>
void
Table<Key>::PopulateTable(Grammar& CFG, FirstSet& firstSet, FollowSet& followSet)
{
	vector<Rule> grammarRules = CFG.GrammarAllRules();
	vector<Rule>::iterator itr;
	vector<string> tail;

	list<string>::iterator its1, its2;
	list<string> setFirst, setFollow;
	
	string head;
	Key key;
	vector < vector<std::string> > tailsWithCommonHead;
	vector < vector<std::string> >::iterator itvs;
	Rule rule;
	
	for ( itr = grammarRules.begin(); itr < grammarRules.end(); itr++ )
	{
		tailsWithCommonHead = (*itr).RuleTails();

		head = (*itr).RuleHead();
		rule.RuleReinitialize(head);

		for ( itvs = tailsWithCommonHead.begin(); itvs < tailsWithCommonHead.end(); itvs++ )
		{
			tail = *itvs;
			rule.RuleAddTail(tail);
			// Now the whole tail has to be considered as a SINGLE AGGREGATE symbol.
			setFirst = FirstOfAggSym(firstSet, tail, tail.begin());
			for ( its1 = setFirst.begin(); its1 != setFirst.end(); its1++ )
			{
				if ( *its1 == EPSILON )		
				{
					//######## Application of Rule(2) ###########.
					setFollow = (followSet.GetFollowSet())[head];
					for ( its2 = setFollow.begin(); its2 != setFollow.end(); its2++ )
					{
						if ( *its2 == ENDMARKER )
						{
							key.SetKey(head, ENDMARKER);
							TableInsert(key, rule);
						}
						
						else if ( isTerminal(*its2) )	// '$' is not a TERMINAL.
						{
							key.SetKey(head, *its2);
							TableInsert(key, rule);
						}
					}

				}
				else if ( isTerminal(*its1) )	// Note:: 'epsilon' is not a terminal.
				{
					//####### Application of Rule(1) ###########.

					key.SetKey(head, *its1);//#TODO The type of 'head' and '*its' should depend on the template parameter Key unlike strings here.
					TableInsert(key, rule);
				}	

			}
			rule.RuleRemoveTail(tail);	
		} 
	}
	// We need to consider each rule in the form :: A -> [alpha], where [alpha] is an AGGREGATE symbol.
}


//###### EXPLICIT INSTANTIATIONS ##########
template class TableKey < string, string >;
template class Table < TableKey<string, string> >;
