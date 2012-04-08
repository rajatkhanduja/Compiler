SRC = .
OBJ = .
BIN = .
CC = g++
DEFINES =
CFLAGS = -I./

all: ${OBJECTS}
	${CC} $^ -g -o ${BIN}/xtoLL1driver.outpp

${OBJ}/%.opp : ${SRC}/%.cpp
	${CC} -c -g ${DEFINES} ${CFLAGS} $< -o $@

clean:
	rm -f ${OBJ}/*.opp
	rm -f ${BIN}/*.outpp
