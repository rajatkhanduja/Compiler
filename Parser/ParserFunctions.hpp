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
		map< string, vector<string> > firstSet;
	public:
		FirstSet();
		vector<string> First(const string Gsym/* Any Grammar Symbol */, Grammar& grammar);
		void FirstSetAddEntry(string key, vector<string> value);
		map< string, vector<string> > GetFirstSet();	

};


vector<string> FirstOfAggSym(FirstSet firstSet, vector<string> tail, vector<string>::iterator itStart);

class FollowSet
{
	private:
		map< string, vector<string> > followSet;
	public:
		FollowSet();
		vector<string> Follow(FirstSet& firstSet, const string Gsym /* Any Grammar Symbol */, Grammar& grammar);
		void FollowSetAddEntry(string key, vector<string> value);
		map< string, vector<string> > GetFollowSet();	

};

#endif
