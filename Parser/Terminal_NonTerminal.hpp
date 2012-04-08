#include <commons.hpp>

#define T	Terminals
#define NT	NonTerminals

void addTerminal(std::string);
void removeTerminal(std::string);
void removeTerminal(int);
int findTerminal(std::string);
std::string getTerminal(int);

void addNonTerminal(std::string);
void removeNonTerminal(std::string);
void removeNonTerminal(int);
int findNonTerminal(std::string);
std::string getNonTerminal(int);

bool isTerminal(std::string);
bool isNonTerminal(std::string);

void outputTerminals();
void outputNonTerminals();
