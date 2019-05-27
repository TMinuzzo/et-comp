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

 

    switch (node->type)
    {
	NODE *tableNode;
	case AST_GLOBAL_DEC:
	case AST_HEADER:
	case AST_PARAM:	
	case AST_GLOBAL_DEC_INIT:
	    if (node->son[0])
	    {
		switch(node->son[0]->type)
    		{
			
        		case AST_INT: 
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_INT;
					node->type = VAR_INT;
				}
				 break;
			case AST_BYTE: 
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_BYTE;
					node->type = VAR_BYTE;
				}
				 break;
			case AST_FLOAT: 
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_FLOAT;
					node->type = VAR_FLOAT;
				}
				break;
			case AST_CONST_STRING: 
				node->symbol->type = LIT_STR;
				break;
			case AST_CONST_INT: 
				node->symbol->type = LIT_INTEGER;
				break;
			case AST_CONST_FLOAT: 
				node->symbol->type = LIT_FLOATING;
				break;
			case AST_CONST_BYTE: 
				node->symbol->type = LIT_BYTE;
				break;
			default: result = -1; fprintf(stderr, "Type not recognized for identifier %s", node->symbol->text);            
   		}
	     }
	     else
	     {
	         fprintf(stderr, "Node has no son: %s", node->symbol->text);
	     }
	     break;
     }
    return result;
}

int checkUndeclared(AST* node)
{
	int i, result = 0; //result = 0 -> check sucessfull
    	if(node == 0)
        	return 0;
	    
	for(i = 0; i < MAX_SONS; ++i )
	{
		if (checkUndeclared(node->son[i]) != 0)
			result = -1;
	}

}
