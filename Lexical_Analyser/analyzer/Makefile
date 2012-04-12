SRC = src
OBJ = obj
BIN = bin
CC = g++
DEFINES =
CFLAGS = -Wall -Iinclude/
LIB_OBJ = $(OBJ)/FSM.o $(OBJ)/RegexParser.o $(OBJ)/SymbolTable.o $(OBJ)/LexicalAnalyser.o
OBJECTS = $(LIB_OBJ) $(OBJ)/main.o

all: $(OBJECTS)
	${CC} $^ -g -o ${BIN}/lexical_analyzer.out

# This is a general rule
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

lex_lib: $(LIB_OBJ)
  

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out

tags:
	ctags -R *
