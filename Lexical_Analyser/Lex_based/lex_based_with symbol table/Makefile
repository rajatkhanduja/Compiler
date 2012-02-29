TEST            = scanner
CC              = gcc
MAKE            = make
RM              = rm
LEX             = lx
MSLIB_FILE      = \
                lex.yy.c \
                hash.c \
                scanner.c
LIBS            = -ll
CFLAGS          = -O -DDEBUG
OBJ = $(MSLIB_FILE:c=o)
TARGET = $(TEST)

all :   $(TARGET)
lexer.yy.o : lex.yy.c
	gcc -c lex.yy.c
lex.yy.c : lexer.l
	flex lexer.l
$(TARGET) : lexer.yy.o $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)
.c.o :
	$(CC) $(CFLAGS) -c $(<)
clean :
	$(RM) -f $(TEST) *.o *.rpo core lex.yy.c
again:
	$(MAKE) clean
	$(MAKE) $(TEST)
