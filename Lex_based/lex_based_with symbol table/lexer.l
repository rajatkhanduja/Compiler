%{

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "token.h"
#include "hash.h"

int yyline = 1;
int yycolumn = 0;
char lit_buffer[180];
int len = 0;

void reportError(char *str);

/* YYSTYPE(union) is either a pointer to a 
 * token or a number 
 */

YYSTYPE yylval;

%}

wildchar [\@\#\$\%\^\?\~]
letter  [A-Za-z]
digit   [0-9]
ts      [\t ]
ident   {letter}({letter}|{digit})*
badident ({letter}|{digit})*({wildchar})+({letter}|{digit})*
intconst ({digit})+
badint ({digit})+(({letter})|(\\)|(\'))+({digit})*

%x Comments
%x ProcessString

%%

"&&"                    {  yycolumn += yyleng; return ANDnum; }
":="                    {  yycolumn += yyleng; return ASSGNnum; }
"."                     {  yycolumn += yyleng; return DOTnum; }
"="                     {  yycolumn += yyleng; return EQUALnum; }
">"                     {  yycolumn += yyleng; return GTnum; }
{ident}                 { 
                                /* check to see if name is already in symbol table */
                                yylval.tk = search(yytext);

                                /* if it is not in the symbol table, search(yytext), 
                                will return a NULL pointer(i.e. 0 ),
                                we then know it is an identifier */

                                if(yylval.tk == 0) {
                                        yylval.tk = insertID(yytext);
                                        yylval.tk->len = strlen(yytext);
                                        yycolumn += yyleng;  //
                                        return IDnum;
                                }
                                /* otherwise, it is already in the symbol table and it  
                                   could be an identifier or a keyword, so 
                                   just return it's type. yylval.tk will point to the 
                                   symbol table entry where the name is located   */
                                else {
                                        yycolumn += yyleng;
                                        return yylval.tk->type;
                                }
                        }
{badident}              {
                           reportError("Bad Identifier");
                        }       
"["                     {  yycolumn += yyleng; return LBRACnum; }
"("                     {  yycolumn += yyleng; return LPARENnum; }
"!="                    {  yycolumn += yyleng; return NEnum; }
"||"                    {  yycolumn += yyleng; return ORnum; }
"]"                     {  yycolumn += yyleng; return RBRACnum; }
")"                     {  yycolumn += yyleng; return RPARENnum; }
";"                     {  yycolumn += yyleng; return SEMInum; }
","                     {  yycolumn += yyleng; return COMMAnum; }
"/"                     {  yycolumn += yyleng; return DIVIDEnum; }
"=="                    {  yycolumn += yyleng; return EQnum; }
">="                    {  yycolumn += yyleng; return GEnum; }
{intconst}              { 
                                yylval.number = atoi(yytext);
                                yycolumn += yyleng;
                                return ICONSTnum; 
                        }
{badint}                {       reportError("Invalid Integer Constant");
                        }       
                                
"{"                     {  yycolumn += yyleng; return LBRACEnum; }
"<="                    {  yycolumn += yyleng; return LEnum; }
"<"                     {  yycolumn += yyleng; return LTnum; }
"-"                     {  yycolumn += yyleng; return MINUSnum; }
"!"                     {  yycolumn += yyleng; return NOTnum; }
"+"                     {  yycolumn += yyleng; return PLUSnum; }
"}"                     {  yycolumn += yyleng;  return RBRACEnum; }
\'                      {
                                BEGIN ProcessString;
                                len = 0;
                                yymore();
                        }

<ProcessString>([^\n'\\])*   {

                                yymore(); 
                        }

<ProcessString>\\/[^tn\'\\]   {  
                                yymore(); 
                        }

<ProcessString>\\/[t]   {  
                                yymore(); 

                        }
<ProcessString>\\/[n]   {  
                                
                                yymore(); 
                        }

<ProcessString>(\\)(\\) {       
                                yymore(); 
                        }

<ProcessString>\\'      {  
                                yymore(); 
                        }

<ProcessString>\'       {
                                /* check to see if string literal is already in symbol table */

                                int i = 0; 
                                int j = 0;
                                int len;
                                int found;
                                len = strlen(yytext);

                                while (i < len) {
                                        found = 0;

                                    if (yytext[i] == '\\') {

                                          if ( yytext[i+1] == 'n' ) {
                                              lit_buffer[j] = '\n';
                                              found = 1;
                                          }
                                          else if ( yytext[i+1] == 't') {
                                              lit_buffer[j] = '\t';
                                              found = 1;
                                          }
                                          else if (yytext[i+1] == '\\') {
                                               lit_buffer[j] = '\\';
                                              found = 1;
                                          }
                                          else if(yytext[i+1] == '\'') {
                                                lit_buffer[j]='\'';
                                                found = 1;
                                          }
                                          else
                                                lit_buffer[j] = yytext[i];
                                     }
                                     else
                                        lit_buffer[j] = yytext[i]; 

                                        if(found == 1)
                                                i+=2;
                                        else
                                                i+=1;
                                        j++;
                                 }

                                 lit_buffer[j] = '\0';

                                 yylval.tk = search(lit_buffer);

                                 /* if it is not, then load it into the symbol table */
                                 if(yylval.tk == 0) {
                                        yylval.tk = insertString(lit_buffer); 
                                        yylval.tk->len = strlen(yytext);
                                 }

                                /* otherwise, it is already in the symbol table*/ 
                                BEGIN 0;

                                yycolumn += yyleng;

                                return SCONSTnum;
                        }

\'([^\'\n])*$           { reportError("Unterminated String Literal"); }
"*"                     { return TIMESnum; }

"/*"                    { BEGIN Comments; }
<Comments>\/\*          {       }

<Comments>\*\/          { BEGIN 0; }

"*/"                    { reportError("no start of comment"); }

[ \t]+                  { yycolumn += yyleng; }
\n                      { yycolumn=0; yyline++; }
.                       { reportError(yytext); }
%%

void reportError(char *str)
{
        printf("%s Error at line %d, Column %d \n", str, yyline, yycolumn);
}
