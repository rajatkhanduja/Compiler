#include <ParserFunctions.hpp>

/* The extern keyword means "declare without defining". In other words ,it is a way to explicitly declare a variable, 
   or to force a declaration without a definition. It is also possible to explicitly define a variable, i.e. to force 
   a definition. It is done by assigning an initialization value to a variable. If neither the extern keyword nor an 
   initialization value are present, the statement can be either a declaration or a definition.

   An external variable may also be declared inside a function. In this case the extern keyword must be used, otherwise 
   the compiler will consider it a definition of a local variable, which has a different scope, lifetime and initial value. 
   This declaration will only be visible inside the function instead of throughout the function's module. 
*/

FirstSet::FirstSet ()
{
	//Empty
}

FollowSet::FollowSet ()
{
	//Empty
}

void FirstSet::FirstSetAddEntry(string key, list<string> value)
{

	if ( (this->firstSet).find(key) == (this->firstSet).end() )
	{	
		(this->firstSet)[key] = value;
	}
	else
	{
		//std::cerr << "Key " << key << " already exists in firstSet with value " << (this->firstSet)[key] << std::endl;
		std::cerr << "Error in adding entry to firstSet. Key already exists.\n";
	}

}


map<string, list<string> > FirstSet::GetFirstSet()
{
	return (this->firstSet);
}

void FirstSet::RemoveDuplicatesFromFirst()
{
	map<string, list<string> >::iterator itm;
	for ( itm = (this->firstSet).begin(); itm != (this->firstSet).end(); itm++ )
	{
		(itm->second).sort();
		(itm->second).unique();
	}
}

