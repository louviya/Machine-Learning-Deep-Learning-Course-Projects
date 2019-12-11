/* Simple Expression Evaluator */

/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"


extern LIST   glist[1024];

int yyparse();

int main()
{
    extern int yydebug;
    yydebug = 0;         /* change to 1 to see debugging info */
	
	for (int i = 0;i < 1024;++i) {  //Init expression list
        glist[i].value = 0;
        glist[i].flag = UNEVALUATED;
        glist[i].tree = NULL;
    }
	
	
    yyparse();
	printf("\n");
    return 0;
}
