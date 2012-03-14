#include <commons.hpp>

#define T	Terminal
#define NT	NonTerminal

void addTerminal(std::string);
void removeTerminal(std::string);
void removeTerminal(int);
int findTerminal(std::string);

void addNonTerminal(std::string);
void removeNonTerminal(std::string);
void removeNonTerminal(int);
int findNonTerminal(std::string);

bool isTerminal(std::string);
bool isNonTerminal(std::string);

void outputTerminals();
void outputNonTerminals();
