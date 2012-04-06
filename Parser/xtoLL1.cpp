#include <xtoLL1.hpp>
#include <fstream>
#include <sstream>
#include <Terminal_NonTerminal.hpp>
using namespace std;

void ScanGrammarFromFile(Grammar& g, char* filename)
{
	ifstream f(filename);
	string s, t;
	bool ishead = true;
	vector<std::string> tail;
	do
	{
		getline(f, s, '\n');
		
		if(!s.compare("") || s[0] == '#')
			continue;
		else
		{
			ishead = true;
			Rule *r;
			stringstream ss(s);
			while(getline(ss, t, ' '))
			{
				if(!t.compare("-->"))
				{
					ishead = false;
					continue;
				}
				else if(!t.compare("|"))
				{
					r->RuleAddTail(tail);
					tail.clear();
				}
				else if(ishead)
					r = new Rule(t);
				else
					tail.push_back(t);
			}
			r->RuleAddTail(tail);
			tail.clear();
			g.GrammarAddRule(*r);
			//r.~Rule();
		}
	}while(f.good());
	f.close();
}
