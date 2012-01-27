import copy

srcFile = file("../../test/test1.c")

# tokenStream is { lineno : [set of tokens], lineno. : [set of tokens], .... }
tokenStream = {}


class symbolTableEntry :
	def __init__(self) :
		self.token = ''
		self.line = 0


# symbolTable = {symbolTableEntry.lexeme : [symbolTableEntry , ....] }
symbolTable = {}

def move(nfa,S,c) : 	# Returns the possible transitions of an nfa with set o current states as S and input characte as c.
	finalStates = set()
	for state in S :
		
		# nfa.move[state][c] is a set of states
		if nfa.move.has_key(state) and nfa.move[state].has_key(c) :
			finalStates.update(nfa.move[state][c])
	return finalStates	

def epsilon_closure(oldStates, nfa):
	activeStates = set ()
	newStates = set ()
	
	activeStates.update(oldStates)
	for state in oldStates :
		activeStates.update(nfa.move[state]['#'])


	while ( True ):
		
		newStates = copy.copy(activeStates.difference (oldStates))
		if not (newStates) :
			break

		oldStates = copy.copy(oldStates.union(activeStates))
		activeStates = copy.copy(oldStates.union(activeStates))

		
		for state in newStates:
			if nfa.move.has_key(state) and nfa.move[state].has_key('#') :
				activeStates.update(nfa.move[state]['#'])
	
	return activeStates				

'''	activeStates = set()
	newStates = set()
	print type(states)
	
	for state in states :
		activeStates.update (nfa.move[state]['#'])
			
	if not (states.difference(activeStates)):	# oldStates is not equal to newStates . i.e some new states have been added to oldStates
		for state in activeStates:
			newStates.update (nfa.move[state]['#'])
		
		 = epsilon_closure (newStates,nfa)
	else :
		return newStates
'''
def NFA_simulate(nfa_all) :
	token = ''
	i = 0
	symObject = symbolTableEntry()
	for nextLine in srcFile:
		nextLine = nextLine.splitlines()[0]
		words = nextLine.split(' ')
		i = i + 1
		for word in words :
			print word
			matched = False
			for nfa in nfa_all :
				S = epsilon_closure(set([nfa.s0]), nfa)	# Set of current states
				for c in word :
					print "At " + str(c) + " in " + str(word)
					S = epsilon_closure(move(nfa,S,c), nfa)
				if S.intersection(nfa.F):
					symObject.token = nfa.token
					symObject.line = i
					if symbolTable.has_key(word) :
						symbolTable[word].append(symObject)
					else :
						symbolTable[word] = []
						symbolTable[word].append(symObject)
					matched = True
					print "Matched " + str(word) + " with token " + str(nfa.token)
				if matched:
					break
			if not matched :
				print "Error on line " + str(i)
				print "No token for lexeme : " + str(word)
				


				
					


		
