%{

#include <stdio.h>
#include <string.h>
  typedef struct node
  {
    struct node *left;
    struct node *right;
    int tokcode;
    char *token;
  } node;
  node *mknode(node *left, node *right, int tokcode, char *token);
  void printtree(node *tree);
  void generate(node *tree);

#define YYSTYPE struct node *

%}

%start lines

%token	NUMBER
%token	PLUS	MINUS	TIMES	DIVIDE	POWER
%token	LEFT_PARENTHESIS	RIGHT_PARENTHESIS
%token	END

%left	PLUS	MINUS
%left	TIMES	DIVIDE
%right	POWER

%%

lines:  /* empty */
        | lines line /* do nothing */

line:   exp END { 
                  printtree($1); 
		  printf("\n\n");
		  generate($1);
		  printf("\n");
                }
	;

exp    : term             {$$ = $1;}
        | exp PLUS term     {$$ = mknode($1, $3, PLUS, "+");}
        | exp MINUS term    {$$ = mknode($1, $3, MINUS, "-");}
        ;

term   : factor           {$$ = $1;}
        | term TIMES factor  {$$ = mknode($1, $3, TIMES, "*");}
        ;

factor : NUMBER           {$$ = mknode(0,0, NUMBER, (char *)yylval);}
        | LEFT_PARENTHESIS exp RIGHT_PARENTHESIS {$$ = $2;}
        ;
%%

int main (void) {return yyparse ( );}

node *mknode(node *left, node *right, int tokcode, char *token)
{
  /* malloc the node */
  node *newnode = (node *)malloc(sizeof(node));
  char *newstr = (char *)malloc(strlen(token)+1);
  strcpy(newstr, token);
  newnode->left = left;
  newnode->right = right;
  newnode->token = newstr;
  newnode->tokcode = tokcode;
  return(newnode);
}

void printtree(node *tree)
{
  int i;

  if (tree->left || tree->right)
    printf("(");

  printf(" %s ", tree->token);

  if (tree->left)
    printtree(tree->left);
  if (tree->right)
    printtree(tree->right);

  if (tree->left || tree->right)
    printf(")");
}

void generate(node *tree)
{
  int i;

  /* generate the code for the left side */
  if (tree->left)
    generate(tree->left);
  /* generate the code for the right side */
  if (tree->right)
    generate(tree->right);

  /* generate code for this node */
  
  switch(tree->tokcode)
  {
  case 0:
    /* we need no code for this node */
    break;

  case NUMBER:
    /* push the number onto the stack */
    printf("PUSH %s\n", tree->token);
    break;

  case PLUS:
    printf("POP A\n");
    printf("POP B\n");
    printf("ADD A= A+B\n");
    printf("PUSH A\n");
    break;

  case MINUS:
    printf("POP A\n");
    printf("POP B\n");
    printf("SUB A= A-B\n");
    printf("PUSH A\n");
    break;

  case TIMES:
    printf("POP A\n");
    printf("POP B\n");
    printf("MULT A= A*B\n");
    printf("PUSH A\n");
    break;

  default:
    printf("error unkown AST code %d\n", tree->tokcode);
  }

}

int yyerror (char *s) {fprintf (stderr, "%s\n", s);}
