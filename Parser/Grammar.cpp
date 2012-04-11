#include <Grammar.hpp>
#include <Terminal_NonTerminal.hpp>
using namespace std;

Grammar::Grammar()
{
	startSym = "";
}

Grammar::Grammar(std::string s, vector<Rule> r)
{
	startSym = s;
	rules = r;
}
		
void Grammar::GrammarSetStartSymbol(string startSymbol)
{
	// TODO : Ensure that the string being set as startSym is a non-terminal already.
	if (!startSymbol.compare (""))
	{
		this->startSym = getNonTerminal(0);
	}
	else 
	{
		this->startSym = startSymbol;
	}
}

std::string Grammar::GrammarStartSymbol()
{
	return this->startSym;
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
	assert(i < this->GrammarNRules());
	return this->rules[i];
}

vector<Rule> Grammar::GrammarAllRules()
{
	return this->rules;
}

int Grammar::GrammarNRules()
{
	return this->rules.size();
}

bool Grammar::GrammarHasEpsilonProductions()
{
	for(int i = 0; i < this->GrammarNRules(); i++)
		if(this->rules[i].RuleFindEpsilonProduction() >= 0)
			return true;
	return false;
}

void Grammar::GrammarOutput()
{
	cout<<"Grammar contains:"<<endl;
	for(int i = 0; i < this->GrammarNRules(); i++)
	{
		cout<<"\t";
		this->rules[i].RuleOutput();
	}
	cout<<endl;
	//cout<<"blahblah"<<endl;
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
