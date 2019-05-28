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
	case AST_READ:
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
				}
				 break;
			case AST_BYTE:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_BYTE;
				}
				 break;
			case AST_FLOAT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_FLOAT;

				}
				break;
			case AST_CONST_STRING:
				node->symbol->type = LIT_STR;
				break;
			case AST_CONST_INT:
				node->symbol->type = LIT_INTEG;
				break;
			case AST_CONST_FLOAT:
				node->symbol->type = LIT_FLOATING;
				break;
			case AST_CONST_BYTE:
				node->symbol->type = LIT_BYTE;
				break;
			default: result = -1; fprintf(stderr, "ERROR: Type not recognized for identifier %s", node->symbol->text);
   		}
	     }
	     else
	     {
	         fprintf(stderr, "Node has no son: %s", node->symbol->text);
	     }
	     if(node->son[2])
		{
	       	   if(node->son[2]->type == AST_ARRAY)
			node->symbol->nature = VECTOR;
		   else
			node->symbol->nature = SCALAR;
		}
	     break;

	case AST_HEADER:
	case AST_FUNC_CALL:
	    node->symbol->nature = FUNCTION;
	    if (node->son[0])
	    {
		switch(node->son[0]->type)
    		{

        		case AST_INT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_INT;

				}
				 break;
			case AST_BYTE:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_BYTE;

				}
				 break;
			case AST_FLOAT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_FLOAT;

				}
				break;
			case AST_CONST_STRING:
				node->symbol->type = LIT_STR;
				break;
			case AST_CONST_INT:
				node->symbol->type = LIT_INTEG;
				break;
			case AST_CONST_FLOAT:
				node->symbol->type = LIT_FLOATING;
				break;
			case AST_CONST_BYTE:
				node->symbol->type = LIT_BYTE;
				break;
			default: result = -1; fprintf(stderr, "ERROR: Type not recognized for identifier %s", node->symbol->text);
   		}
	     }
	     else
	     {
	         fprintf(stderr, "ERROR: Node has no son: %s", node->symbol->text);
	     }
	     break;


	case AST_ARR_POS:
	    node->symbol->nature = VECTOR;
	    if (node->son[0])
	    {
		switch(node->son[0]->type)
    		{

        		case AST_INT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_INT;

				}
				 break;
			case AST_BYTE:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_BYTE;

				}
				 break;
			case AST_FLOAT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_FLOAT;

				}
				break;
			case AST_CONST_STRING:
				node->symbol->type = LIT_STR;
				break;
			case AST_CONST_INT:
				node->symbol->type = LIT_INTEG;
				break;
			case AST_CONST_FLOAT:
				node->symbol->type = LIT_FLOATING;
				break;
			case AST_CONST_BYTE:
				node->symbol->type = LIT_BYTE;
				break;
			default: result = -1; fprintf(stderr, "ERROR: Type not recognized for identifier %s", node->symbol->text);
   		}
	     }
	     else
	     {
	         fprintf(stderr, "ERROR: Node has no son: %s", node->symbol->text);
	     }
	     break;
	case AST_ATTRIB:
	node->symbol->nature = FUNCTION;
	    if (node->son[0])
	    {
		switch(node->son[0]->type)
    		{

        		case AST_INT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_INT;

				}
				 break;
			case AST_BYTE:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_BYTE;

				}
				 break;
			case AST_FLOAT:
				tableNode = hashFind(node->symbol->text);
				if (tableNode->type == VAR_INT || tableNode->type == VAR_FLOAT || tableNode->type == VAR_BYTE)
				{
					fprintf(stderr, "ERROR: Identifier %s redeclared\n", node->symbol->text);
					result = -1;
				}
				else
				{
					tableNode->type = VAR_FLOAT;

				}
				break;
			case AST_CONST_STRING:
				node->symbol->type = LIT_STR;
				break;
			case AST_CONST_INT:
				node->symbol->type = LIT_INTEG;
				break;
			case AST_CONST_FLOAT:
				node->symbol->type = LIT_FLOATING;
				break;
			case AST_CONST_BYTE:
				node->symbol->type = LIT_BYTE;
				break;
			default: result = -1; fprintf(stderr, "ERROR: Type not recognized for identifier %s", node->symbol->text);
   		}
	     }
	     else
	     {
	         fprintf(stderr, "ERROR: Node has no son: %s", node->symbol->text);
	     }
	     if (node->symbol != 0)
		node->symbol->nature = SCALAR;
	     break;
     }
    return result;
}

