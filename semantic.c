#include "semantic.h"

int SemanticErrorFlag;
AST* rootNode = 0;

void setDeclaration(AST* node){
	int i;
	if (node == 0)
    {
        return;
    }
	if (rootNode == 0)
    {
        rootNode = node;
    }
	for(i = 0; i < MAX_SONS; i++)
    {
		setDeclaration(node->son[i]);
    }
	switch(node->type)
    {
		case AST_GLOBAL_DEC_INIT:
			if (node->symbol == 0 && node->son[0])
            {
                return;
            }
			if(node->symbol->type != SYMBOL_IDENTIFIER)
            {
				fprintf(stderr, "Linha %i: Redeclaração %s.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->symbol->type = SYMBOL_SCALAR;
			if (node->son[0]->type == AST_INT || node->son[0]->type == AST_BYTE)
            {
				node->symbol->datatype = DATATYPE_INT;
            }
			else
            {
				node->symbol->datatype = DATATYPE_FLOAT;
            }
			node->dataType  = node->symbol->datatype;
		    break;

		// Inicialização e declaração de vetor
		case AST_GLOBAL_DEC:
			if (node->symbol == 0 && node->son[0])
            {
                return;
            }
			if(node->symbol->type != SYMBOL_IDENTIFIER)
            {
				fprintf(stderr, "Linha %i: Redeclaração %s.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->symbol->type = SYMBOL_ARRAY;
			if (node->son[0]->type == AST_INT|| node->son[0]->type == AST_BYTE)
            {
				node->symbol->datatype = DATATYPE_INT;
            }
			else
            {
				node->symbol->datatype = DATATYPE_FLOAT;
            }
			node->dataType  = node->symbol->datatype;
		break;

		case AST_HEADER:
			if (node->symbol == 0  && node->son[0])
            {
                return;
            }
			if(node->symbol->type != SYMBOL_IDENTIFIER)
            {
				fprintf(stderr, "Linha %i: Redeclaração %s.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->symbol->type = SYMBOL_FUNCTION;
			if (node->son[0]->type == AST_INT || node->son[0]->type == AST_BYTE)
            {
				node->symbol->datatype = DATATYPE_INT;
            }
			else
            {
				node->symbol->datatype = DATATYPE_FLOAT;
            }
			node->dataType  = node->symbol->datatype;
		break;

		case AST_PARAM:
			if (node->symbol == 0  && node->son[0])
            {
                return;
            }
			if(node->symbol->type != SYMBOL_IDENTIFIER)
            {
				fprintf(stderr, "Linha %i: Redeclaração %s.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->symbol->type = SYMBOL_SCALAR;
			if (node->son[0]->type == AST_INT || node->son[0]->type == AST_BYTE)
            {
					node->symbol->datatype = DATATYPE_INT;
            }
			else
            {
				node->symbol->datatype = DATATYPE_FLOAT;
            }
			node->dataType  = node->symbol->datatype;
		break;
	}
}

// Checagem por conta da tabela hash
void checkUndeclared(){
	hashCheckUndeclared();
}

/*
	Funcition: Realiza a checagem do uso correto das expressões conforme a operação associada
	Parameters:
		AST* node: Ponteiro para nodo a ser analisado
	Return: Não retorna nenhum valor, mas altera a variavel global "SemanticErrorFlag" para 1 se identificar erro
*/
void checkOperands(AST* node){
	int i;
	int err;
	if (node == 0 )
    {
        return;
    }
	for ( i = 0; i < MAX_SONS; i++)
    {
		checkOperands(node->son[i]);
    } 
	switch(node->type)
    {
		case AST_PARAM:
            if(node->symbol == 0){return;}
			node->dataType = node->symbol->datatype;
			break;

		case AST_BYTE:
            node->dataType = DATATYPE_BYTE;
		case AST_INT:
			node->dataType = DATATYPE_INT;
		    break;

		case AST_FLOAT:
			node->dataType = DATATYPE_FLOAT;
		    break;

		case AST_LIT_INTEGER:
            if(node->symbol == 0){return;}
			node->dataType = DATATYPE_INT;
			node->symbol->datatype = DATATYPE_INT;
			node->symbol->type = SYMBOL_LITINT;
			node->value = atof(convertValueToNumberString(node->symbol->text));
		    break;

		case AST_LIT_CHAR:
            if(node->symbol == 0){return;}
			node->dataType = DATATYPE_INT;
			node->symbol->datatype = DATATYPE_INT;
			node->symbol->type = SYMBOL_LITCHAR;
			node->value = atof(node->symbol->text);            
		    break;

		case AST_LIT_FLOAT:
            if(node->symbol == 0){return;}
			node->dataType = DATATYPE_FLOAT;
			node->symbol->datatype = DATATYPE_FLOAT;
			node->symbol->type = SYMBOL_LITFLOAT;
			node->value = atof(convertValueToNumberString(node->symbol->text));
		    break;

		case AST_LIT_STRING:
            if(node->symbol == 0){return;}
			node->dataType = DATATYPE_STRING;
			node->symbol->datatype = DATATYPE_STRING;
			node->symbol->type = SYMBOL_LITSTRING;
			break;

		case AST_GLOBAL_DEC_INIT:
            if(node->symbol == 0){return;}
			if( node->son[0]->dataType != node->son[1]->dataType )
            {
				fprintf(stderr, "Linha %i: Veriável %s inicializada precisa ser do mesmo tipo.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->symbol->datatype;
		    break;

		case AST_ARRAY:
            if(node->symbol == 0){return;}
			if(  isFloat(node->son[0]->dataType) || isBoolean(node->son[0]->dataType) )
            {
				fprintf(stderr, "Linha %i: Array %s precisa ser inicializado com um valor inteiro\n",node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->symbol->datatype;
		    break;
        /*
		case AST_DEC_VEC_INIT:
			if( isFloat(node->son[1]->dataType) || isBoolean(node->son[1]->dataType) )
            {
				fprintf(stderr, "[SEMANTIC] - Line %i: Size in array %s must be an integer\n",node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->symbol->datatype;
			err = checkArray(node);
			switch (err) 
            {
				case 1:
					fprintf(stderr, "[SEMANTIC] - Line %i: Array %s initialisation must have all elements the same type as the array.\n", node->line, node->symbol->text);
					SemanticErrorFlag = 1;
				    break;
				case 2:
				 	fprintf(stderr,"[SEMANTIC] - Line %i:Array %s is missing parameters \n", node->line, node->symbol->text);
					SemanticErrorFlag = 1;
				    break;
				case 3:
					fprintf(stderr,"[SEMANTIC] - Line %i: Array %s has too much parameters \n", node->line, node->symbol->text);
					SemanticErrorFlag = 1;
				    break;
				default : break;
			}
		    break;
		*/
		case AST_HEADER:
            if(node->symbol == 0){return;}
			node->dataType = node->symbol->datatype;
			node->line = node->son[0]->line;
			err = correctFunctionReturn(node);
			if (err == 1)
            {
				fprintf(stderr, "Linha %i: Retorno da função %s não corresponde seu tipo\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
		    break;

		case AST_IDENTIFIER:
            if(node->symbol == 0){return;}
			if ( node->symbol->type != SYMBOL_SCALAR && node->symbol->type != SYMBOL_LITINT &&
				 node->symbol->type != SYMBOL_LITFLOAT && node->symbol->type != SYMBOL_LITCHAR )
			{
				fprintf(stderr, "Linha %i: Identificador %s precisa ser um escalar \n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			if ( node->symbol->datatype ==  DATATYPE_INT || node->symbol->datatype ==  DATATYPE_BYTE )
			{
				node->dataType = DATATYPE_INT;
			}
			else
			{
				node->dataType = DATATYPE_FLOAT;
			}
			break;

		case AST_ARR_POS:
            if(node->symbol == 0){return;}
			if ( isBoolean(node->son[0]->dataType) || isFloat(node->son[0]->dataType))
			{
				fprintf(stderr, "Linha %i: Expressão no array %s precisa ser um inteiro.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			if (node->symbol->type != SYMBOL_ARRAY)
			{
				fprintf(stderr, "Linha %i: Identificador %s não é um array\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			if ( node->symbol->datatype ==  DATATYPE_INT || node->symbol->datatype ==  DATATYPE_BYTE )
			{
				node->dataType = DATATYPE_INT;
			}
			else
			{
				node->dataType = DATATYPE_FLOAT;
			}
			break;

		case AST_FUNC_CALL:
             if(node->symbol == 0){return;}  
			if(node->symbol->type != SYMBOL_FUNCTION)
			{
				fprintf(stderr, "Linha %i: %s não é uma função\n", node->line, node->symbol->text );
				SemanticErrorFlag = 1;
			}
			err = checkFunctionParams(node);
			if (err == 1)
			{
				fprintf(stderr, "Linha %i:  parâmetros da função %s não compatíveis\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->symbol->datatype;
			break;

		case AST_ADD:
			if( isBoolean(node->son[0]->dataType) || isBoolean(node->son[1]->dataType) )
			{
				fprintf(stderr, "Linha %i: Operando inválido em operação aritmética\n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->son[0]->dataType;
			if (node->son[0]->dataType != node->son[1]->dataType )
			{
				if (node->son[1]->dataType == DATATYPE_FLOAT)
				{
					node->dataType = node->son[1]->dataType;
				}
			}
			node->value = node->son[0]->value + node->son[1]->value;
			break;
		
		case AST_SUB:
			if( isBoolean(node->son[0]->dataType) || isBoolean(node->son[1]->dataType) )
			{
				fprintf(stderr, "Linha %i: Operando inválido em operação aritmética\n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->son[0]->dataType;
			if (node->son[0]->dataType != node->son[1]->dataType )
			{
				if (node->son[1]->dataType == DATATYPE_FLOAT)
				{
					node->dataType = node->son[1]->dataType;
				}
			}
			node->value = node->son[0]->value - node->son[1]->value;
			break;

		case AST_MUL:
			if( isBoolean(node->son[0]->dataType) || isBoolean(node->son[1]->dataType) )
			{
				fprintf(stderr, "Linha %i: Operando inválido em operação aritmética\n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->son[0]->dataType;
			if (node->son[0]->dataType != node->son[1]->dataType )
			{
				if (node->son[1]->dataType == DATATYPE_FLOAT)
				{
					node->dataType = node->son[1]->dataType;
				}
			}
			node->value = node->son[0]->value * node->son[1]->value;
			break;

		case AST_DIV:
			if( isBoolean(node->son[0]->dataType) || isBoolean(node->son[1]->dataType) )
			{
				fprintf(stderr, "Linha %i: Operando inválido em operação aritmética\n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->son[0]->dataType;
			if (node->son[0]->dataType != node->son[1]->dataType )
			{
				if (node->son[1]->dataType == DATATYPE_FLOAT)
				{
					node->dataType = node->son[1]->dataType;
				}
			}
			node->value = node->son[0]->value / node->son[1]->value;
			break;

		case AST_LESS:
		case AST_GREATER:
		case AST_LE:
		case AST_GE:
		case AST_EQ:
			if ( !isBoolean(node->son[0]->dataType) || !isBoolean(node->son[1]->dataType) )
			{
				fprintf(stderr, "Linha %i: expressão booleana inválida.\n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = AST_BOOL;
			break;
		
		case AST_AND:
		case AST_OR:
			if ( !isBoolean(node->son[0]->dataType) || !isBoolean(node->son[1]->dataType) )
			{
				fprintf(stderr, "Linha %i: expressão booleana inválida \n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = AST_BOOL;
			break;

		case AST_NOT:
			if ( !isBoolean(node->son[0]->dataType) )
			{
				fprintf(stderr, "Linha %i: uso inválido do NOT \n", node->line);
				SemanticErrorFlag = 1;
			}
			node->dataType = AST_BOOL;
			break;

		case AST_LEAP:
			if ( (node->son[0] != 0) ||  (node->son[1] != 0) || (node->son[2] != 0) || (node->son[3] != 0) )
			{
				fprintf(stderr, "Linha %i :uso inválido do Leap.\n", node->line);
				SemanticErrorFlag = 1;
			}
			break;

		case AST_ATTRIB:
             if(node->symbol == 0){return;}
			if( node->symbol->datatype != node->son[0]->dataType || node->symbol->type != SYMBOL_SCALAR)
			{
				fprintf(stderr, "Linha %i: Não foi possível fazer atribuição em  %s.\n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->symbol->datatype;
			break;

		case AST_ARR_ATTRIB:
            if(node->symbol == 0){return;}
			if ( !isInt(node->son[0]->dataType) )
			{
					fprintf(stderr, "Linha %i: tamanho do array %s precisa ser inteiro\n", node->line, node->symbol->text);
					SemanticErrorFlag = 1;
			}
			if (node->symbol->datatype != node->son[1]->dataType || node->symbol->type != SYMBOL_ARRAY)
			{
				fprintf(stderr, "Linha %i: Array %s precisa ter tipo compatível para atribuição \n", node->line, node->symbol->text);
				SemanticErrorFlag = 1;
			}
			node->dataType = node->symbol->datatype;
			break;

		case AST_READ:
            if(node->symbol == 0){return;}
			if (node->symbol->type != SYMBOL_SCALAR)
			{
				fprintf(stderr, "Linha %i: parâmetro de READ precisa ser escalar \n", node->line);
				SemanticErrorFlag = 1;
			}
			break;

		case AST_PRINT:
			err = checkPrintParams(node->son[0]);
			if (err == 1)
			{
				fprintf(stderr, "Linha %i: Parâmetros de print precisam ser strings \n", node->line);
				SemanticErrorFlag  = 1;
			}
			break;

		case AST_RETURN:
			if(node->son[0]->dataType == AST_BOOL)
			{
				fprintf(stderr, "Linha %i: Retorno da expressão não pode ser booleano \n", node->line);
				SemanticErrorFlag;
			}
			node->dataType = node->son[0]->dataType;
			break;

		case AST_IF:
		case AST_IF_ELSE:
		case AST_LOOP:
			node->line = node->son[1]->line;
			if( !isBoolean(node->son[0]->dataType) )
			{
				fprintf(stderr, "Linha %i: Controle de fluxo precisa ter retorno booleano \n", node->line);
				SemanticErrorFlag = 1;
			}
			break;

		default :
			break;
	}
}

/*
	Funcition: Valida de um tipo pode ser tratado como inteiro
	Parameters:
		int type: Identificador de tipo
	Return: Retorna 1 é equivalente a inteiro e 0 caso contrario
*/
int isInt(int type)
{
	switch(type)
    {
		case DATATYPE_INT:
            return 1;
		case DATATYPE_BYTE:
            return 1;
		case AST_BYTE:
            return 1;
		case AST_INT:
            return 1;
		case AST_LIT_INTEGER:
            return 1;
		case AST_LIT_CHAR:
            return 1;
		default:
            return 0;
	}
}

/*
	Funcition: Valida de um tipo pode ser tratado como float
	Parameters:
		int type: Identificador de tipo
	Return: Retorna 1 é equivalente a float e 0 caso contrario
*/
int isFloat(int type)
{
	switch(type)
    {
		case DATATYPE_FLOAT:
		case AST_FLOAT:
            return 1;
		case AST_LIT_FLOAT:
            return 1;
		default:
            return 0;
	}
}

/*
	Funcition: Valida de um tipo pode ser tratado como boleano
	Parameters:
		int type: Identificador de tipo
	Return: Retorna 1 é equivalente a boleano e 0 caso contrario
*/
int isBoolean(int type)
{
	switch(type)
    {
		case AST_BOOL:
            return 1;
		case AST_LESS:
            return 1;
		case AST_GREATER:
            return 1;
		case AST_AND:
            return 1;
		case AST_OR:
            return 1;
		case AST_NOT:
            return 1;
		case AST_LE:
            return 1;
		case AST_GE:
            return 1;
		case AST_EQ:
            return 1;
		default:
            return 0;
	}
}

/*
	Funcition: Checa parametros de função
	Parameters:
		AST* node: Ponteiro para nodo da ast a ser checado
	Return: Retorna 0 se correto e 1 se ocorreu erro
*/
int checkFunctionParams(AST* node)
{
	AST* rNode = rootNode;
	AST* def;
	if (node == 0)
	{
		return 1;
	}
	if (node->symbol->type != SYMBOL_FUNCTION)
	{
		return 1;
	}
	def = getFunctionDef(rNode, node);

    if(!def) return 1;

	if( node->son[0] == 0 && def->son[1] == 0 )
	{
		return 0;
	}
	if (!def && !def->son[1])
	{
		return 1;
	}
	def = def->son[1];
	if ( !node->son[0])
	{
		return 1;
	}
	node = node->son[0];
	if (def->type == AST_PARAM)
	{
		if(node->dataType == def->dataType)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return compareFunctionDef(def, node);
}

/*
	Funcition: Busca a definição da função na ast
	Parameters:
		AST* rNode: Ponteiro para o root node da ast
		AST* node: Ponteiro para o nodo a ser analisado
	Return: Retorna o ponteiro para o nodo da ast se achar. Caso contrario retorna 0
*/
AST* getFunctionDef(AST* rNode, AST* node)
{
	if (rNode == 0)
	{
		return 0;
	}
    if(node == 0) return 0;
    if(node->symbol == 0) return 0;
	switch ( rNode->type )
	{
		case AST_DEC_VAR_LIST:
			if( strcmp(rNode->son[0]->symbol->text, node->symbol->text) == 0 )
			{
				return rNode->son[0];
			}
			return getFunctionDef(rNode->son[1], node);
		break;
		case AST_DEC_FUNC_LIST:
             if(rNode->son[1] == 0) return;
             if(rNode->son[1]->symbol == 0) return;
			if( strcmp(rNode->son[1]->symbol->text, node->symbol->text) == 0 )
			{
				return rNode->son[1];
			}
		break;
		default:
		break;
	}
	return getFunctionDef(rNode->son[0], node);
}

/*
	Funcition: Compara definição de função com utilização
	Parameters: 
		AST* def: Ponteiro para nodo de definição na ast
		AST* node: Ponteiro para nodo de utilização na ast
	Return: Retorna 1 se uso e declarações são equivalentes e 0 se não
*/
int compareFunctionDef(AST* def, AST* node)
{
	if ( def == 0 )
	{
		return 1;
	}
	if ( node == 0 )
	{
		return 1;
	}
	if (def->son[1] == 0)
	{
		return 1;
	}
	if (node->son[1]== 0)
	{
		return 1;
	}
	if( def->son[1]->dataType != node->son[1]->dataType )
	{
		return 1;
	}
	switch (def->son[0]->type)
	{
		case AST_PARAMS:
			return compareFunctionDef(def->son[0], node->son[0]);
		break;
		case AST_PARAM:
			if( def->son[0]->dataType != node->son[0]->dataType )return 1;

		break;

	}
}


/*
	Funcition: Checa parametros do comando print
	Parameters: 
		AST* node: Ponteiro da ast a ser analisado
	Return: Retorna 1 se correto, senão 0
*/
int checkPrintParams(AST* node)
{
	if( node == 0)
	{
		return 1;
	}
	if( isExpr(node) || node->type == AST_LIT_STRING)
	{
		return 0;
	}
	if( node->son[0] == 0)
	{
		return 1;
	}
	if(node->son[0]->type == AST_PRINT_ELEM)
	{
		if( node->son[1]->type != AST_LIT_STRING && !isExpr(node->son[1]))
		{
			return 1;
		}
		return checkPrintParams(node->son[0]);
	}
}

/*
	Funcition: Checa se o nodo é uma expressão
	Parameters:
		AST* node: Ponteiro da ast a ser analisado
	Return: Retorna 1 se correto, senão 0
*/
int isExpr(AST* node)
{
	if (node == 0)
	{
		return 0;
	}
	switch (node->type)
	{
		case AST_IDENTIFIER:
		case AST_LIT_INTEGER:
		case AST_LIT_CHAR:
		case AST_LIT_FLOAT:
		case AST_ARR_POS:
		case AST_ADD:
		case AST_SUB:
		case AST_MUL:
		case AST_DIV:
		case AST_LESS:
		case AST_GREATER:
		case AST_AND:
		case AST_OR:
		case AST_NOT:
		case AST_LE:
		case AST_GE:
		case AST_EQ:
		case AST_FUNC_CALL:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

/*
	Funcition: Realiza a conversão de codigos literias para inteiro conforme definição do trabalho
	Parameters:
		char* stringIn: String a ser convertida
	Return: Retorna string equivalente
*/
char* convertValueToNumberString(char* stringIn)
{
    char* stringToAppend;
    char* stringOut;
    stringOut = (char*)calloc(strlen(stringIn), sizeof(char));
    for(int i = 0 ; i <= strlen(stringIn)-1 ; i++)
    {
        switch (stringIn[i])
        {
        case 'O':
            stringToAppend = "0";
            break;
        case 'I':
            stringToAppend = "1";
            break;
        case 'H':
            stringToAppend = "2";
            break;
        case 'G':
            stringToAppend = "3";
            break;
        case 'F':
            stringToAppend = "4";
            break;
        case 'E':
            stringToAppend = "5";
            break;
        case 'D':
            stringToAppend = "4";
            break;
        case 'C':
            stringToAppend = "3";
            break;
        case 'B':
            stringToAppend = "2";
            break;
        case 'A':
            stringToAppend = "1";
            break;
        case '.':
            stringToAppend = ".";
            break;
        default:
            break;
        }
        if (stringOut != NULL && stringToAppend != NULL)
        {
            strcat(stringOut, stringToAppend);
        }
    }
    return stringOut;
}

/*
	Funcition: Valida tamanho e inicialização do array
	Parameters:
		AST* param: Ponteiro para nodo a ast de parametro
		int* sizeArray: Tamanho do array
		int arrayType: Identificador de tipo de array
*/
int sizeAndInitialisationArray(AST* param, int* sizeArray, int arrayType)
{
	if(!param)
    {
        return 1;
    }
	if( param->type == AST_ARRAY_VALUE)
    {
		if(param->son[1]->dataType != arrayType)
        {
			return 1;
        }
		*sizeArray = *sizeArray - 1;
		return sizeAndInitialisationArray(param->son[0], sizeArray, arrayType);
	}
	if(param->dataType != arrayType)
    {
        return 1;
    }
	*sizeArray = *sizeArray - 1;
	return 0;

}

/*
	Funcition: Valida array
	Parameters:
		AST* param: Ponteiro para nodo a ast a ser analisado
	Return: 0 se ok, positivo(há mais elementos que declarados) ou negativo(há menos elementos que declarados) se erro
*/
int checkArray(AST* node)
{
		int size = (int)node->son[1]->value;
		AST* params = node->son[2];
		int ok = 0;
		ok = sizeAndInitialisationArray(params, &size, node->dataType);
		if (ok)
		{
			return 1;
		}
		else
		{
			if (size > 0)
        	{
            	return 2;
        	}
			if (size < 0)
        	{
        		return 3;
        	}
			return ok;
		}
}

/*
	Funcition: Verifica se a função possui um retorno de valor condizente com o declarado
	Parameters:
		AST* node: Ponteiro para nodo a ast a ser analisado 
	Return:  0 se ok senão 1
*/
int correctFunctionReturn(AST* node)
{
	if (node->son[2]==0)
	{
		return 0;
	}
	if(node->son[2]->son[0] == 0)
	{
		return 0;
	}
	AST* first = node->son[2]->son[0];
	return (getFunctionReturn(first,node->son[0]->dataType));
}

/*
	Funcition: Verifica se a função possui um retorno de valor condizente com o declarado
	Parameters:
		AST* node: Ponteiro para nodo a ast a ser analisado 
		int funcDataType: Identificador de tipo de dado declarado como retorno da função
	Return:  0 se ok senão 1
*/
int getFunctionReturn(AST* node, int funcDataType)
{
	int r, l;
	if ( node == 0 )
	{
		return 0;
	}
	if (node->type == AST_RETURN)
	{
		if( node->dataType == funcDataType)
		{
			return 0;
		}
		return 1;
	}
	if (node->son[1])
	{
		r = getFunctionReturn(node->son[1], funcDataType);
	}
	if (node->son[0])
	{
		l= getFunctionReturn(node->son[0], funcDataType);
	}
	if ( r == 1 || l ==1 )
	{
		return 1;
	}
	return 0;
}


