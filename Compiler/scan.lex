%{
	/* scan.l    
	
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"
#include "y.tab.h"
#define YY_NO_INPUT
%}
%option noyywrap



int_const    [0-9]+
blank 	     [ \t]
b            {blank}+
digit        [0-9]
digits       {digit}+
alpha        [A-Za-z_]
alnum        [A-Za-z0-9_]
id           {alpha}{alnum}*
str_const    (\"[^\"^\n]*\")|(\'[^\'^\n]\')


%%

{b}          {}
{int_const}  {yylval.yyint = (int)atoi(yytext); return CONST;}
{id}         {yylval.yyint = *yytext; return VAR;}
.|\n         {return yytext[0];}



%%





