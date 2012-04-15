SRC = .
OBJ = .
OBJ_GRAMMAR = $(LL1)
LEX = ../Lexical_Analyser/analyzer
BIN = .
CC = g++
DEFINES =
CFLAGS = -I./
LEX_OBJECTS = $(LEX)/obj/FSM.o  $(LEX)/obj/LexicalAnalyser.o $(LEX)/obj/RegexParser.o $(LEX)/obj/SymbolTable.o

all: $(LEX_OBJECTS) ParserFunctions.cpp
	${CC} $^ -g -o ${BIN}/ll1-parser.out

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
