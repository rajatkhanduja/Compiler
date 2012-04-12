LL1 = ../Parser
SRC = .
OBJ = obj
OBJ_GRAMMAR = $(LL1)
LEX = ../Lexical_Analyser/analyzer
BIN = bin
CC = g++
DEFINES =
CFLAGS = -Wall -I./ -I$(LL1)/ -I$(LEX)/include/
LEX_OBJECTS = $(LEX)/obj/FSM.o  $(LEX)/obj/LexicalAnalyser.o $(LEX)/obj/RegexParser.o $(LEX)/obj/SymbolTable.o
GRAMMAR_OBJECTS = $(OBJ_GRAMMAR)/Terminal_NonTerminal.opp $(OBJ_GRAMMAR)/Rule.opp $(OBJ_GRAMMAR)/Grammar.opp $(OBJ_GRAMMAR)/xtoLL1.opp
OBJECTS = $(OBJ)/LR0Automaton.o $(OBJ)/SLRParser.o

all: $(LEX_OBJECTS) $(GRAMMAR_OBJECTS) $(OBJECTS) $(SRC)/main.cpp
	${CC} $^ -g -o ${BIN}/slr-parser.out

# This is a general rule
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

$(OBJ_GRAMMAR)/%.opp : 
	make -C $(LL1) grammar

$(LEX)/obj/%.o:
	make -C $(LEX) lex_lib

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out

tags:
	ctags -R *
