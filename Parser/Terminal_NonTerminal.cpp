#include <Terminal_NonTerminal.hpp>

static vector<std::string> Terminal;
static vector<std::string> NonTerminal;

static int find(vector<std::string> X, std::string x)
{
	for(int i = 0; i < X.size(); i++)
		if(X[i] == x)
			return i;
	return -1;
}

static void add(vector<std::string> X, std::string x)
{
	if(find(X, x) < 0)
		X.push_back(x);
}

static void remove(vector<std::string> X, int i)
{
	if(i < X.size())
		X.erase(X.begin() + i);
}

static void remove(vector<std::string> X, std::string x)
{
	int i;
	if((i = find(X, x)) >= 0)
		remove(X, i);
}

static bool is(vector<std::string> X, std::string x)
{
	if(find(X, x) > -1)
		return true;
	return false;
}

static void output(vector<std::string> X)
{
	for(int i = 0; i < X.size(); i++)
		cout<<X[i]<<" ";
	cout<<endl;
}

void addTerminal(std::string t)
{
	add(T, t);
}

void removeTerminal(std::string t)
{
	remove(T, t);
}

void removeTerminal(int i)
{
	remove(T, i);
}

int findTerminal(std::string t)
{
	return find(T, t);
}

void addNonTerminal(std::string nt)
{
	add(NT, nt);
}

void removeNonTerminal(std::string nt)
{
	remove(NT, nt);
}

void removeNonTerminal(int i)
{
	remove(NT, i);
}

int findNonTerminal(std::string nt)
{
	return find(NT, nt);
}

bool isTerminal(std::string t)
{
	return is(T, t);
}

bool isNonTerminal(std::string nt)
{
	return is(NT, nt);
}

void outputTerminals()
{
	cout<<"Terminals:"<<endl;
	output(T);
}

void outputNonTerminals()
{
	cout<<"Non-Terminals:"<<endl;
	output(NT);
}