int checkUndeclared(AST* node)
{
	int i, result = 0; //result = 0 -> check sucessfull
	NODE *tableNode;
    	if(node == 0)
        	return 0;

	for(i = 0; i < MAX_SONS; ++i )
	{
		if (checkUndeclared(node->son[i]) != 0)
			result = -1;
	}

	switch(node->type)
	{
		case AST_ATTRIB:
		case AST_READ:
		case AST_IDENTIFIER:
		case AST_ARR_POS:
		case AST_FUNC_CALL:
			tableNode = hashFind(node->symbol->text);
			if (tableNode->type != VAR_INT && tableNode->type != VAR_FLOAT && tableNode->type != VAR_BYTE)
			{
				fprintf(stderr, "ERROR: Undeclared identifier: %s\n", node->symbol->text);
				result = -1;
				tableNode->type = VAR_UNDECLARED;
			}
	}
	return result;
}

AST* findFuncDeclaration(NODE *sym, AST* node)
{
	int i;
	AST *next;
	if (node == 0)
		return -1;
	if (node->symbol == sym)
		return node;
	else
	{
		for(i = 0;i<MAX_SONS;i++)
		{
			next = findFuncDeclaration(sym, node->son[i]);
			if(next != -1)
				return next;
		}
	}
}


int checkFunctions(AST *node)
{
	int i, result = 0; //result = 0 -> check sucessfull
	AST *decNode;
	AST *callNode;
    	if(node == 0)
        	return 0;

	for(i = 0; i < MAX_SONS; ++i )
	{
		if (checkFunctions(node->son[i]) != 0)
			result = -1;
	}
	if(node->type == AST_FUNC_CALL)
	{
		if(node->symbol->type != VAR_UNDECLARED)
		{
			decNode = findFuncDeclaration(node->symbol, root)->son[1];
			if(decNode == -1) return -1;
			callNode = node->son[0];
			while(1)
			{
				if(decNode == 0 && callNode == 0)
					return result;
				if(decNode == 0 && callNode != 0)
				{
					fprintf(stderr, "ERROR: Too many operands for function %s\n", node->symbol->text);
					return -1;
				}
				if(decNode != 0 && callNode == 0)
				{
					fprintf(stderr, "ERROR: Too few operands for function %s\n", node->symbol->text);
					return -1;
				}
				if(decNode != 0 && callNode != 0)
				{
					decNode = decNode->son[1];
					callNode = callNode->son[1];
				}
			}
		}
		else return -1;
	}
	else return 0;
}

