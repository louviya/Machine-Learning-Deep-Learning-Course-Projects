#ifndef TREE_H
#define TREE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "types.h"
#include "bucket.h"
#include "backend-x86.h"
#include "message.h"
#include "symtab.h"



typedef enum {NODE_PTR, NODE_ARRAY, NODE_ID, NODE_FUNC, NODE_PARAM_LIST, 
NODE_REF, NODE_PARAM_DECL, NODE_PARAM} TN_TYPE;
typedef struct tree_node {
	TN_TYPE type;
	struct tree_node* next;
	union {
		int array_dim;
		ST_ID st_id;
		struct func_node {
			PARAMSTYLE param_style;
			PARAM_LIST params;
			TYPE param_type;
		} func_node;
	} u;
} TREE_NODE, *TN;

typedef enum {CONST_INT, CONST_DOUBLE} CONST_TYPE;
typedef struct const_expr {
	CONST_TYPE type;
	union {
		int int_val;
		double double_val;	
	} u;
} CONST_EXPR, *CONSTT;



/* add a new node as the head of the given tree*/
TN add_node(TN node);

/* go through the tree top-down, building up the full TYPE*/
TYPE build_full_type(BUCKET_PTR bucket, TN root);

/* through the tree find the id*/
ST_ID get_idd(TN node);

PARAM_LIST build_param_list(BUCKET_PTR bucket, TN root);

TN build_param_tree(TN root);

/* whether the given tree has a reference type node */
int is_ref(TN root);

CONSTT build_int(int val);
CONSTT build_double(double val);

/*integrated installment of ST_ID to symbol table, returns whether installation successful*/
BOOLEAN install_global(TYPE t, ST_ID id, STORAGE_CLASS sc);

/*through the parameter list check for duplicate ids. If a duplicate*/
void check_params(PARAM_LIST list);

/*integrated steps for spitting out assembly code */
void gen_assembly(TYPE t, ST_ID id);

/*Check for errors of function*/
void check_func_type(TYPE t);

/*calculate and return the total number of elements in the array recursively*/
int cal_array_size(TYPE t, TYPETAG* RETURN_basic_type);

void install_generate(BUCKET_PTR bucket, TN tree);

PARAM_LIST reverse_list(PARAM_LIST plist);

#endif
