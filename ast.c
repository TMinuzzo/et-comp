#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern FILE *out;

AST* astCreate(int type, NODE* symbol, AST* son0, AST* son1, AST* son2, AST* son3) {
   AST* newnode;
   newnode = (AST*) calloc(1, sizeof(AST));
   newnode->type = type;
   newnode->symbol = symbol;
   newnode->son[0] = son0;
   newnode->son[1] = son1;
   newnode->son[2] = son2;
   newnode->son[3] = son3;
   return newnode;
}

void astPrint(AST* node, int level) {
	int i;
	if (node == 0) {
		return;
    }
	for(i=0; i<level; ++i) {
		fprintf(stderr, "   ");
    }
	fprintf(stderr, "AST(");
	switch(node->type) {
		case AST_DEC_LIST : 
            fprintf(stderr, "AST_DEC_LIST\n");
            break;
        case AST_ARRAY : 
            fprintf(stderr, "AST_ARRAY\n");
            break;
        case AST_ARRAY_INIT : 
            fprintf(stderr, "AST_ARRAY_INIT\n");
            break;
        case AST_ARRAY_VALUE : 
            fprintf(stderr, "AST_ARRAY_VALUE\n");
            break;
        case AST_GLOBAL_DEC : 
            fprintf(stderr, "AST_GLOBAL_DEC\n");
            break;
        case AST_GLOBAL_DEC_INIT : 
            fprintf(stderr, "AST_GLOBAL_DEC_INIT\n");
            break;
		default: 
            fprintf(stderr, "AST_UNKNOWN\n");
            break;
	}
	for(i=0; i< MAX_SONS; ++i) {
		astPrint(node->son[i], level + 1);
    }
}


