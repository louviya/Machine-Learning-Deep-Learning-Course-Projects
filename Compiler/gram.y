/*
 *
 * yacc/bison input for simplified C++ parser
 *
 */

%{

#include "defs.h"
#include "types.h"
#include "symtab.h"
#include "bucket.h"
#include "message.h"
#include "tree.h"
#include <stdio.h>


    int yylex();
    int yyerror(char *s);
    

%}

%union {
	int	y_int;
	unsigned int y_uint;
	double	y_double;
	char *	y_string;
	TN	y_tree;
	ST_ID	y_stid;
	BUCKET_PTR y_bucket;
	TYPE_SPECIFIER y_type_spec;
	TYPE y_type;
	CONSTT y_const_rec;
	PARAM_LIST y_param_list;
};

%type <y_bucket> declaration_specifiers 
%type <y_type_spec> type_specifier struct_or_union type_qualifier storage_class_specifier
%type <y_tree> init_declarator pointer declarator direct_declarator parameter_type_list  
%type <y_stid> identifier 
%type <y_param_list> parameter_list parameter_declaration
%type <y_uint> initializer_list

%type <y_const_rec> primary_expr postfix_expr unary_expr cast_expr multiplicative_expr expr assignment_expr
%type <y_const_rec> additive_expr shift_expr relational_expr equality_expr and_expr
%type <y_const_rec> exclusive_or_expr inclusive_or_expr logical_and_expr
%type <y_const_rec> logical_or_expr conditional_expr constant_expr
%token <y_int> INT_CONSTANT
%token <y_double> DOUBLE_CONSTANT
%token <y_string> IDENTIFIER 


%token STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token <y_type_spec> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <y_type_spec> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <y_type_spec> STRUCT UNION ENUM ELIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token BAD

%start translation_unit
%%

 /*******************************
  * Expressions                 *
  *******************************/

primary_expr
	: identifier		{}
	| INT_CONSTANT		{ $$ = build_int($1); }
	| DOUBLE_CONSTANT	{ $$ = build_double($1); }
	| STRING_LITERAL    {}
	| '(' expr ')'      { $$ = $2; }
	;

postfix_expr
	: primary_expr      { $$ = $1; }
	| postfix_expr '[' expr ']'
	| postfix_expr '(' argument_expr_list_opt ')'
	| postfix_expr '.' identifier
	| postfix_expr PTR_OP identifier
	| postfix_expr INC_OP
	| postfix_expr DEC_OP
	;

argument_expr_list_opt
	: /* null derive */
	| argument_expr_list
	;

argument_expr_list
	: assignment_expr
	| argument_expr_list ',' assignment_expr
	;

unary_expr
	: postfix_expr              { $$ = $1; }
	| INC_OP unary_expr         {}
	| DEC_OP unary_expr         {}
	| unary_operator cast_expr  {}
	| SIZEOF unary_expr         {}
	| SIZEOF '(' type_name ')'  {}
	;

unary_operator
	: '&' | '*' | '+' | '-' | '~' | '!'
	;

cast_expr
	: unary_expr                    { $$ = $1; }
	| '(' type_name ')' cast_expr   {}
	;

multiplicative_expr
	: cast_expr                         { $$ = $1; }
	| multiplicative_expr '*' cast_expr	{ /* $$ = add_mult_node($1, $3); */ }
	| multiplicative_expr '/' cast_expr	{ /* $$ = add_div_node($1, $3); */ }
	| multiplicative_expr '%' cast_expr	{ /* $$ = add_mod_node($1, $3); */ }
	;

additive_expr
	: multiplicative_expr                   { $$ = $1; }
	| additive_expr '+' multiplicative_expr	{ /* $$ = add_add_node($1, $3); */ }
	| additive_expr '-' multiplicative_expr	{ /* $$ = add_sub_node($1, $3); */ }
	;

shift_expr
	: additive_expr                     { $$ = $1; }
	| shift_expr LEFT_OP additive_expr	{ /* $$ = add_lshift_node($1, $3); */ }
	| shift_expr RIGHT_OP additive_expr	{ /* $$ = add_rshift_node($1, $3); */ }
	;


relational_expr
	: shift_expr                        { $$ = $1; }
	| relational_expr '<' shift_expr	{ /* $$ = add_lt_node($1, $3); */ }
	| relational_expr '>' shift_expr	{ /* $$ = add_gt_node($1, $3); */ }
	| relational_expr LE_OP shift_expr	{ /* $$ = add_lte_node($1, $3); */ }
	| relational_expr GE_OP shift_expr	{ /* $$ = add_gte_node($1, $3); */ }
	;

