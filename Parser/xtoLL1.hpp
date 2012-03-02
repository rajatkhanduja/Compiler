#include <commons.hpp>
#include <Grammar.hpp>
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


/* Key Structure for the LL(1) Table => (Non-Terminal, Terminal ) */
class LL1_TableKey
{
	private:
		string NTerm;	// Single Non-Teminal
		string Term;	// Single Terminal 
	public:
		LL1_TableKey(string NTerm,string Term);
		SetKey(string Nterm,string Term);
		pair<string, string> GetKey();
};


/* LL(1) Table implemented as a multimap (Associative Container) */

class LL1_Table
{
	private:
		multimap<LL1_TableKey, Rule> ParsingTable;

	public:
		LL1_Table();
		void LL1_TableInsert(LL1_TableKey key, Rule rule);
		vector<Rule> LL1_TableFind(LL1_TableKey);
};


typedef multimap<LL1TableKey, Rule> PTable_t;
