SRC = .
OBJ = .
BIN = .
CC = g++
DEFINES =
CFLAGS = -I./

OBJECTS = ${OBJ}/Rule.opp ${OBJ}/Grammar.opp ${OBJ}/Terminal_NonTerminal.opp ${OBJ}/xtoLL1.opp ${OBJ}/xtoLL1driver.opp# ${OBJ}/ParserFunctions.opp ${OBJ}/Table.opp ${OBJ}/NonRecursivePredictiveParser.opp


all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/xtoLL1driver.outpp

${OBJ}/%.opp : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

grammar: $(OBJ)/Rule.opp $(OBJ)/Terminal_NonTerminal.opp $(OBJ)/Grammar.opp $(OBJ)/xtoLL1.opp
	

clean:
	rm -f ${OBJ}/*.opp
	rm -f ${BIN}/*.outpp
