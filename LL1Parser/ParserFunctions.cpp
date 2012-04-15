#include "ParserFunctions.hpp"

int main(int argc,char* argv[])
{
    args.push_back(string(argv[1]));
    args.push_back(string(argv[2]));
    GrammarNonTerminals source_grammar;
    source_grammar.populateProductions();
    cout<<"read productions from file"<<endl;
    source_grammar.populateFirst();
    cout<<"calculated FIRST"<<endl;
    source_grammar.populateFollow();
    cout<<"calculated FOLLOW"<<endl;
    source_grammar.print();
    source_grammar.generateTable();
    cout<<"LL1 Table"<<endl;
    if(source_grammar.printTable())
        cout<<"Ambiguity Detected while constructing LL1 table."<<endl;
    cout<<"Stack of parsing"<<endl;
    source_grammar.parser();
    cout<<"Successfully parsed."<<endl;

    return 0;
}
