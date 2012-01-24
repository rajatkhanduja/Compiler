SRC = src
OBJ = obj
BIN = bin
CC = gcc
CFLAGS = -Iinclude/ 
OBJECTS = 

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/lexical_analyzer

<<<<<<< HEAD
# This is a general rule
=======
>>>>>>> 9002683f73118f82898f92c8ad657c56a0fe9c0a
${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

common_clean : clean
	rm -f ${COMMON_OBJ}/*.o

clean:
	rm -f ${OBJ}/*.o
	rm -f ${BIN}/*
