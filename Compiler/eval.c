#include <stdio.h>
#include <stdlib.h>
#include "eval.h"

extern LIST glist[1024];
extern int get_val(int);
extern void yyerror(char *s);

int eval(TN t){
    int left_val, right_val, ret=0;
	switch(t->tag){
	    case INT_CONST:
		    ret = t->u.int_const_val;
			break;
		case VAR_CONST:
		    ret = get_val(t->u.var_val);
			break;
		case OP_NODE:
		    left_val = eval(t->u.op_node.left);
		    right_val = eval(t->u.op_node.right);
		    switch(t->u.op_node.op){
		        case '+': return left_val+right_val;
				case '-': return left_val-right_val;
				case '*': return left_val*right_val;
				case '/': return left_val/right_val;
				case '%': return left_val%right_val;
				default: 
				break;
				}
			break;
		case UNOP_NODE:
		    switch(t->u.unop_node.unop){
			    case '+': ret = eval(t->u.unop_node.child); break;
				case '-': ret = (-1)*eval(t->u.unop_node.child); break;
				case '#': {
				    int index = eval(t->u.unop_node.child);
				    if((index <0) || (index >= 1024) || (glist[index].tree == NULL)){
					    yyerror("index out of range\n");
						exit(-1);
					}
					else {
						if(glist[index].flag ==EVALUATED) {ret = glist[index].value;}
						else if(glist[index].flag == UNEVALUATED) {
						    glist[index].flag = EVALUATING;
							ret = eval(glist[index].tree);
							glist[index].flag = EVALUATED;
							glist[index].value = ret;
							}
						else {
							yyerror("circular reference found\n");
							exit(-1);
						}
				    }
			    }
				default: 
				break;
			} 
		default: 
		break;
	}
	return ret;
}