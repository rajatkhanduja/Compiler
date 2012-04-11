LL1 = ../Parser
SRC = .
OBJ = obj
OBJ_GRAMMAR = $(LL1)
BIN = bin
CC = g++
DEFINES =
CFLAGS = -Wall -I./ -I$(LL1)/
GRAMMAR_OBJECTS = $(OBJ_GRAMMAR)/Terminal_NonTerminal.opp $(OBJ_GRAMMAR)/Rule.opp $(OBJ_GRAMMAR)/Grammar.opp $(OBJ_GRAMMAR)/xtoLL1.opp
OBJECTS = $(OBJ)/LR0Automaton.o $(OBJ)/main.o

all: $(GRAMMAR_OBJECTS) $(OBJECTS)
	${CC} $^ -g -o ${BIN}/slr-parser.out

# This is a general rule
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

$(OBJ_GRAMMAR)/%.opp : 
	make -C $(LL1) grammar

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out

tags:
	ctags -R *