/*void compile(AST*node, FILE* out) {

	AST* temp = node;
	if (temp == 0) {
		return;
    }
	switch(temp->type) {
		case AST_DEC_LIST_FUNC :
			compile(temp->son[0], out);
			compile(temp->son[1], out);
			break;
		case AST_DEC_LIST_VAR :
			compile(temp->son[1], out);	
			compile(temp->son[0], out);
			break;
		
		case AST_DEC_VAR : 
			compile(temp->son[0], out);
			fprintf(out,"%s = ", temp->symbol->text);
			compile(temp->son[1], out);
			fprintf(out, ";\n");
			break;
		case AST_DEC_VEC : 
			compile(temp->son[0], out);
			fprintf(out,"%s ", temp->symbol->text);
			fprintf(out, "[");
			compile(temp->son[1], out);
			fprintf(out, "];\n");
			break;
		case AST_DEC_VEC_INIT : 
			compile(temp->son[0], out);
			fprintf(out,"%s ", temp->symbol->text);
			fprintf(out, "[");
			compile(temp->son[1], out);
			fprintf(out, "]: ");
			compile(temp->son[2], out);
			fprintf(out, ";\n");
			break;
		case AST_KW_INT : 
			fprintf(out,"int ");
			break;
        case AST_KW_BYTE : 
			fprintf(out,"byte ");
			break;
		case AST_KW_FLOAT : 
			fprintf(out,"float ");
			break;		
		case AST_LIST_LITERAL : 
			compile(temp->son[0], out);
			fprintf(out," ");
			compile(temp->son[1], out);
			break;
		case AST_LIT_INT : 
			fprintf(out,"%s", temp->symbol->text);
			break;
		case AST_LIT_FLOAT : 
			fprintf(out,"%s", temp->symbol->text);
			break;
		case AST_LIT_CHAR : 
			fprintf(out,"%s", temp->symbol->text);
			break;
		case AST_DEC_FUNC : 
			compile(temp->son[0], out);
			fprintf(out,"%s ", temp->symbol->text);
			fprintf(out, "(");
			compile(temp->son[1], out);
			fprintf(out, ")");
			compile(temp->son[2], out);
			break;
		case AST_FUNC_LIST : 
			compile(temp->son[0], out);
			fprintf(out, ", ");
			compile(temp->son[1], out);
			break;
		case AST_FUNC_PARAM :
		 	compile(temp->son[0], out);
			fprintf(out,"%s ", temp->symbol->text);
			break;
        case AST_BLOCK : 
			fprintf(out, "{\n");
			compile(temp->son[0], out);
			fprintf(out, "\n};");
			break;
		case AST_CMD_BLOCK : 
			fprintf(out, "{\n");
			compile(temp->son[0], out);
			fprintf(out, "}");
			break;
		case AST_CMD_LIST : 
			compile(temp->son[1], out);
			if( temp->son[0] != 0)
			{
				fprintf(out, ";\n");
				compile(temp->son[0], out);
			}	
			break;
        case AST_READ : 
			fprintf(out, "read ");
			fprintf(out,"%s", node->symbol->text);		
			break;
        case AST_LEAP : 
			fprintf(out, "leap");	
			break;
		case AST_LIT_STRING :
			fprintf(out,"%s", node->symbol->text);
			break;
		case AST_RET : 
			fprintf(out, "return ");
			compile(temp->son[0], out);
			break;
		case AST_ADD : 
			compile(temp->son[0], out);
			fprintf(out, " + ");
			compile(temp->son[1], out);
			break;
		case AST_SUB : 
			compile(temp->son[0], out);
			fprintf(out, " - ");
			compile(temp->son[1], out);
			break;
		case AST_MULT : 
			compile(temp->son[0], out);
			fprintf(out, " * ");
			compile(temp->son[1], out);
			break;
		case AST_DIV : 
			compile(temp->son[0], out);
			fprintf(out, " / ");
			compile(temp->son[1], out);
			break;
		case AST_VEC : 
			fprintf(out, "%s[", node->symbol->text);
			compile(temp->son[0], out);
			fprintf(out, "]");
			break;
		case AST_FUNC :
			fprintf(out, "%s(", node->symbol->text);
			compile(temp->son[0], out);
			fprintf(out, ")");
			break;
		case AST_ID : 
			fprintf(out, "%s", node->symbol->text);
			break;
		case AST_LESS : 
			compile(temp->son[0], out);
			fprintf(out, " < ");
			compile(temp->son[1], out);
			break;
		case AST_GREATER : 
			compile(temp->son[0], out);
			fprintf(out, " > ");
			compile(temp->son[1], out);
			break;
		case AST_AND : 
			compile(temp->son[0], out);
			fprintf(out, " and ");
			compile(temp->son[1], out);
			break;
		case AST_OR : 
			compile(temp->son[0], out);
			fprintf(out, " or ");
			compile(temp->son[1], out);
			break;
		case AST_NOT : 
			fprintf(out, "not ");
			compile(temp->son[0], out);
			break;
		case AST_LE : 
			compile(temp->son[0], out);
			fprintf(out, " <= ");
			compile(temp->son[1], out);			
			break;
		case AST_GE : 
			compile(temp->son[0], out);
			fprintf(out, " >= ");
			compile(temp->son[1], out);
			break;
		case AST_DIF : 
			compile(temp->son[0], out);
			fprintf(out, " != ");
			compile(temp->son[1], out);
			break;
		case AST_EQ : 
			compile(temp->son[0], out);
			fprintf(out, " == ");
			compile(temp->son[1], out);
			break;
		case AST_ATTR : 
			fprintf(out,"%s = ", node->symbol->text);
			compile(temp->son[0], out);
			break;
		case AST_ATTR_VEC : 
			fprintf(out,"%s[", node->symbol->text);
			compile(temp->son[0], out);
			fprintf(out, "] = ");
			compile(temp->son[1], out);
			break;
		case AST_LOOP : 
			fprintf(out, "loop (");
			compile(temp->son[0], out);
			fprintf(out, ")\n");
			compile(temp->son[1], out);
			break;
		case AST_IF : 
			fprintf(out, "if( ");
			compile(temp->son[0], out);
			fprintf(out, " ) then\n");
			compile(temp->son[1], out);			
			break;
		case AST_IF_ELSE :
			fprintf(out, "if( ");
			compile(temp->son[0], out);
			fprintf(out, " ) then\n");
			compile(temp->son[1], out);			
			fprintf(out, " else\n");
			compile(temp->son[2], out);
			break;
		case AST_FUNC_PARAMS : 
			compile(temp->son[0], out);
			fprintf(out, ", ");
			compile(temp->son[1], out);
			break;
		case AST_PRINT : 
			fprintf(out, "print ");
			compile(temp->son[0], out);
			compile(temp->son[1], out);		
			break;
		case AST_PRINT_PARAMS : 
			if( temp->son[0] != 0){
				compile(temp->son[0], out);
				fprintf(out,", ");
			}
			compile(temp->son[1], out);
			break;	

		default: 
            fprintf(stderr, "AST_UNKNOWN\n"); 
            break;
	}
    */
}
