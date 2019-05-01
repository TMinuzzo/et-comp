
#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAX_SONS 4


#define AST_DEC_LIST    	1
#define AST_ARRAY	        2
#define AST_ARRAY_INIT	    3
#define AST_ARRAY_VALUE    	4
#define AST_GLOBAL_DEC     	5
#define AST_GLOBAL_DEC_INIT 6
#define AST_FUNCTION  	    7
#define AST_HEADER       	8
#define AST_PARAMS      	9
#define AST_PARAM       	10
#define AST_CMDS        	11

typedef struct ast_node{
	int type;
	struct ast_node *son[MAX_SONS];
	NODE* symbol;
} AST;

FILE *out;
AST* astCreate(int type, NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3);
void astPrint(AST* node, int level);
void compile(AST*node, FILE* out);

#endif
