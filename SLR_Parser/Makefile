LL1 = ../Parser
SRC = .
OBJ = obj
OBJ_GRAMMAR = $(LL1)
LEX = ../Lexical_Analyser/analyzer
BIN = bin
CC = g++
DEFINES =
CFLAGS = -I./ -I$(LL1)/ -I$(LEX)/include/
LEX_OBJECTS = $(LEX)/obj/FSM.o  $(LEX)/obj/LexicalAnalyser.o $(LEX)/obj/RegexParser.o $(LEX)/obj/SymbolTable.o
GRAMMAR_OBJECTS = $(OBJ_GRAMMAR)/Terminal_NonTerminal.opp $(OBJ_GRAMMAR)/Rule.opp $(OBJ_GRAMMAR)/Grammar.opp $(OBJ_GRAMMAR)/xtoLL1.opp $(OBJ_GRAMMAR)/ParserFunctions.opp
OBJECTS = $(OBJ)/LR0Automaton.o $(OBJ)/SLRParser.o $(OBJ)/main.o

all: $(LEX_OBJECTS) $(GRAMMAR_OBJECTS) $(OBJECTS)
	${CC} $^ -g -o ${BIN}/slr-parser.out

# This is a general rule
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

$(OBJ_GRAMMAR)/%.opp : 
	make -f $(LL1)/Makefile_LL1 grammar

$(LEX)/obj/%.o:
	make -C $(LEX) lex_lib

tree:parseTree.dot
	dot -Tpng $^ -o parseTree.png

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out

tags:
	ctags -R *
