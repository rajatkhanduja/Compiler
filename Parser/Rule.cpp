#include <Rule.hpp>
#include <iostream>
using namespace std;

Rule::Rule()
{
	head = "";
	tail.push_back("");
};

Rule::Rule(string h, vector<string> t)
{
	head(h);
	tail = t;
};

std::string Rule::RuleHead()
{
	return this->head;
};

vector<std::string> Rule::RuleTail()
{
	return this->tail;
};

void Rule::RuleAddTail(std::string t)
{
	this->tail.push_back(t);
};

void Rule::RuleRemoveTail(int i)
{
	this->tail.erase(this->tail.begin() + i);
};

void RuleRemoveTail(std::string s)
{
	for(int i = 0; i < this->RuleNTails(); i++)
		if(this->tail[i] == s)
			this->RuleRemoveTail(i);
};
			
int Rule::RuleNTails()
{
	return this->tail.size();
};

void Rule::RuleOutput()
{
	cout<<"Rule:\t"<<this->head<<" -> "<<this->tail[0];
	int i = 1;
	while(i < this->RuleNTails())
		cout<<" | "<<tail[i++];
	cout<<std::endl;
};

bool Rule::operator==(Rule rhs)
{
	int l;
	if(this->head == rhs.head && (l = this->RuleNTails()) == rhs.RuleNTails())
	{
		for(int i = 0; i < l; i++)
			if(this->tail[i] != rhs.tail[i])
				return false;
		return true;
	}
	return false;
};
