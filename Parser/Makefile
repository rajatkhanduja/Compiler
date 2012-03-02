SRC = .
OBJ = .
BIN = .
CC = g++
DEFINES =
CFLAGS = -I./
OBJECTS = ${OBJ}/Rule.opp ${OBJ}/Grammar.opp ${OBJ}/xtoLL1driver.opp

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/xtoLL1drivier.outpp

${OBJ}/%.opp : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.opp
	rm -f ${BIN}/*.outpp
