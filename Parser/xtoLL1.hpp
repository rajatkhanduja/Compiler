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
// checks whether the rule passed as \
reference has non-terminating production(s) \
or not.
// TODO: case where rule has a \
non-(terminal production) and the \
non-terminal in it has a non-terminating \
production.
bool HasNonTerminatingProductions(Rule&);
// checks whether the grammar passed as \
reference has non-terminating rule(s) \
or not.
bool HasNonTerminatingRules(Grammar&);
// recursively eliminates epsilon productions \
in rules of the grammar passed as reference \
in the argument.
// TODO
void EliminateEpsilonProductions(Grammar&);
// checks whether the rule passed as reference \
in the argument has left recursion or not.
bool HasLeftRecursion(Rule&);
// eliminates immediate left recursion in \
the rule passed as reference in the argument.
vector<Rule> EliminateImmediateLeftRecursion(Rule&);
// eliminates immediate left recursion in \
the grammar passed as reference in the argument.
void EliminateImmediateLeftRecursion(Grammar&);
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
