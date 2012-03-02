#include "xtoLL1.hpp"
#include "commons.hpp"


class FirstSet
{
	private:
		map<string, vector<string>> firstSet;
	public:
		FirstSet();
		vector<string> First(const string Gsym/* Any Grammar Symbol */, Grammar grammar);
		FirstSetAddEntry(string key, vector<string > value);
		

};

class FollowSet
{
	private:
		map<string, vector<string>> followSet;
	public:
		FollowSet();
		vector<string> Follow(const string Gsym /* Any Grammar Symbol */, Grammar grammar);
		FollowSetAddEntry(string key, vector<string> value);

};