list<string> FirstSet::First (string Gsym, Grammar& CFG)
{
 	list<string> retval;
	bool continueOnEpsilon = false;
	int index;

	if ( Gsym == EPSILON )
	{
		std::cerr << "FIRST[epsilon] does not exist" << std::endl;
		return retval;
	}
	// Gsym is a terminal.
	if ( isTerminal(Gsym) ) //#TODO isTerminal() should be a part of the Grammar class. ;)
		{
			// Application of Rule(1).
			retval.push_back(Gsym);
		}

	//Gsym is a non-teminal.
	/*
		"" Iterator for a map "" 
			is a pointer to the pair(key, value). To access them, we have it->first, it->second. 
			map< string, bool> isTerm,	map<string, vector<string>> firstSet
		"" Iterator for a vector ""
			is a pointer to value that is stored at the position where the iterator is pointing at present. To access it, we have (*it)
			vector<string> matchedRules
	*/
	else
		{
			// We need to search for all productions that start with the non-terminal GSym.
			//vector<Rule>::iterator itr;
			vector< vector<std::string> >::iterator itvs;
			vector<string>::iterator its1;
			vector<string> tail; 

			list<string>::iterator its2;
			list<string> firstVals;
			
			index = CFG.GrammarFindRule(Gsym);				
			
			/* A rule has a structure:
			 *	head: <string>
			 *	tail(s): <vector of strings>
			 */

			Rule matchedRule = CFG.GrammarRule(index);  // The Rule whose head starts with Gsym.
			vector< vector<std::string> > matchedTails = matchedRule.RuleTails();		// matchedTails is a vector of vector of strings.			

			for ( itvs = matchedTails.begin(); itvs < matchedTails.end(); itvs++ )
				{
					// Given a rule, We need to traverse all the symbols in it.
					//*(itvs) is the tail of the matched rule.
					
					//tail = (*itr).tail;	// #TODO This requires tail to be public member.
					tail = *itvs;					

					// ########################## TRAVERSING A MATCHED RULE #############################################

					// ################ APPLICATION OF RULE(3) ###############################
					if ( *(tail.begin()) == EPSILON )	// i.e we have a production of the form :: A -> epsilon
					{
						retval.push_back(EPSILON);
					}
					
					// ################ APPLICATION OF RULE(2) ################################
					else
					{	
						// Now we will traverse the tail of the current production chosen to build the first set.
						for ( its1 = tail.begin(); its1 < tail.end(); its1++ )
							{
								continueOnEpsilon = false;	
								// Hoping the leftmost symbol in the reduction is a terminal or even if it's a non-terminal, 
								// it's firstSet does not contain an epsilon. Hence nextHasEpsilon is set to 0 in this hope.

								if ( *its1 == EPSILON )	
								{
									std::cerr << "Unexpected epsilon symbol in the tail of a grammar rule" << std::endl;
									break;	//Break from this rule, proceed to next grammar rule.
								}
								
								
								// The condition below states that the leftmost symbol of this reduction is a terminal.
								if ( isTerminal(*its1) )		// Terminating condition for Recursion.
								{	
									retval.push_back(*its1);
								}
								else	// It's a non-terminal.
								{	
								/* This recursive call would also find the FIRST sets of a lot of other grammar symbols as well. 
								 * So before calling First(Gsym) we need to check whether there alredy exists an entry 
								 * corresponding to Gsym in firstSet map.
								 */
									if ( (this->firstSet).find(*its1) == (this->firstSet).end() )
									{	
										/* It's first set has not been constructed yet and thus a recusive call is 
										   required to construct it. */
                                	                                        
										// ###  RECURSIVE CALL ###
						                		    // For Rajat
		    								if ( Gsym.compare(*its1) != 0 ) 
    											First(*its1, CFG);
					                                        // ### RECURSIVE CALL ###

 										/*  Here we are sure that the recursion will terminate at some point of time; 
										 *  as the base condition is reached. It cannot happen that the recursion 
										 *  encounters a loop. For eg :
										 *  First ( A ) ==> FIRST( B ) ==> .... This sequence will never contain a 
										 *  back edge. Since if it contains a back edge from a Non-terminal K to 
										 *  Non-terminal T let's say, it implies that we have a reduction like  
										 *  ( K --> T ). But from the sequence we are following, we also have 
										 *  a reduction like  ( T --> K ) as per the recursion path. 
										 *  This implies we can have a reduction T --> T or K --> K in the grammar 
										 *  which implies it is left recursive. Hence contradiction.
										 */
									}	
										
										/* We now have the firstSet for the non-terminal we encountered in the 
										 * production for Gsym. */
										
										firstVals = (this->firstSet)[*its1];
										// firstvals is a vector of strings.
										for ( its2 = firstVals.begin(); its2 != firstVals.end(); its2++ )
											{
												if ( *its2 == EPSILON ) 
												{	// We have an epsilon production, Hence we need 
													// to continue in the loop.
													continueOnEpsilon = true;

													// The following is equivalent to the condition that we
													// have a rule :: A -> epsilon. We found an epsilon in 
													// the first set of the last symbol of this grammar rule
													// and the fact that we have reached the last grammar 
													// symbol in this calculation implies that the first set
													// of all the previous symbols in this rule contained an
													// epsilon symbol and hence the first set of this symbol
													// will contain an epsilon.
													if ( (its1 + 1) == tail.end() )
													{
														retval.push_back(EPSILON);
													}
												}	
												else
												{ 
													retval.push_back(*its2); 
												}

											}
								 }
								if ( not (continueOnEpsilon) )	
								{	
									break;	// No epsilon productions found, break from traversing THIS RULE further.	
								}	
							}

						// ########################## TRAVERSING A MATCHED RULE ############################################# 
					}
				}
		}

	FirstSetAddEntry(Gsym, retval);			// Filling the class data structure firstSet through Recursive Calls.
	return retval;
}	


/* FirstOfAggSym() is a non-class method. */
list<string> FirstOfAggSym(FirstSet firstSet, vector<string> tail, vector<string>::iterator itStart)
{
	vector<string>::iterator it;

	list<string>::iterator epsilonPosition;
	list<string> tmpFirstSet;
	list<string> retval;
	map<string, list<string> > firstSetMap = firstSet.GetFirstSet();

	for ( it = itStart; it < tail.end(); it++ )
	{
		tmpFirstSet = firstSetMap[*it];	// Find the firstSet of this non-terminal.
		retval.insert(retval.end(), tmpFirstSet.begin(), tmpFirstSet.end());
			
		if ( (epsilonPosition = find(retval.begin(), retval.end(), EPSILON)) != retval.end() )			
		{	
			if ( (it + 1) != tail.end() )	// i.e this is not the LAST symbol of the aggregate symbol.
				retval.erase(epsilonPosition);
		}	
		else // No epsilon symbol found. We can stop here 
		{	
			break;
		}	
		
	}
	return retval;
}

