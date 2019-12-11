#ifndef TREE_H_
#define TREE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EVALUATED  1
#define EVALUATING 0
#define UNEVALUATED -1

enum types{INT_CONST, VAR_CONST, OP_NODE, UNOP_NODE};



typedef struct tn{
	int tag;
    union{
		int int_const_val;
		char var_val;
		struct {
			char op;
			struct tn *left, *right;
		} op_node;
		struct {
			char unop;
			struct tn *child;
		} unop_node;
	} u;
} TREE_NODE, *TN;

typedef struct list{
	int value;
	int flag;
	TN  tree;
} LIST;




TN make_int_const_node(int val);
TN make_var_node(int val);
TN make_op_node(char op, TN left, TN right);
TN make_unop_node(char op, TN child);

#endif