# % ==> concatenation
# | ==> or
# * ==> Kleen closure
# Priority *(Kleene closure) > %(concatenation) > |(or) > ( (left parenthesis)


def infix2postfix(regex):
	postfix = []  # Used to output the postfix notation
	operatorStack = []
	indexO = 0
	topO = '0'  # top of the operatorStack
	i = 0
	length = len(regex)
	regex = '(' + str(regex) + ')'
	
	while i < length :
		if regex[i] == '(' :
			operatorStack.append(regex[i]) # Push the left parenthesis on to the stack
			i = i + 1
			while ( regex[i] != ')' ) :
				if regex[i] == '|' : # Since it has the lowest priority.
					if operatorStack != [] :
						indexO = len(operatorStack) - 1
						topO = operatorStack[indexO] # Equivalent of popping off the stack.
						while ( topO != '|' and topO != '(' ) :	# A higher prioirty operator exists on the stack. So we need to apply it.
							
							top0 = operatorStack.pop(index0)
							postfix.append(topO)
						
							if operatorStack != [] :
								indexO = len(operatorStack) - 1
								topO = operatorStack[indexO]
							else :
								break
					
					operatorStack.append(regex[i])	# Found a '|'or '('  on top. 
					i = i + 1
				
				elif regex[i] == '*' : # This is the highest priority operator. Hence it can be pushed on the stack directly.
					operatorStack.append(regex[i])
					i = i + 1

				elif regex[i] == '%' : # Pop the operatorStack until an operator of lower ( or equal priority ) is found.
					
					if ( operatorStack != [] ) :
						indexO = len(operatorStack) - 1 
						topO = operatorStack[indexO] # Equivalent of popping off the stack.
						while ( topO != '%' and topO != '|' and topO != '(' ) :
							
							topO = operatorStack.pop(indexO)
							postfix.append(topO)

							if operatorStack != [] :
								index0 = len(operatorStack) - 1
								topO = operatorStack[indexO]
							else :
								break

					operatorStack.append(regex[i])	
					i = i + 1

				else :  # This can be an alphabet or a '('
						operatorStack.append(regex[i])
					else : # any alphabet
						postfix.append(regex[i])
					i = i + 1
					
			# Now, we just saw the ) right parenthesis. Pop all the operators in order until you find the left parenthesis.
			indexO = len(operatorStack) - 1
			topO = operatorStack.pop(indexO)
			
			while ( topO != '(' ) :
				postfix.append(topO)
				
				indexO = len(operatorStack) - 1
				topO = operatorStack.pop(indexO)
			
			operatorStack.pop(indexO) # Pop out the left parenthesis.
				

		elif regex[i] == '|': # Since | holds the lowest priority it will always be pushed on the stack.
			if operatorStack != [] :
				indexO = len(operatorStack) - 1
				topO = operatorStack[indexO] # Equivalent of popping off the stack.
				while ( topO != '|' and topO != '(' ) :	# A higher prioirty operator exists on the stack. So we need to apply it.
					
					top0 = operatorStack.pop(index0)
					postfix.append(topO)
				
					if operatorStack != [] :
						indexO = len(operatorStack) - 1
						topO = operatorStack[indexO]
					else :
						break
					
			operatorStack.append(regex[i])	# Found a '|'or '('  on top. 
			i = i + 1

			
		elif regex[i] == '*':
			operatorStack.append(regex[i])
			i = i + 1

			
		elif regex[i] == '%':
			if ( operatorStack != [] ) :
				indexO = len(operatorStack) - 1 
				topO = operatorStack[indexO] # Equivalent of popping off the stack.
				while ( topO != '%' and topO != '|' and topO != '(' ) :
					
					topO = operatorStack.pop(indexO)
					postfix.append(topO)

					if operatorStack != [] :
						indexO = len(operatorStack) - 1
						topO = operatorStack[indexO]
					else :
						break

			operatorStack.append(regex[i])	
			i = i + 1

			
		else : # This can be an alphabet or a '('
			if regex[i] == '(' :
				operatorStack.append(regex[i])
			else : # any alphabet
				postfix.append(regex[i])
			i = i + 1

	j = 0
	while j <= (len(operatorStack) - 1) :
		postfix.append(operatorStack[j])
		j = j + 1

	return postfix



if __name__ == '__main__' :
	regex = "(a|b)%a%b*"
	postfix = infix2postfix(regex)
	print postfix

