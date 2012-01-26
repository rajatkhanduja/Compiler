# % ==> concatenation
# | ==> or
# * ==> Kleen closure
# Priority *(Kleene closure) > %(concatenation) > |(or) > ( (left parenthesis)

# Escaping is required in all the symbols that are used in regular expressions as well as constitute the mathematical system of regular expressions.
# Ex: *, |, (, ), %, ., #.

# TODO Also concatenation operator has to be input to into the regex taken before converting it into postfix notation.
# TODO So any to operands without a * or | have to stuffed with a % between them.


def infix2postfix(regex):
	postfix = []  # Used to output the postfix notation
	operatorStack = []
	indexO = 0
	topO = '0'  # top of the operatorStack
	i = 0
	regex = '(' + str(regex) + ')'
	length = len(regex)    # EDIT
	
	while i < length :
		if regex[i] == '(' :
			operatorStack.append(regex[i]) # Push the left parenthesis on to the stack
			i = i + 1

		elif regex[i] == '|' :
			if operatorStack != [] :
				indexO = len(operatorStack) - 1
				topO = operatorStack[indexO] 
				
				while ( topO != '|' and topO != '(' ) :	# A higher prioirty operator exists on the stack. So we need to apply it.
					
					topO = operatorStack.pop(indexO)
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
						indexO = len(operatorStack) - 1
						topO = operatorStack[indexO]
					else :
						break

			operatorStack.append(regex[i])	
			i = i + 1

		elif regex[i] == ')' :
			# Now, we just saw the ) right parenthesis. Pop all the operators in order until you find the left parenthesis.
			indexO = len(operatorStack) - 1
			topO = operatorStack.pop(indexO)
			
			while ( topO != '(' ) :
				postfix.append(topO)
				
				indexO = len(operatorStack) - 1
				topO = operatorStack.pop(indexO)
			
			i = i + 1
		
		elif regex[i] == '\\' : # Escaped character. We don't allow the single \ character at present for any other symbol in regex.
			postfix.append('\\')
			postfix.append(regex[i + 1])
			i = i + 2
		
		else :  # Encountered an operand.
			postfix.append(regex[i])
			i = i + 1
	
	return postfix
