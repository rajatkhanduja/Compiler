import copy
import pprint

srcFile = file("../test/source.c")
outFile = file("../test/source.c.lex.output","w")

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

def NFA_simulate(nfa_all) :
	token = ''
	i = 0
	for nextLine in srcFile:
		nextLine = nextLine.splitlines()[0]
		words = nextLine.split(' ')
		while '' in words:
			words.remove('')
		print "Words on line " + str(i) + " : " + str(words)
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

					# Writing to file uotput for the parser.
					outFile.write("< " + str(nfa.token) + ", " + str(i) + " > ") 

				if matched:
					break
			if not matched :

				# Writing to file uotput for the parser.
				outFile.write("@@ Error : " + str(word) + " @@ ") 

				print "Error on line " + str(i)
				print "No token for lexeme : " + str(word)

		# Writing to file uotput for the parser.		
		outFile.write("\n")

	# Printing Symbol Table
	print
	print "``````````````````````````````````````````````````````````````````````````````````````````````````"
	print "                                       SYMBOL TABLE                                               "
	print "``````````````````````````````````````````````````````````````````````````````````````````````````"

	for lexeme in symbolTable.iterkeys() :
		#print "*******************************************************************************************"
		print "Lexeme\t:\t" + str(lexeme)
		print "Token\t:\t" + str(symbolTable[lexeme][0].token)
		for entry in symbolTable[lexeme] :
			# symbolTable[lexeme] is an list of objects
			print "Line\t:\t" + str(entry.line)
		print "*******************************************************************************************"
	#pp = pprint.PrettyPrinter(indent=4)
	#pp.pprint(symbolTable)
	 

				
					


		
