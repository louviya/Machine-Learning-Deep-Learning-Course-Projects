#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include <string.h>


TN add_node(TN node){
    TN nnode = (TN)malloc(sizeof(TREE_NODE));
    nnode->next = node;
    return nnode;
}


TYPE build_full_type(BUCKET_PTR bucket, TN root){
	TYPE t = build_base(bucket);
    TN node = root;
	TYPE_QUALIFIER qualifier = get_qual(bucket);
    while (node != NULL){
		switch (node->type){
			case NODE_PTR:
				t = ty_build_ptr(t, qualifier);
				break;
			case NODE_ARRAY:
				t = ty_build_array(t, DIM_PRESENT, node->u.array_dim);
				break;
			case NODE_FUNC:
				t = ty_build_func(t, PROTOTYPE, node->u.func_node.params);
				break;
			default:
				break;
		}
		node = node->next;
    }
	return t;
}


ST_ID get_idd(TN node) {
	ST_ID id;
	while(node != NULL) {
		if(node->type == NODE_ID) {
			id = node->u.st_id;
		}
		node = node->next;
	}
	return id;
}


PARAM_LIST build_param_list(BUCKET_PTR bucket, TN root){
	TYPE t = build_full_type(bucket, root);
	ST_ID id = get_idd(root);
	PARAM_LIST plist = plist_alloc();
	plist->id = id;
	plist->type = t;
	plist->qu = get_qual(bucket);
	plist->sc = get_class(bucket);
	if (is_ref(root) == TRUE)
		plist->is_ref = TRUE;
	else
		plist->is_ref = FALSE;
	return plist;
}

BOOLEAN is_ref(TN root){
	TN iter = root;
	while (iter != NULL) {
		TN_TYPE type = iter->type;
		if (iter->type == NODE_REF)
			return TRUE;
		iter = iter->next;
	}
	return FALSE;
}

TN build_param_tree(TN root){
	TN node = root;
	while(node != NULL){
		if(node->type == NODE_PARAM_DECL){
			printf("declaration\n");
		}
		else {
			printf("not declaration\n");
		}
		node = node->next;
	}
	return root;
}


CONSTT build_int(int val) {
	CONSTT rec = (CONSTT)malloc(sizeof(CONST_INT));
	rec->type = CONST_INT;
	rec->u.int_val = val;
	return rec;
}

CONSTT build_double(double val) {
	CONSTT rec = (CONSTT)malloc(sizeof(CONST_DOUBLE));
	rec->type = CONST_DOUBLE;
	rec->u.double_val = val;
	return rec;
}

BOOLEAN install_global(TYPE t, ST_ID id, STORAGE_CLASS sc){
	ST_DR st_dr = stdr_alloc();
	st_dr->tag = GDECL;
	st_dr->u.decl.type = t;
	st_dr->u.decl.sc = sc;   
	return st_install(id, st_dr);
}

void check_params(PARAM_LIST list){
	PARAM_LIST plist1 = list;
	PARAM_LIST plist2 = list;
	while(plist1 != NULL){
		plist2 = plist2->next;
		while(plist2 != NULL){
			if(strcmp(st_get_id_str(plist1->id), st_get_id_str(plist2->id)) == 0){
				error("%s", "It is a duplicate parameter declaration for %s", st_get_id_str(plist1->id));
			}
			plist2 = plist2->next;
		}
		plist1 = plist1->next;
		plist2 = plist1;
	}
}

void gen_assembly(TYPE t, ST_ID id){
	TYPETAG type_tag = ty_query(t);
	char *identy = st_get_id_str(id);
	unsigned int basic_type_size;
	unsigned int full_size;
	if (type_tag == TYARRAY){
		full_size = cal_array_size(t, &type_tag);
		if(full_size == 0){
			return;
		}
		basic_type_size = get_size_basic(type_tag);
		full_size *= basic_type_size;
	}	
	else if(type_tag == TYFUNC){
		check_func_type(t);
		return;
	}
	else{
		basic_type_size = get_size_basic(type_tag);
		full_size = basic_type_size;
	}
	b_global_decl(identy, basic_type_size, full_size);
	b_skip(full_size);
}

void check_func_type(TYPE t){
	PARAMSTYLE paramstyle;
	PARAM_LIST params;
	TYPE type = ty_query_func(t, &paramstyle, &params);
	TYPETAG type_tag = ty_query(type);
	if(type_tag == TYFUNC){
		error("%s", "It cannot return a function.");
	} 
	else if (type_tag == TYARRAY) {
		error("%s", "It cannot return an array.");
	}
}

int cal_array_size(TYPE t, TYPETAG* return_type){
	unsigned int dimensions;
	DIMFLAG dim_flag;
	TYPE type = ty_query_array(t, &dim_flag, &dimensions);
	TYPETAG type_tag = ty_query(type);
	if (type_tag == TYARRAY){
		return dimensions * cal_array_size(type, return_type);
	}
	else{
		if(type_tag == TYFUNC){
			error("%s", "It cannot be an array of functions.");
		}
		else if(type_tag == TYBITFIELD){
			error("%s", "It cannot be an array of bitfileds.");
		}
		*return_type = type_tag;
		return dimensions;
	}
}

void install_generate(BUCKET_PTR bucket, TN tree){
	ST_ID id = get_idd(tree);
	TYPE t = build_full_type(bucket, tree);
	BOOLEAN install = FALSE;
	STORAGE_CLASS sc = get_class(bucket);
	install = install_global(t, id, sc);
	if (install == FALSE){
		error("There exists a duplicate declaration for %s", st_get_id_str(id));
	}
	else{
		gen_assembly(t, id);
	}
}

PARAM_LIST reverse_list(PARAM_LIST plist){
	PARAM_LIST prev = NULL;
	PARAM_LIST current = plist;
	PARAM_LIST next = NULL;
	while(current != NULL){
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return prev;
}