
#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

#define MAX_SONS 4


#define AST_DEC_FUNC_LIST       1
#define AST_ARRAY	            2
#define AST_ARRAY_INIT	       	3
#define AST_ARRAY_VALUE       	4
#define AST_GLOBAL_DEC        	5
#define AST_GLOBAL_DEC_INIT 	6
#define AST_FUNCTION  	    	7
#define AST_HEADER          	8
#define AST_PARAMS          	9
#define AST_PARAM              10
#define AST_CMDS        	   11
#define AST_RETURN	           12
#define AST_READ		       13
#define AST_PRINT	           14

#define AST_ADD			15
#define AST_SUB			16
#define AST_MUL			17
#define AST_DIV			18
#define AST_EXPRESSION	19
#define AST_LE			20
#define AST_GE			21
#define AST_EQ			22
#define AST_DIF			23
#define AST_GREATER		24
#define AST_LESS		25
#define AST_OR			26
#define AST_AND			27
#define AST_NOT			28

#define AST_ARR_POS		29
#define AST_FUNC_CALL	30
#define AST_CONST		31
#define AST_ATTRIB		32
#define AST_ARR_ATTRIB	33
#define AST_PRINT_ELEM	34
#define AST_OPERAND		35
#define AST_IDENTIFIER	36
#define AST_LITERAL		37

#define AST_IF			38
#define AST_ELSE		39
#define AST_LOOP		40
#define AST_LEAP		41

#define AST_INT			42
#define AST_BYTE		43
#define AST_FLOAT		44

#define AST_CMD			45
#define AST_BLOCK		46
#define AST_IF_ELSE		47
#define AST_ARGS		48
#define AST_LIT_INTEGER	49
#define AST_LIT_FLOAT	50
#define AST_LIT_CHAR	51
#define AST_LIT_STRING	52
#define AST_DEC_VAR_LIST 53

#define AST_BOOL 1000

typedef struct ast_node{
	int type;
	struct ast_node *son[MAX_SONS];
	NODE* symbol;
	int dataType;
	float value;
	int line;
} AST;

AST* astCreate(int type, NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3, int line);
void astPrint(AST* node, int level);
void compile(AST*node, FILE* out);

#endif
