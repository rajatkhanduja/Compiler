#include <commons.hpp>

/* TODO IMPORTANT
 * make Terminals and NonTerminals a part
 * of class Grammar as they would be specific
 * to a certain grammar only.
 */

#define T	Terminals
#define NT	NonTerminals

// the argument string is \
added to the vector if it \
is not already present.
void addTerminal(std::string);
// the argument string is \
searched for in the vector \
and removed if found.
void removeTerminal(std::string);
// the argument index entry \
is removed without mercy if it \
exists!
void removeTerminal(int);
// the argument string is \
looked for and its index \
is returned.
int findTerminal(std::string);
// the argument index entry's \
value is returned; no questions \
asked!
std::string getTerminal(int);

// the argument string is \
added to the vector if it \
is not already present.
void addNonTerminal(std::string);
// the argument string is \
searched for in the vector \
and removed if found.
void removeNonTerminal(std::string);
// the argument index entry \
is removed without mercy if it \
exists!
void removeNonTerminal(int);
// the argument string is \
looked for and its index \
is returned.
int findNonTerminal(std::string);
// the argument index entry's \
value is returned; no questions \
asked!
std::string getNonTerminal(int);


// the argument string is \
checked for presence in the \
vector and corresponding \
truth value is returned.
bool isTerminal(std::string);
// the argument string is \
checked for presence in the \
vector and corresponding \
truth value is returned.
bool isNonTerminal(std::string);

// outputs the whole vector.
void outputTerminals();
// outputs the whole vector.
void outputNonTerminals();
