#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"



TN make_int_const_node(int val){
	TN ret;
	ret = (TN)malloc(sizeof(TREE_NODE));
	ret->tag = INT_CONST;
	ret->u.int_const_val = val;
    return ret;
}

TN make_var_node(int val){
	TN ret;
	ret = (TN)malloc(sizeof(TREE_NODE));
	ret->tag = VAR_CONST;
	ret->u.var_val = val;
    return ret;
}

TN make_op_node(char op, TN left, TN right){
	TN ret;
	ret = (TN)malloc(sizeof(TREE_NODE));
	ret->tag = OP_NODE;
	ret->u.op_node.op = op;
	ret->u.op_node.left = left;
	ret->u.op_node.right = right;
	return ret;
}

TN make_unop_node(char op, TN child){
	TN ret;
	ret = (TN)malloc(sizeof(TREE_NODE));
	ret->tag = UNOP_NODE;
	ret->u.unop_node.unop = op;
	ret->u.unop_node.child = child;
	return ret;
}

	