// vim:ts=8:noexpandtab
/* This file defines the functions/methods of the LR0Automaton class. */

#include <LR0Automaton.h>
#include <Terminal_NonTerminal.hpp>
#include <xtoLL1.hpp>
#include <vector>
#include <iostream>
#include <sstream>

using std::stringstream;
using std::vector;

static Rule * startRule;

// Declare the augmentedStartSymbol to be complex so that it 
// is highly unlikely that it is used in the grammar.
const string LR0Automaton :: augmentedStartSymbol = "StArT5yMb0l";	

// This function can be called after the grammar has been initialized
void LR0Automaton::initialize ()
{
	// Add the rule S' -> S to create the augmented grammar
	Rule startSymRule (augmentedStartSymbol);
	startSymRule.RuleAddTail (vector<string> (1, 
					slrGrammar.GrammarStartSymbol()));
	slrGrammar.GrammarAddRule (startSymRule);
	slrGrammar.GrammarSetStartSymbol (augmentedStartSymbol);

	startRule = &startSymRule;

	// Construct Canonical Collection
	constructCanonicalCollection ();
}


LR0Automaton::LR0Automaton (char * grammarFileName)
{
	ScanGrammarFromFile (slrGrammar, grammarFileName);
	initialize ();
}

LR0Automaton::LR0Automaton (const Grammar& grammar)
{
	slrGrammar = grammar;
	initialize ();
}

string LR0Automaton::postDotSymbol (const Item& item)
{
	return (item.second.second.size() > 0) ? item.second.second[0] : string();
}
/* Function to convert a rule (a particular tail) to an Item
 * This function returns an item such that the 'dot' is 
 * right in the beginning.
 */
Item rule2Item (Rule& rule, unsigned int ruleIndex)
{
	return (make_pair (rule.RuleHead(), 
			make_pair (vector <string> (),
				rule.RuleTail(ruleIndex))));
}
/* Function to convert a rule to an ItemSet using rule2Item
 * This function needs to be passed an ItemSet for results
 * A pointer to the same variable (result) is returned.
 */
ItemSet* rule2ItemSet(Rule& rule,ItemSet& result)
{
	unsigned int i, n;
	for ( i = 0, n = rule.RuleNTails(); i < n; i++)
	{
		result.insert (rule2Item (rule, i));	
	}
	return &result;
}

/* Function to take closure given an ItemSet 
 * This function needs to be passed an ItemSet for results
 * A pointer to the same variable (result) is returned.
 */
ItemSet* ItemSetsClosure (const ItemSet& items, Grammar &slrGrammar,
				ItemSet& result)
{
	result.insert (items.begin(), items.end());
	ItemSet::iterator itr, itr_end;

	bool newElemAdded = true;	
	Rule rule;
	int ruleIndex;

	while (newElemAdded)
	{
		newElemAdded = false;
		
		for (itr = result.begin(), itr_end = result.end();
			itr != itr_end; itr++)
		{
			if ( isNonTerminal ( LR0Automaton::postDotSymbol(*itr)))
			{
				ruleIndex = slrGrammar.GrammarFindRule(
						(itr->second).second[0]);
				if ( ruleIndex >= 0)
				{
					rule = slrGrammar.GrammarRule (
								ruleIndex);
					ItemSet fromRules;
					rule2ItemSet (rule, fromRules);
					
					ItemSet::iterator itr1, itr1_end;

					for ( 	itr1 = fromRules.begin (),
						itr1_end = fromRules.end ();
						itr1 != itr1_end; itr1++)
					{
						if (result.find (*itr1) == 
							result.end ())
						{
							result.insert (*itr1);
							newElemAdded = true;
						}
					}
				}
			}
		}
	}

	return &result;
}

/* Function to return the set of symbols that will have 
 * a corresponding value in GOTO. Basically, all the symbols that follow
 * the 'dot', i.e. the first string in the second vector in Item.
 */
set<string> requiredSymbols (const ItemSet& itemSet)
{
	ItemSet::iterator itr, itr_end;
	set<string> symbols;

	for (itr = itemSet.begin(), itr_end = itemSet.end();
		itr != itr_end; itr++)
	{
		if (itr->second.second.size() > 0)
			symbols.insert (itr->second.second[0]);
	}
	
	return symbols;
}

/* Function that takes as input a pointer and returns if another element
 * in the set has the same value. Since the elements in the set are pointers 
 * themselves, we need to compare with each
 */
template <class Iterator, class Type> Iterator inContainer (Iterator start, 
							Iterator end, 
							Type* ptr)
{
	for (; start != end; start++)
	{
		if ( *ptr == **start )
		{
			break;
		}
	}

	return start;
}