void FollowSet::FollowSetAddEntry(string key, list<string> value)
{

	if ( (this->followSet).find(key) == (this->followSet).end() )
	{	
		(this->followSet)[key] = value;
	}
	else
	{
		std::cerr << "Key \"" << key << "\" already exists in firstSet" << std::endl;
	}

}


map<string, list<string> > FollowSet::GetFollowSet()
{
	return (this->followSet);
}

void FollowSet::RemoveDuplicatesFromFollow()
{
	map<string, list<string> >::iterator itm;
	for ( itm = (this->followSet).begin(); itm != (this->followSet).end(); itm++ )
	{
		(itm->second).sort();
		(itm->second).unique();
	}
}

//###############################################  FOR HANDLING RIGHT RECURSION #####################################################

// This is constructed for a given 'recursion path'.
// NOTE :: If the FollowSet of a symbol is already calculated, we will not call 'UpdateDependencyList' for it.
/* [3] cases can arise for a node in the recursive Follow tree
   1) 'recursion path' before the node ( CANNOT OCCUR since that path would break from the recursion as soon as it finds the FIRST 'recursion path'.
   2) 'recursion path' below the node, does not lead to any dependencies in the node, since dependencies do not INHERIT from child nodes to 
       parent node in the recursive FOLLOW tree.
   3) 'recursion path' passing through the node is the only case which leads to dependencies in the node itself.
*/	

void FollowSet::UpdateDependencyList(vector<string> &recursionStack, string Gsym)
{
	vector<string>::iterator it;
	map< string, list<string> >::iterator itm;
	list<string> tmpList;

	for ( it = recursionStack.end() - 1; it >= recursionStack.begin(); it++ )
	{
		if ( (*it).compare(Gsym) == 0 )
	{
			break;
		}
		else
		{
			// The 'NTDependencyList' will be constructed for each node only when it is  encountered first in the 'recursion tree'.
			if ( ( itm = (this->NTDependencyList).find(*it) ) == (this->NTDependencyList).end() )
			{	
				tmpList.push_back(Gsym);
				(this->NTDependencyList).insert( pair<string, list<string> >(*it, tmpList) );	// NTDependencyList is a map
				tmpList.clear();
			}
			else
			{
				((*itm).second).push_back(*it);		
			}

			
			if ( (this->dirtyFlags).find(*it) == (this->dirtyFlags).end() )					// dirtyFlags is a map
			{
				(this->dirtyFlags).insert( pair<string, bool>(*it, true) );
			}
		}
	
	}
}

void FollowSet::UndirtySymbol(string Gsym)
{
	list<string>::iterator itls;
	for ( itls = (NTDependencyList[Gsym]).begin(); itls != (NTDependencyList[Gsym]).end(); itls++ )
	{
		if ( dirtyFlags[*itls] )
		{
			UndirtySymbol(*itls);
		}
		else
		{
			// Add the contents of FOLLOW(*itvs) to FOLLOW(Gsym)

			/*  Modifying the 'followSet' bypassing the FollowSetAddEntry() function  */
			(followSet[Gsym]).insert( (followSet[Gsym]).end(), (followSet[*itls]).begin(), (followSet[*itls]).end() );
			/**************************************************************************/

			(followSet[Gsym]).sort();
			(followSet[Gsym]).unique();

		}
	}

	dirtyFlags[Gsym] = false;	// IMPORTANT

}


void FollowSet::ProcessDependencyList()
{
	// We'll process the dependency list of all the Non-terminals now.
	
	map<string, list<string> >::iterator itm;
	for ( itm = NTDependencyList.begin(); itm != NTDependencyList.end(); itm++ )
	{
		UndirtySymbol((*itm).first);
	}
	
	// Clear all the 'NTDependencyList' data with the destruction of the 'recursion (FOLLOW) tree'
	(this->NTDependencyList).clear();

	// Clear all the 'dirtyFlags' data with the destruction of the 'recursion (FOLLOW) tree'
	(this->dirtyFlags).clear();
}

