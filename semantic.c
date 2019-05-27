#include "semantic.h"
#include "ast.h"


int setAndCheckRedeclared(AST *node)
{
    int i, result = 0; //result = 0 -> check sucessfull
    if(node == 0)
        return 0;
    
    for(i = 0; i < MAX_SONS; ++i )
    {
        if (setAndCheckRedeclared(node->son[i]) != 0)
	result = -1;
    }

 
    if (node->type == AST_GLOBAL_DEC || node->type == AST_HEADER || node->type == AST_PARAM)
    switch (node->type)
    {
	NODE *tableNode;
	case AST_GLOBAL_DEC:
	case AST_HEADER:
	case AST_PARAM:	
	case AST_GLOBAL_DEC_INIT:
	    if (node->son[0])
		switch(node->son[0]->type)
    		{
        		case AST_INT:// node->symbol->type = VAR_INT;
				fprintf(stderr, "node text %s, type %d\n", node->symbol->text, tableNode->type);
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT)
				{
					fprintf(stderr, "Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_INT;	
				}
				 break;
			case AST_BYTE: //node->symbol->type = VAR_BYTE;
				tableNode = hashFind(node->symbol->text);
				fprintf(stderr, "node text %s, type %d\n", node->symbol->text, tableNode->type);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_BYTE;	
				}
				 break;
			case AST_FLOAT: 
				//node->symbol->type = VAR_FLOAT; 
				fprintf(stderr, "node text %s, type %d\n", node->symbol->text, tableNode->type);
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_FLOAT;
				}
				break;

			default: result = -1; fprintf(stderr, "Type not recognized for identifier %s", node->symbol->text);            
   		 }
	     else
	     {
	         fprintf(stderr, "Node has no son: %s", node->symbol->text);
	     }
	     break;
     }
    return result;
}
