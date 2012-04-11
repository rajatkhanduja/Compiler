#include <Rule.hpp>
using namespace std;

Rule::Rule()
{
	head = "";
	vector<std::string> tail;
	tail.push_back("");
	tails.push_back(tail);
}

Rule::Rule(const string h)
{
	head = h;
}

std::string Rule::RuleHead()
{
	return this->head;
}

vector<std::string> Rule::RuleTail(int i)
{
	return this->tails[i];
}

vector<vector<std::string> > Rule::RuleTails()
{
	return this->tails;
}

void Rule::RuleAddTail(vector<std::string> t)
{
	if(this->RuleFindTail(t) < 0)
		this->tails.push_back(t);
}

void Rule::RuleRemoveTail(int i)
{
	if(i < this->RuleNTails())
		this->tails.erase(this->tails.begin() + i);
}

void Rule::RuleRemoveTail(vector<std::string> v)
{
	int i;
	if((i = this->RuleFindTail(v)) > 0)
		this->RuleRemoveTail(i);
}

int Rule::RuleFindTail(vector<std::string> t)
{
	for(int i = 0; i < this->RuleNTails(); i++)
		if(equals(this->tails[i], t))
			return i;
	return -1;
}

int Rule::RuleNTails()
{
	return this->tails.size();
}

void Rule::RuleOutput()
{
	if(this->head == "")
	{
		cout<<"<empty or invalid rule>"<<endl;
		return;
	}
	cout<<"Rule:\t("<<this->head<<") --> ";
	for(int i = 0; i < this->RuleNTails(); i++)
	{
		for(int j = 0; j < this->tails[i].size(); j++)
			cout<<"("<<tails[i][j]<<")";
		if(i == this->RuleNTails() - 1)
			cout<<"\n";
		else
			cout<<" | ";
	}
}

int Rule::RuleFindEpsilonProduction()
{
	for(int i = 0; i < this->RuleNTails(); i++)
		if(this->tails[i].size() == 1 && !(this->tails[i][0].compare(EPSILON)))
			return i;
	return -1;
}

bool Rule::equals(vector<std::string> a, vector<std::string> b)
{
	int l;
	if((l = a.size()) != b.size())
		return false;
	else
		for(int i = 0; i < l; i++)
			if(a[i] != b[i])
				return false;
	return true;
}

bool Rule::operator==(Rule rhs)
{
	int l;
	if(this->head == rhs.head && (l = this->RuleNTails()) == rhs.RuleNTails())
	{
		for(int i = 0; i < l; i++)
			if(!equals(this->tails[i], rhs.tails[i]))
				return false;
		return true;
	}
	return false;
}