//###############################################  FOR HANDLING RIGHT RECURSION #####################################################

/* NOTE :: Follow() function access the data structure 'firstSet' ONLY through the non-class function firstOfAggSym() */

/* The 'ProcessDependencyList()' needs to be called everytime a function call is made to 'Follow' in the client program. And not anywhere else
   since the root node in the 'recursion FOLLOW tree' is the primary source of dependency in the tree and before any updation is done; all the
   nodes dependent on it must be updated so that further percolation of the updation process is done correctly.
*/

list<string> FollowSet::Follow (FirstSet& firstSet, string Gsym, Grammar& CFG)
{
	map<string,list<string> >::iterator itrRet = followSet.find (Gsym);
	if ( itrRet != followSet.end())
	{
  	return itrRet->second;
	}

	list<string> retval;
	if ( Gsym == CFG.GrammarStartSymbol() )
	{
		retval.push_back(ENDMARKER);		// We place the "$" symbol in follow of the start symbol.
	}

	vector<Rule> GRules = CFG.GrammarAllRules();
	vector< vector<std::string> >::iterator itvs;
	vector< vector<std::string> > tailsWithCommonHead;	
	vector<Rule>::iterator itr; 
	vector<string>::iterator its; 	// Declaration
	vector<string> tail;
	string head;
	
	list<string> firstOfNextSym;
	list<string> followVals; 
	
	static vector<string> recursionStack;	// To track what's on the recursion stack.

	// Push to the static variable 'recursionStack' .
	recursionStack.push_back(Gsym);

	bool needTorecurse = false;

	for ( itr = GRules.begin(); itr < GRules.end(); itr++ )
	{
		/* A rule has a structure:
			 *	head: <string>
			 *	tail(s): <vector of strings>
		*/
		tailsWithCommonHead = (*itr).RuleTails();
		head = (*itr).RuleHead();	
	
		for ( itvs = tailsWithCommonHead.begin(); itvs < tailsWithCommonHead.end(); itvs++ )
		{	
			tail = *itvs;
			
			for ( its = tail.begin(); its < tail.end(); its++ )
			{
				if ( (*its) == Gsym )	// Symbol(Gsym) found in the tail of the Grammar Rule.
				{
					//#################################### RECURSION DOMAIN ######################################################
					
					/* Consider the rule A -> [a] [C] [Gsym] [B] [g] [D]
					 * For the application of Rule(2) or Rule(3), all the symbols before 'Gsym' would need to be
					 * considered as one symbol and the same is true for all the symbols after 'Gsym'. So, let's re-write the rule as follows
					 * A -> [alpha] [Gsym] [beta]
					 * Now, examine the contents of FIRST([beta]). But before that, we need to consider an important issue ::
					 * note that the 'AGGREGATE SYMBOL' [beta] composed of [B], [g] and [D] does not have an EXPLICIT entry for itself
					 * in the 'firstSet' data structure, since the 'firstSet' data structure contains entries only for the grammar symbols
					 * and not such 'AGGREGATE SYMBOLS'. Hence we need to be careful here.
					 * */
					
					/* We need to create an artificial rule for this 'AGGREGATE SYMBOL' and calculate it's first set with the existing
					 * information in the 'firstSet' data structure.
					 * 
					 * << ARTIFICIAL RULE >>
					 * [beta] -> [B] [g] [D]
					 *
					 * We use firstOfAggSym() function for this purpose.
					 * */
					
					needTorecurse = false;

					if ( (its + 1) == tail.end() ) /* i.e 'Gsym' is the last symbol in the tail. */ 
					{
						// Application of Rule (3). Since we have a rule of the form :: A -> [alpha] [Gsym]
						// Hence everything in Follow(A) is in Follow(Gsym).
						needTorecurse = true;
						   
					}
					else
					{	
											
						firstOfNextSym = FirstOfAggSym(firstSet, tail, its + 1);	// Find the firstSet of this non-terminal.

						if ( find(firstOfNextSym.begin(), firstOfNextSym.end(), EPSILON) != firstOfNextSym.end() ) 
						{
							// Application of Rule(3). Since we have :: A -> [alpha] [Gsym] [beta] where FIRST([beta]) contains an epsilon.
							// Hence everything in Follow(A) is in Follow(Gsym). 						
							needTorecurse = true;
						}
						else
						{
							// Application of Rule(2). Everything in FIRST([beta]) is in FOLLOW([Gsym])
							retval.insert(retval.end(), firstOfNextSym.begin(), firstOfNextSym.end());
							// We will not break here since we may find the same symbol 'Gsym' many times in the tail.
							// We'll just finish this iteration here and scan the remaining tail portion.

						}
						
					}
					// Recurse if needed
					if ( needTorecurse )
					{
						if ( (this->followSet).find(head) == (this->followSet).end() )
							{
								// The below keeps a check for RIGHT RECURSIVE GRAMMARS .
								// The recursion will never END in case of RIGHT RECURSIVE GRAMMARS. Hence the following 'if' condition
								// makes sure that the recursion will terminate at some point.
								if ( find(recursionStack.begin(), recursionStack.end(), head) == recursionStack.end() )
								{	
									//###### RECURSIVE CALL ##########
									followVals = Follow(firstSet, head, CFG);
									//###### RECURSIVE CALL ##########
								}
								else
								{	
									// If it does not have an entry in the followSet and also exists in the recursion stack implies
									// it is in the process of calculation and has occurred again which is a case of 
									// RIGHT RECURSIVE GRAMMAR. Here the CALCULATION of the followSet of a grammar symbol in the 
									// form of recursion has back-edged to ITSELF. Hence, we END the recursion here. 

									// Report the occurence of a RIGHT RECURSIVE GRAMMAR
									/*
										We have encountered a RECURSION PATH in the recursive method for the 
									        calculation of the FOLLOW. Now we take account of all such recursive 
										paths the emerge during calculation of the FOLLOW of a non-terminal which
										progresses in a tree-like fashion. All the nodes that lie on a 'recursion 
										path' need to be UPDATED later when the node in whose recursion they are										      involved has completely dealt with for it's FOLLOW calculation. So all the
										nodes involved in the 'recurision path' are set with their dirty bit = 1
										and their dependency list pushed with the concerned recursive symbol.
										These dependency lists are processed at the very end i.e after we wind
										up with the orginal function call to 'Follow()' again recursively, SINCE
										BY THEN ALL THE SYMBOLS IN THE DEPENDENCY LIST OF ANY SYMBOL INVOLVED
										IN THIS FUNCTION CALL WOULD HAVE BEEN COMPLETELY CALCULATED.
										depending whether the dependency list itself contains dirty symbols.
									*/

									/* Break recursion here since this path would provide no useful contribution
									   anymore. But we need to update the dependency list for the nodes on the
									   'recursion path' which would be processed at the very end.
									*/
									// Push the non-terminal responsible for the recursion in the dependency list of
									// all in the 'affectedNonTerminals'
									/*
										It's like a 'BURST' of updation that travels starting from the nodes with 
										no dirty bit set that UPDATES all the nodes dependent on it.
										Once such node i.e the root node always exists which can initiate the very
										first burst. A node with the dirty bit unset can SAFELY emit it's burst
										of updation to other nodes.
									*/
										
									// update the dependency list and break off.
									UpdateDependencyList(recursionStack, Gsym);
	
									std::cerr << ":: Found a RIGHT RECURSIVE path :: Don't worry it'll work\n";
									followVals.clear();	// Clear the followVals vector. 
								}	
							}
							
						// 'followSet' now contains the follow set for head. 
						// We just need to append that to follow of Gsym.
						else
							{
								followVals = ((this->followSet).find(head))->second;
							}
						retval.insert(retval.end(), followVals.begin(), followVals.end());
					}
					   	
					//#################################### RECURSION DOMAIN ######################################################
				}
			} 
		}
	
	}
	
	FollowSetAddEntry(Gsym, retval);	// Filling the class data structure followSet through Recursive Calls.
	
	// Pop the static variable 'recursionStack'.
	recursionStack.pop_back();
	return retval;
}

