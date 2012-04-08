#include <Grammar.hpp>

void ScanGrammarFromFile(Grammar&, char*);

bool HasCycles(Grammar&);
bool HasNonTerminatingRules(Grammar&);
void EliminateEpsilonProductions(Grammar&);
void EliminateLeftRecursion(Grammar&);
void LeftFactorize(Grammar&);
void EliminateDuplicateProductions(Grammar&);