equality_expr
	: relational_expr                       { $$ = $1; }
	| equality_expr EQ_OP relational_expr	{ /* $$ = add_eq_node($1, $3); */ }
	| equality_expr NE_OP relational_expr	{ /* $$ = add_neq_node($1, $3); */ }
	;

and_expr
	: equality_expr                   { $$ = $1; }
	| and_expr '&' equality_expr
	;

exclusive_or_expr
	: and_expr                        { $$ = $1; }
	| exclusive_or_expr '^' and_expr
	;

inclusive_or_expr
	: exclusive_or_expr               { $$ = $1; }
	| inclusive_or_expr '|' exclusive_or_expr
	;

logical_and_expr
	: inclusive_or_expr                         { $$ = $1; }
	| logical_and_expr AND_OP inclusive_or_expr	{ /* $$ = add_log_and_node($1, $3); */ }
	;

logical_or_expr
	: logical_and_expr                          { $$ = $1; }
	| logical_or_expr OR_OP logical_and_expr	{ /* $$ = add_log_or_node($1, $3); */ }
	;

conditional_expr
	: logical_or_expr                               { $$ = $1; }
	| logical_or_expr '?' expr ':' conditional_expr	{ /* $$ = add_cond_expr_node($1, $3, $5); */ }
	;

assignment_expr
	: conditional_expr                               { $$ = $1; }
	| unary_expr assignment_operator assignment_expr
	;

assignment_operator
	: '=' | MUL_ASSIGN | DIV_ASSIGN | MOD_ASSIGN | ADD_ASSIGN | SUB_ASSIGN
	| LEFT_ASSIGN | RIGHT_ASSIGN | AND_ASSIGN | XOR_ASSIGN | OR_ASSIGN
	;

expr
	: assignment_expr
	| expr ',' assignment_expr
	;

constant_expr
	: conditional_expr              { $$ = $1; }
	;

expr_opt
	: /* null derive */
	| expr
	;

 /*******************************
  * Declarations                *
  *******************************/

declaration
	: declaration_specifiers ';'			{ yyerror("there is no declarator"); }
	| declaration_specifiers init_declarator_list ';'			
	;

declaration_specifiers
	: storage_class_specifier				            { /*$$ = update_bucket(NULL, $1, NULL);*/ }
	| storage_class_specifier declaration_specifiers	{ /*$$ = update_bucket($2, $1, NULL); */ }
	| type_specifier					                { $$ = update_bucket(NULL, $1, NULL); }
	| type_specifier declaration_specifiers			    { $$ = update_bucket($2, $1, NULL); }
	| type_qualifier					                { /*$$ = update_bucket(NULL, $1, NULL);*/ }
	| type_qualifier declaration_specifiers			    { /*$$ = update_bucket($2, $1, NULL);*/ }
	;

init_declarator_list
	: init_declarator					            { install_generate($<y_bucket>0, $1); }
	| init_declarator_list ',' init_declarator		{ install_generate($<y_bucket>0, $3); }
	;

init_declarator
	: declarator	   { $$ = $1; }
	| declarator '=' initializer
	;

storage_class_specifier
	: TYPEDEF	{ $$ = TYPEDEF_SPEC; }
	| EXTERN	{ $$ = EXTERN_SPEC; }
	| STATIC	{ $$ = STATIC_SPEC; }
	| AUTO		{ $$ = AUTO_SPEC; }
	| REGISTER	{ $$ = REGISTER_SPEC; }
	;

type_specifier
	: VOID		{ $$ = VOID_SPEC; }
	| CHAR		{ $$ = CHAR_SPEC; }
	| SHORT		{ $$ = SHORT_SPEC; }
	| INT		{ $$ = INT_SPEC; }
	| LONG		{ $$ = LONG_SPEC; }
	| FLOAT		{ $$ = FLOAT_SPEC; }
	| DOUBLE	{ $$ = DOUBLE_SPEC; }
	| SIGNED	{ $$ = SIGNED_SPEC; }
	| UNSIGNED	{ $$ = UNSIGNED_SPEC; }
	| struct_or_union_specifier     {}
	| enum_specifier                {}
	| TYPE_NAME	{ $$ = TYPENAME_SPEC; }
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	| struct_or_union identifier '{' struct_declaration_list '}'
	| struct_or_union identifier
	;

struct_or_union
	: STRUCT	{ $$ = STRUCT_SPEC; }
 	| UNION		{ $$ = UNION_SPEC; }
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list_opt
	| type_qualifier specifier_qualifier_list_opt
	;

specifier_qualifier_list_opt
	: /* null derive */
	| specifier_qualifier_list
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expr
	| declarator ':' constant_expr
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM identifier '{' enumerator_list '}'
	| ENUM identifier
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: identifier
	| identifier '=' constant_expr
	;

