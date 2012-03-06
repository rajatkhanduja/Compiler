SRC = src
OBJ = obj
BIN = bin
CC = g++
DEFINES =
CFLAGS = -Wall -Iinclude/
OBJECTS = $(OBJ)/fsm.o $(OBJ)/regex_parser.o $(OBJ)/symbol_table.o $(OBJ)/lexical_analyser.o $(OBJ)/main.o

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/lexical_analyzer.out

# This is a general rule
${OBJ}/%.o : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out
