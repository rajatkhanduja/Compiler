SRC = .
OBJ = .
BIN = .
CC = g++
DEFINES =
CFLAGS = -I./
OBJECTS = ${OBJ}/Rule.opp ${OBJ}/xtoLL1.opp

all: clean ${OBJECTS}
	${CC} $^ -g -o ${BIN}/xtoLL1.outpp

${OBJ}/%.opp : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.opp
	rm -f ${BIN}/*.outpp
