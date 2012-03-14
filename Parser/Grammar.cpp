#include <Grammar.hpp>
using namespace std;

Grammar::Grammar()
{
	Rule* r = new Rule();
}

Grammar::Grammar(vector<Rule> r)
{
	rules = r;
}

void Grammar::GrammarAddRule(Rule r)
{
	if(this->GrammarFindRule(r) < 0)
		this->rules.push_back(r);
}

void Grammar::GrammarRemoveRule(int i)
{
	if(i < GrammarNRules())
		this->rules.erase(this->rules.begin() + i);
}

void Grammar::GrammarRemoveRule(Rule r)
{
	int i;
	if((i = this->GrammarFindRule(r)) > 0)
		this->GrammarRemoveRule(i);
}

int Grammar::GrammarFindRule(Rule r)
{
	for(int i = 0; i < this->GrammarNRules(); i++)
		if(this->rules[i] == r)
			return i;
	return -1;
}

int Grammar::GrammarFindRule(std::string head)
{
	for(int i = 0; i< this->GrammarNRules(); i++)
		if(this->rules[i].RuleHead() == head)
			return i;
	return -1;
}

Rule Grammar::GrammarRule(int i)
{
	return this->rules[i];
}

int Grammar::GrammarNRules()
{
	return this->rules.size();
}

void Grammar::GrammarOutput()
{
	cout<<"Grammar contains:"<<endl;
	for(int i = 0; i < this->GrammarNRules(); i++)
		this->rules[i].RuleOutput();
}

bool Grammar::operator==(Grammar* rhs)
{
	int l;
	if((l = this->GrammarNRules()) == rhs->GrammarNRules())
	{
		for(int i = 0; i < l; i++)
			if(!(this->rules[i] == rhs->rules[i]))
				return false;
		return true;
	}
	return false;
}
