SRC = src
OBJ = obj
BIN = bin
CC = gcc
CFLAGS = -Iinclude/ 
OBJECTS = ${OBJ}/regex_parser.o ${OBJ}/main.o

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/lexical_analyzer

# This is a general rule
${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*
