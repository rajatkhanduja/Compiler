import copy
import pprint

srcFile = file("../../test/test3.c")

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
		if nfa.move.has_key(state) and nfa.move[state].has_key('#') :
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
	for nextLine in srcFile:
		nextLine = nextLine.splitlines()[0]
		words = nextLine.split(' ')
		print "Words on line :" + str(i) + " " + str(words)
		i = i + 1
		for word in words :
			symObject = symbolTableEntry()
			matched = False
			for nfa in nfa_all :
				S = epsilon_closure(set([nfa.s0]), nfa)	# Set of current states
				for c in word :
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
					
					print "---------------------------------------------------------------------------------"
					print "MATCHED  :: " + str(word) + " with token < " + str(nfa.token) + ", " + str(i) + " >" + " TOKEN " + str(symObject.token) + " LINE " + str(symObject.line)

					print "---------------------------------------------------------------------------------"
				if matched:
					break
			if not matched :
				print "Error on line " + str(i)
				print "No token for lexeme : " + str(word)
	# Printing Symbol Table
	print "``````````````````````````````````````````````````````````````````````````````````````````````````"
	print "				SYMBOL TABLE 								"
	print "``````````````````````````````````````````````````````````````````````````````````````````````````"

	for lexeme in symbolTable.iterkeys() :
		print "*******************************************************************************************"
		print
		print "Lexeme	: " + str(lexeme)
		print "Token	: " + str(symbolTable[lexeme][0].token)
		for entry in symbolTable[lexeme] :
			# symbolTable[lexeme] is an list of objects
			print "Line	: " + str(entry.line)
		print
		print "*******************************************************************************************"
	#pp = pprint.PrettyPrinter(indent=4)
	#pp.pprint(symbolTable)
	 

				
					


		
