flex TAC.l
yacc -d TAC.y
gcc y.tab.c lex.yy.c -ly -ll
