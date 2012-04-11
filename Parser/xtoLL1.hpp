#include <Grammar.hpp>

// scans grammar from the file with \
filename passed as argument and populates \
the grammar reference passed as argument. \
also, populates Terminals and \
NonTerminals. also, sets startSym.
void ScanGrammarFromFile(Grammar&, char*);

// checks whether the grammar passed as \
reference in the argument has cycles or not.
bool HasCycles(Grammar&);
// checks whether the grammar passed as \
reference has non-terminating rule(s) \
or not.
bool HasNonTerminatingRules(Grammar&);
// recursively eliminates epsilon productions \
in rules of the grammar passed as reference \
in the argument.
void EliminateEpsilonProductions(Grammar&);
// eliminates left recursion in the grammar \
passed as reference in the argument.
void EliminateLeftRecursion(Grammar&);
// left factorizes all the rules in the \
grammar passed as reference in the argument.
void LeftFactorize(Grammar&);
// eliminates any duplicate productions in \
any of the rules in the grammar passed as \
reference in the argument.
void EliminateDuplicateProductions(Grammar&);
