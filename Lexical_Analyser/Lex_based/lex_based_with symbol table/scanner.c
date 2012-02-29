#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "token.h"
#include "hash.h"

int main(int argc, char **argv)
{
        int token_num;
        extern int yyline;
        extern int yycolumn;
        extern int yyleng;
        int idx = 0;
        char tem[10];

        /* either an integer or a pointer to a node */
        /* if it's a pointer to a node, then yylex returned
        /* an identifier or a keyword or a string literal
           if it's a number, then yylex returned an integer
           constant, and yylval refers to the value of the integer. */

        extern YYSTYPE yylval;
        int num =0;

#ifdef DEBUG
        struct node *tk;
        const char *str = "\'hello\n\t world!!!\'";
#endif

        /* initialize symbol table with reserved keywords */
        initialize();

        do {
                token_num = yylex();
                printf("%d\t%d\t", yyline, yycolumn);
                idx = 0;

                switch(token_num) {
                        case IDnum:
                                        printf("IDnum\n");
                                #ifdef DEBUG
                                        printf("yylval = %s \n",yylval.tk->lexeme.string);
                                #endif
                                        break;
                        case INTnum:
                                        printf("INTnum\n");
                                #ifdef DEBUG
                                        printf("yylval = %s \n",yylval.tk->lexeme.string);
                                #endif
                                        break;
                        case ICONSTnum:
                                        printf("ICONSTnum\n");          
                                #ifdef DEBUG
                                        printf("number = %d \n", yylval.number);
                                #endif
                                        break;
                        case SCONSTnum:
                                        printf("SCONSTnum\n");
                                #ifdef DEBUG
                                        printf("%s \n", yylval.tk->lexeme.string); 
                                #endif
                                        break;
                        case IFnum:
                                        printf("IFnum\n");
                                #ifdef DEBUG
                                        printf("yylval = %s \n", yylval.tk->lexeme.string);
                                #endif
                                        break;
                        case ANDnum:    printf("ANDnum\n");
                                        break;
                        case ASSGNnum:
                                        printf("ASSGNnum\n");
                                        break;
                        case DECLARATIONnum:    
                                        printf("DECLARATIONnum\n");
                                        break;
                        case DOTnum:
                                        printf("DOTnum\n");
                                        break;
                        case ENDDECLARATIONSnum:
                                        printf("ENDDECLARATIONSnum\n");
                                        break;
                        case EQUALnum:
                                        printf("EQUALnum\n");
                                        break;
                        case GTnum:
                                        printf("GTnum\n");
                                        break;
                        case LBRACnum:
                                        printf("LBRACnum\n");
                                        break;
                        case LPARENnum:
                                        printf("LPARENnum\n");
                                        break;
                        case METHODnum:
                                        printf("METHODnum\n");
                                        break;
                        case NEnum:
                                        printf("NEnum\n");
                                        break;
                        case ORnum:
                                        printf("ORnum\n");
                                        break;
                        case PROGRAMnum:
                                        printf("PROGRAMnum\n");
                                        break;
                        case RBRACnum:
                                        printf("RBRACnum\n");
                                        break;
                        case RPARENnum:
                                        printf("RPARENnum\n");
                                        break;
                        case SEMInum:
                                        printf("SEMInum\n");
                                        break;
                        case VALnum:
                                        printf("VALnum\n");
                                        break;
                        case WHILEnum:
                                        printf("WHILEnum\n");
                                        break;
                        case CLASSnum:
                                        printf("CLASSnum\n");
                                        break;
                        case COMMAnum:
                                        printf("COMMAnum\n");
                                        break;
                        case DIVIDEnum:
                                        printf("DIVIDEnum\n");
                                        break;
                        case ELSEnum:
                                        printf("ELSEnum\n");
                                        break;
                        case EQnum:
                                        printf("EQnum\n");
                                        break;
                        case GEnum:
                                        printf("GEnum\n");
                                        break;
                        case LBRACEnum:
                                        printf("LBRACEnum\n");
                                        break;
                        case LEnum:
                                        printf("LEnum\n");
                                        break;
                        case LTnum:
                                        printf("LTnum\n");
                                        break;
                        case MINUSnum:
                                        printf("MINUSnum\n");
                                        break;
                        case NOTnum:
                                        printf("NOTnum\n");
                                        break;
                        case PLUSnum:
                                        printf("PLUSnum\n");
                                        break;
                        case RBRACEnum:
                                        printf("RBRACEnum\n");
                                        break;
                        case RETURNnum:
                                        printf("RETURNnum\n");
                                        break;
                        case TIMESnum:
                                        printf("TIMESnum\n");
                                        break;
                        case VOIDnum:
                                        printf("VOIDnum\n");
                                        break;
                        case EOFnum:    
                                        printf("EOFnum\n");
                                        break;
                }
        } while(token_num != 0);

#ifdef DEBUG
        tk = search(str); 
        printf("\n%s\n", tk->lexeme.string);
#endif
        return 0;
}
 