type_qualifier
	: CONST						{ $$ = CONST_SPEC; }
 	| VOLATILE					{ $$ = VOLATILE_SPEC; }
  	;

declarator
	: direct_declarator			{ $$ = $1; }
	| pointer declarator		{ $1->next = $2; $$ = $1; }
	;

direct_declarator
	: identifier				{	
		TN node = add_node(NULL);
		node->type = NODE_ID;
		node->u.st_id = $1;
		$$ = node;
	}
	| '(' declarator ')'				{ $$ = $2; }
	| direct_declarator '[' ']'			
	| direct_declarator '[' constant_expr ']'	{	
		TN node = add_node($1);
		CONSTT rec = $3;
		node->type = NODE_ARRAY;
		if (rec->type != CONST_INT)
			yyerror("There should be an int dimension of array.");
		if (rec->u.int_val <= 0)
			yyerror("The dimenssion of array cannot be negative.");
		node->u.array_dim = rec->u.int_val;
		$$ = node;
	}
	| direct_declarator '(' parameter_type_list ')' {
		TN node = add_node($1);
		node->type = NODE_FUNC;
		node->u.func_node.param_style = $3->u.func_node.param_style;
		node->u.func_node.params = $3->u.func_node.params;
		$$ = node;
	}
	| direct_declarator '(' ')'			{ 	
		TN node = add_node($1);
		node->type = NODE_FUNC;
		node->u.func_node.param_style = PROTOTYPE;
		$$ = node;
	}
	;

pointer
	: '*' specifier_qualifier_list_opt		{	
		TN node = add_node(NULL);
		node->type = NODE_PTR;
		$$ = node;
	}
	| '&'	{	
		TN node = add_node(NULL);
		node->type = NODE_REF;
		$$ = node;
	}
	;

parameter_type_list
	: parameter_list	{
		TN node = add_node(NULL);
		if($1 != NULL)  {
			PARAM_LIST p = reverse_list($1);
			check_params(p);
			node->type = NODE_PARAM;
			node->u.func_node.params = p;
			node->u.func_node.param_style = OLDSTYLE;
		}
	    $$ = node;
	}
	| parameter_list ',' ELIPSIS			{ }
	;

parameter_list
	: parameter_declaration				{ $$ = $1; }
	| parameter_list ',' parameter_declaration	{ 
		if($3 != NULL){
			$3->next = $1;
			$1->prev = $3;
		}
		$$ = $3;
	}
	;

parameter_declaration
	: declaration_specifiers declarator		{ $$ = build_param_list($1, $2); }
	| declaration_specifiers			{ yyerror("There is no ids in parameter list"); $$ = NULL; }
	| declaration_specifiers abstract_declarator	{  }
	;

type_name
	: specifier_qualifier_list			
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer					
	| direct_abstract_declarator			
	| pointer abstract_declarator			
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'			
	| '[' ']'					
	| '[' constant_expr ']'				
	| direct_abstract_declarator '[' ']'		
	| direct_abstract_declarator '[' constant_expr ']'	
	| '(' ')'						
	| '(' parameter_type_list ')'				
	| direct_abstract_declarator '(' ')'				
	| direct_abstract_declarator '(' parameter_type_list ')'	
	;

initializer 
	: assignment_expr 
	| '{' initializer_list comma_opt '}'
	;

comma_opt
	: /* Null derive */
	| ','
	;

initializer_list 
	: initializer	{ $$ = 1; }
	| initializer_list ',' { $<y_type>$ = $<y_type>0; } initializer { $$ = $1 + 1; }
	;

 /*******************************
  * Statements                  *
  *******************************/

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: identifier ':' statement
	| CASE constant_expr ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	| '{' declaration_list statement_list '}'
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement
	| statement_list statement
	;

expression_statement
	: expr_opt ';'
	;

selection_statement
	: IF '(' expr ')' statement
	| IF '(' expr ')' statement ELSE statement
	| SWITCH '(' expr ')' statement
	;

iteration_statement
	: WHILE '(' expr ')' statement
	| DO statement WHILE '(' expr ')' ';'
	| FOR '(' expr_opt ';' expr_opt ';' expr_opt ')' statement
	;

jump_statement
	: GOTO identifier ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN expr_opt ';'
	;

 /*******************************
  * Top level                   *
  *******************************/

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declarator compound_statement
	| declaration_specifiers declarator compound_statement
	;

 /*******************************
  * Identifiers                 *
  *******************************/

identifier
	: IDENTIFIER	{ $$ = st_enter_id($1); }
	;
%%

extern int column;

int yyerror(char *s)
{
	error("%s (column %d)",s,column);
        return 0;  /* never reached */
}