void LR0Automaton::constructCanonicalCollection ()
{
	// Start with inserting the set of item containing only S'->S
	ItemSet *tmpSet = new ItemSet();
	rule2ItemSet (*startRule, *tmpSet);
	bool newElemAdded = true;

	set<ItemSet*>::iterator itr, itr_end;
	/* Verify if this remains constant 
	ItemSet::iterator itr_end = canonicalCollection.end();
	*/

	canonicalCollection.insert (
		(ItemSetsClosure (*tmpSet, slrGrammar, startSet)));
	states.push_back (&startSet);

	while (newElemAdded)
	{
		newElemAdded = false;
		for (itr = canonicalCollection.begin(), 
		     itr_end = canonicalCollection.end();
		     itr != itr_end; itr++)
		{
			/* For each symbol, check GOTO */
			set<string> symGoTo = requiredSymbols (**itr);
			set<string>::iterator strSetItr, strSetItrEnd;
			for ( 	strSetItr = symGoTo.begin (),
				strSetItrEnd = symGoTo.end(); 
				strSetItr != strSetItrEnd; strSetItr++)
			{
				ItemSet * gotoSet = goTo (*itr, *strSetItr);
	//			printItemSet (*gotoSet);

				/* If gotoSet is not empty and is not in
				 * in canonicalCollection. */
				if ( !(gotoSet->empty()) &&
					(canonicalCollection.end() ==
						inContainer(
						canonicalCollection.begin(),
						canonicalCollection.end(), 
						gotoSet)))
				{
					canonicalCollection.insert (gotoSet);
					states.push_back (gotoSet);
					newElemAdded = true;
				}
			}
		}
	}
}


static Item shiftDot (const Item& item)
{
	Item result(item);
	
	// Shift the 'dot' virtually by erasing the first element
	// from the second part of the item and inserting into the first part
	result.second.first.push_back (result.second.second[0]);
	result.second.second.erase (result.second.second.begin());

	return result;
}

ItemSet* LR0Automaton::goTo (ItemSet* I, const string& X)
{
//	std::cerr << "Symbol " << X << std::endl;
	/* First check GOTO */
	map<ItemTerminalPair, ItemSet*>::iterator itr;
	ItemTerminalPair setTerminalPair = make_pair (I, X);
	if ( (itr = GOTO.find (setTerminalPair)) != GOTO.end ())
	{
		return itr->second;	
	}
	
	ItemSet tmpSet, *result = new ItemSet();
	assert (result->empty());
	Item tmpItem;

	ItemSet::iterator itemItr;
	for ( itemItr = I->begin(); itemItr != I->end(); itemItr++)
	{
//		std::cerr << "Post dot symbol : " << postDotSymbol (*itemItr)
//			  << "\n";
		if ( ! X.compare (postDotSymbol (*itemItr)))
		{
//			std::cerr << "Inside if\n";
			// Store the changed item in tmpItem
			tmpItem = shiftDot (*itemItr);

			// Insert tmpItem to tmpSet
			tmpSet.insert (tmpItem);
		}
	}
	
	ItemSetsClosure (tmpSet, slrGrammar, *result);

	// Add the result to GOTO.
	set<ItemSet*>::iterator tmp = inContainer(canonicalCollection.begin(),
					canonicalCollection.end(), result);
	if (tmp == canonicalCollection.end())
	{
		// Use the new set.
		GOTO[setTerminalPair] = result;
	}
	else
	{
		// Refer to the previous set
		GOTO[setTerminalPair] = *tmp;
	}
	return result;
}	

ItemSet * LR0Automaton::startItemSet ()
{
	return &(this->startSet);
}

#define vectorIterate(v,itr) \
for (itr = v.begin(); itr != v.end(); itr++) \
{\
	output << " " << *itr << " ";\
}

string item2String (const Item& item)
{
	stringstream output;
	output << item.first << " --> ";
	vector<string>::const_iterator itr;

	vectorIterate (item.second.first, itr);
	output << ".";
	vectorIterate (item.second.second, itr);
	
	return output.str();
}

string itemSet2String (const ItemSet& items)
{
	ItemSet::const_iterator itr, itr_end;
	stringstream output;

	for (itr = items.begin(), itr_end = items.end(); itr != itr_end; itr++)
	{
		output << item2String(*itr) << "\n";
	}
	return output.str();
}

string LR0Automaton::canonicalCollection2String (void)
{
	set<ItemSet*>::iterator itr, itr_end;
	stringstream s;
	int i;
	for (i = 0, itr = canonicalCollection.begin(),
		itr_end = canonicalCollection.end(); itr != itr_end; itr++, i++)
	{
		s << "ItemSet " << i << ":" << std::endl;
		s << itemSet2String (**itr);
		s << "\n";
	}
	
	return s.str();
}
