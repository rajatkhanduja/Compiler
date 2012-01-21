SRC = src
OBJ = obj
BIN = bin
CC = gcc
CFLAGS = -Iinclude/ 
OBJECTS = 

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/lexical_analyzer

${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

common_clean : clean
	rm -f ${COMMON_OBJ}/*.o

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*
