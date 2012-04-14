#ifndef PARSER_FUNCTIONS_HPP
#define PARSER_FUNCTIONS_HPP

#include <xtoLL1.hpp>
#include <Grammar.hpp>
#include <commons.hpp>
#include <map>
#include <Terminal_NonTerminal.hpp>

class FirstSet
{
	private:
		map<string, list<string> > firstSet;
	public:
		FirstSet();
		list<string> First(const string Gsym/* Any Grammar Symbol */, Grammar& grammar);
		void FirstSetAddEntry(string key, list<string> value);
		map<string, list<string> > GetFirstSet();
		void RemoveDuplicatesFromFirst();	

};


list<string> FirstOfAggSym(FirstSet firstSet, vector<string> tail, vector<string>::iterator itStart);

class FollowSet
{
	private:
		map< string, list<string> > followSet;
		map< string, bool> dirtyFlags;
		// list and vector provide almost the same functionality despite their merits and de-merits. Lists are implemented as doubly-linked
		// lists whereas vectors are implemented as dynamic arrays. 
		/*
			Lists are useful for :
			1) Insertion and Deletion anywhere ( constant Time )
			2) Moving block of elements within the same or between different containers. ( constant time )
			3) Iterating over the items ( linear time )

			Vectors are useful for :
			1) Random indexing ( constant Time )
			2) Insertion and Deletion AT THE END ( Amortized constant time )
			3) Iteration over the items ( linear time )

		*/
		map<string, list<string> > NTDependencyList;
					
	public:
		FollowSet();
		list<string> Follow(FirstSet& firstSet, const string Gsym /* Any Grammar Symbol */, Grammar& grammar);
		void FollowSetAddEntry(string key, list<string> value);
		map< string, list<string> > GetFollowSet();

		void UpdateDependencyList(vector<string>& recursionStack, string Gsym);	
		void UndirtySymbol(string Gsym);
		void ProcessDependencyList();

		void RemoveDuplicatesFromFollow();
};

#endif
