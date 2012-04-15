#include "ParserFunctions.h"

int main(int argc,char* argv[])
{
    GrammarNonTerminals source_grammar;
    source_grammar.populateProductions();
    printBox("AFTER PRODUCTION READING");cout<<endl;
    source_grammar.print();
    source_grammar.populateFirst();
    printBox("AFTER FIRST");cout<<endl;
    source_grammar.print();
    source_grammar.populateFollow();
    printBox("AFTER FOLLOW");cout<<endl;
    source_grammar.print();
    source_grammar.generateTable();
    printBox("LL1 TABLE");cout<<endl;
    if(source_grammar.printTable())
    {
        printBox("AMBIGUITY DETECTED");cout<<endl;
    }
    printBox("COMPUTATION STACK");cout<<endl;
    source_grammar.parser();
    printBox("PROGRAM IS GRAMATICALLY CORRECT");cout<<endl;

    return 0;
}
