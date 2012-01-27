#ifndef TOKEN_H
#define TOKEN_H

/* yylval */
typedef union
{
        int number;
        struct node *tk;
} YYSTYPE;

/* constants corresponding to tokens */
#define ANDnum                  1
#define ASSGNnum                2
#define DECLARATIONnum          3
#define DOTnum                  4
#define ENDDECLARATIONSnum      5
#define EQUALnum                6
#define GTnum                   7
#define IDnum                   8
#define INTnum                  9
#define LBRACnum                10
#define LPARENnum               11
#define METHODnum               12
#define NEnum                   13
#define ORnum                   14
#define PROGRAMnum              15
#define RBRACnum                16
#define RPARENnum               17
#define SEMInum                 18
#define VALnum                  19
#define WHILEnum                20
#define CLASSnum                21
#define COMMAnum                22
#define DIVIDEnum               23
#define ELSEnum                 24
#define EQnum                   25
#define GEnum                   26
#define ICONSTnum               27
#define IFnum                   28
#define LBRACEnum               29
#define LEnum                   30
#define LTnum                   31
#define MINUSnum                32
#define NOTnum                  33
#define PLUSnum                 34
#define RBRACEnum               35
#define RETURNnum               36
#define SCONSTnum               37
#define TIMESnum                38
#define VOIDnum                 39
#define EOFnum                  0

#endif