#include <xtoLL1.hpp>

using namespace std;


LL1_TableKey::LL1_TableKey(string Nterm, string Term)
{
	this.Nterm = Nterm;
	this.Term = Term;
}

LL1_TableKey::SetKey(string Nterm, string Term)
{
	this.Nterm = Nterm;
	this.Term = Term;
}

pair<string, string> LL1_TableKey::GetKey()
{
	return pair<string, string>(this.Nterm, this.term);
}

void LL1_Table::LL1_TableInsert(LL1_TableKey key, Rule rule)
{
	this.ParsingTable.insert( pair<LL1_TableKey,Rule>(key, rule) );

}


vector<Rule> LL1_Table::LL1_TableFind(LL1_TableKey key)
{
	vector<Rule> retval;
	/* multimap class has an inbuilt 'class' iterator ;
	   so, multimap<LL1_TableKey, Rule>::iterator is a class type.
	*/

        // #TODO Review Templates
	PTable_t::iterator it;
	pair<PTable_t::iterator, PTable_t::iterator> ret; 
        // #TODO Review Templates
	
	ret = this.ParsingTable.equal_range(key);

	for ( it = ret.first; ret != ret.second; it++ )
		{
			retval.push_back((*it).second) ;
			

		}

	return retval;
}
