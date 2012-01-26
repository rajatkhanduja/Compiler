SRC = src
OBJ = obj
BIN = bin
CC = gcc
CFLAGS = -Iinclude/ 
OBJECTS = ${OBJ}/fsm.o ${OBJ}/set.o ${OBJ}/regex_parser.o ${OBJ}/regex_to_fsm.o ${OBJ}/main.o

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/lexical_analyzer.out

rbrxg:	${OBJ}/hashtable_functions.rb.o ${OBJ}/symboltable_functions.rb.o

# This is a general rule
${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*.out
