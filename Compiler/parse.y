/* Simple Expression Evaluator */

/* parse.y
 *
 * Grammer rules for bison.
 * Includes the lexical analyzer routine yylex().
 */


%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "eval.h"

#define YYDEBUG 1

void yyerror(char *s);
int  yylex();
void print_welcome();
int  get_val(int);
void set_val(int, int);


int gindex=0;
int gresult=0;
LIST   glist[1024];
static int val_tab[26];
static int val_flag[26]={UNEVALUATED};

%}


%union{
   int yyint;
   TN yytree;}
%token<yyint> CONST VAR
%type<yytree> expr term factor hash assign





%%

session
    : { print_welcome(); }
      eval
    ;

eval
    : eval line
    | /* empty */
    ;

line
    : assign '\n'		{ printf("%d", gresult); printf("\n%d:", ++gindex); }
    ;

assign
    : VAR '=' expr		{ 
	$$ = make_op_node('=', (make_var_node($1)), $3);
	gresult = eval($3);
    if (val_flag[$1-'A']==EVALUATED){
	    for(int i =0; i<1024; ++i){
        glist[i].flag = UNEVALUATED;	   
		}
    }		
	set_val($1, gresult);
	glist[gindex].value = gresult;
	glist[gindex].tree = $3; 
	}
    | expr              {
	$$ = $1;
	gresult = eval($1);
    glist[gindex].value = gresult;	
	glist[gindex].tree = $1;
	}
    ;
	
expr
    : term           
    | '+' term          { $$ = make_unop_node('+',$2);}
	| '-' term          { $$ = make_unop_node('-',$2);} 
    | expr '+' term		{ $$ = make_op_node('+', $1, $3); }
    | expr '-' term		{ $$ = make_op_node('-', $1, $3); } 
    ;


term
    : hash
    | term '*' hash		{ $$ = make_op_node('*', $1, $3); }
    | term '/' hash		{ $$ = make_op_node('/', $1, $3); }
    | term '%' hash		{ $$ = make_op_node('%', $1, $3); }
    ;

hash
    : factor 
	| '#' hash          { $$ = make_unop_node('#', $2); }
	;


factor
    : CONST             { $$ = make_int_const_node(yylval.yyint); }
	| VAR               { $$ = make_var_node(yylval.yyint); }
    | '(' expr ')'		{ $$ = $2; }
    ;


%%




void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}

void print_welcome()
{
    printf("Welcome to the Simple Expression Evaluator.\n");
    printf("Enter one expression per line, end with ^D\n\n");
	printf("%d: ", ++gindex);
}


int get_val(int v)
{
    return val_tab[v - 'A'];
}

void set_val(int v, int val)
{
    val_tab[v - 'A'] = val;
	val_flag[v -'A'] = EVALUATED;
}