int checkAritExpressions(AST *node)
{
	int i, result = 0; //result = 0 -> check sucessfull
    	if(node == 0)
        	return 0;
	for(i = 0; i < MAX_SONS; ++i )
	{
		if (checkAritExpressions(node->son[i]) != 0)
			result = -1;
	}
	switch(node->type)
	{
		case AST_ADD:
		case AST_SUB:
		case AST_DIV:
		case AST_MUL:
			if(node->son[0] && node->son[1])
			{
				if(node->son[0]->symbol == 0 || node->son[1]->symbol == 0)
				{
					fprintf(stderr, "ERROR: Arithmetic operands are not numbers\n");
					return -1;
				}

				if(node->son[0]->symbol->type == VAR_FLOAT || node->son[0]->symbol->type == LIT_FLOATING || node->son[1]->symbol->type == VAR_FLOAT || node->son[1]->symbol->type == LIT_FLOATING)
				{
					node->type = VAR_FLOAT;
				}
				else
				{
					if((node->son[0]->symbol->type == LIT_INTEG || node->son[0]->symbol->type == VAR_INT || node->son[0]->symbol->type == VAR_BYTE || node->son[0]->symbol->type == LIT_BYTE) && (node->son[1]->symbol->type == LIT_INTEG || node->son[1]->symbol->type == VAR_INT || node->son[1]->symbol->type == VAR_BYTE || node->son[1]->symbol->type == LIT_BYTE))
						node->type = VAR_INT;
					else
					{
						fprintf(stderr, "ERROR: Incompatible operand %s\n", node->symbol->text);
						return -1;
					}
				}
				return 0;
			}
			return -1;
		case AST_LE:
		case AST_GE:
		case AST_EQ:
		case AST_DIF:
		case AST_GREATER:
		case AST_LESS:
		case AST_OR:
		case AST_AND:
		case AST_NOT:
			if(node->son[0] && node->son[1])
				if ((node->son[0]->type != VAR_INT && node->son[0]->type != VAR_FLOAT && node->son[0]->type != VAR_BYTE && node->son[0]->type != AST_CONST_FLOAT && node->son[0]->type != AST_CONST_INT && node->son[0]->type != AST_CONST_BYTE && node->son[0]->type != AST_ADD && node->son[0]->type != AST_SUB && node->son[0]->type != AST_DIV && node->son[0]->type != AST_MUL && node->son[0]->type != AST_IDENTIFIER) ||
					(node->son[1]->type != VAR_INT && node->son[0]->type != VAR_FLOAT && node->son[1]->type != VAR_BYTE && node->son[1]->type != AST_CONST_FLOAT && node->son[1]->type != AST_CONST_INT && node->son[1]->type != AST_CONST_BYTE && node->son[1]->type != AST_ADD && node->son[1]->type != AST_SUB && node->son[1]->type != AST_DIV && node->son[1]->type != AST_MUL && node->son[1]->type != AST_IDENTIFIER))
				{
					fprintf(stderr, "%d, %d\n", node->son[0]->type, node->son[1]->type);
					fprintf(stderr, "ERROR: Invalid operands for boolean expression\n");
					return -1;
				}
				else return result;
			else
				return -1;
			break;

		default: return result;
	}
}

int checkStrings(AST *node)
{
	int i, result = 0; //result = 0 -> check sucessfull
    	if(node == 0)
        	return 0;
	for(i = 0; i < MAX_SONS; ++i )
	{
		if (checkStrings(node->son[i]) != 0)
			result = -1;
	}
	for(i = 0; i < MAX_SONS; i++)
	{
		if (node->son[i] && node->son[i]->type == AST_CONST_STRING)
			if(node->type != AST_PRINT_ELEM)
			{
				fprintf(stderr, "ERROR: Constant String used outside of print\n");
			}
			else
				return result;
	}

}

int isNumber(AST *node)
{
    int i, result = 0;
    if (node == 0)
        return 0;
    for(i = 0; i < MAX_SONS; ++i )
	{
		if (isNumber(node->son[i]) != 0)
			result = -1;
	}
	if (!(node->symbol && (node->symbol->type == VAR_BYTE || node->symbol->type == VAR_INT || node->type == AST_CONST_BYTE || node->type == AST_CONST_INT)))
    {
        result = -1;
    }
    return result;
}

int checkVectorIndex(AST *node)
{
	int i, result = 0; //result = 0 -> check sucessfull
    	if(node == 0)
        	return 0;
	for(i = 0; i < MAX_SONS; ++i )
	{
		if (checkVectorIndex(node->son[i]) != 0)
			result = -1;
	}
    if (node->type == AST_ARR_POS)
    {
        for(i=0;i<MAX_SONS;i++)
        {
            if (isNumber(node->son[i]) != 0)
                result = -1;
        }
    }
    return result;
}
