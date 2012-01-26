SRC = src
OBJ = obj
BIN = bin
CC = gcc
CFLAGS = -Iinclude/ 
OBJECTS = 

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/lexical_analyzer

rbrxg:	${OBJ}/hashtable_functions.rxg.o ${OBJ}/symboltable_functions.rb.o

# This is a general rule
${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*

