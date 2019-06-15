#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern FILE *out;

AST *astCreate(int type, NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3, int line)
{
    AST *newnode;
    newnode = (AST *)calloc(1, sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;
    newnode->line = line;
    return newnode;
}

void astPrint(AST *node, int level)
{
    int i;
    if (node == 0)
    {
        return;
    }
    for (i = 0; i < level; ++i)
    {
        fprintf(stderr, "  ");
    }
    switch (node->type)
    {
    case AST_DEC_FUNC_LIST:
        fprintf(stderr, "AST_DEC_FUNC_LIST\n");
        break;
    case AST_DEC_VAR_LIST:
        fprintf(stderr, "AST_DEC_VAR_LIST\n");
        break;
    case AST_ARRAY:
        fprintf(stderr, "AST_ARRAY %s\n", node->symbol->text);
        break;
    case AST_ARRAY_INIT:
        fprintf(stderr, "AST_ARRAY_INIT\n");
        break;
    case AST_ARRAY_VALUE:
        fprintf(stderr, "AST_ARRAY_VALUE\n");
        break;
    case AST_GLOBAL_DEC:
        fprintf(stderr, "AST_GLOBAL_DEC %s\n", node->symbol->text);
        break;
    case AST_GLOBAL_DEC_INIT:
        fprintf(stderr, "AST_GLOBAL_DEC_INIT %s\n", node->symbol->text);
        break;
    case AST_FUNCTION:
        fprintf(stderr, "AST_FUNCTION\n");
        break;
    case AST_HEADER:
        fprintf(stderr, "AST_HEADER%s\n", node->symbol->text);
        break;

    case AST_PARAMS:
        fprintf(stderr, "AST_PARAMS\n");
        break;
    case AST_PARAM:
        fprintf(stderr, "AST_PARAM %s\n", node->symbol->text);
        break;
    case AST_CMDS:
        fprintf(stderr, "AST_CMDS\n");
        break;
    case AST_RETURN:
        fprintf(stderr, "AST_RETURN\n");
        break;
    case AST_READ:
        fprintf(stderr, "AST_READ\n");
        break;
    case AST_PRINT:
        fprintf(stderr, "AST_PRINT\n");
        break;
    case AST_ADD:
        fprintf(stderr, "AST_ADD\n");
        break;
    case AST_SUB:
        fprintf(stderr, "AST_SUB\n");
        break;
    case AST_MUL:
        fprintf(stderr, "AST_MUL\n");
        break;
    case AST_DIV:
        fprintf(stderr, "AST_DIV\n");
        break;
    case AST_EXPRESSION:
        fprintf(stderr, "AST_EXPRESSION\n");
        break;
    case AST_GE:
        fprintf(stderr, "AST_GE\n");
        break;
    case AST_DIF:
        fprintf(stderr, "AST_DIF\n");
        break;
    case AST_GREATER:
        fprintf(stderr, "AST_GREATER\n");
        break;
    case AST_LESS:
        fprintf(stderr, "AST_LESS\n");
        break;
    case AST_OR:
        fprintf(stderr, "AST_OR\n");
        break;
    case AST_AND:
        fprintf(stderr, "AST_AND\n");
        break;
    case AST_EQ:
        fprintf(stderr, "AST_EQ\n");
        break;
    case AST_NOT:
        fprintf(stderr, "AST_NOT\n");
        break;
    case AST_ARR_POS:
        fprintf(stderr, "AST_ARR_POS\n");
        break;
    case AST_FUNC_CALL:
        fprintf(stderr, "AST_FUNC_CALL\n");
        break;
    case AST_LIT_INTEGER:
        fprintf(stderr, "AST_LIT_INTEGER %s\n", node->symbol->text);
        break;
    case AST_LIT_FLOAT:
        fprintf(stderr, "AST_LIT_FLOAT %s\n", node->symbol->text);
        break;
    case AST_LIT_CHAR:
        fprintf(stderr, "AST_LIT_CHAR %s\n", node->symbol->text);
        break;
    case AST_LIT_STRING:
        fprintf(stderr, "AST_LIT_STRING %s\n", node->symbol->text);
        break;
    case AST_ATTRIB:
        fprintf(stderr, "AST_ATTRIB\n");
        break;
    case AST_ARR_ATTRIB:
        fprintf(stderr, "AST_ARR_ATTRIB\n");
        break;
    case AST_PRINT_ELEM:
        fprintf(stderr, "AST_PRINT_ELEM\n");
        break;
    case AST_IDENTIFIER:
        fprintf(stderr, "AST_IDENTIFIER %s\n", node->symbol->text);
        break;
    case AST_LITERAL:
        fprintf(stderr, "AST_LITERAL\n");
        break;
    case AST_IF:
        fprintf(stderr, "AST_IF\n");
        break;
    case AST_ELSE:
        fprintf(stderr, "AST_ELSE\n");
        break;
    case AST_LOOP:
        fprintf(stderr, "AST_LOOP\n");
        break;
    case AST_LEAP:
        fprintf(stderr, "AST_LEAP\n");
        break;
    case AST_INT:
        fprintf(stderr, "AST_INT\n");
        break;
    case AST_BYTE:
        fprintf(stderr, "AST_BYTE\n");
        break;
    case AST_FLOAT:
        fprintf(stderr, "AST_FLOAT\n");
        break;
    case AST_CMD:
        fprintf(stderr, "AST_CMD\n");
        break;
    case AST_BLOCK:
        fprintf(stderr, "AST_BLOCK\n");
        break;
    case AST_IF_ELSE:
        fprintf(stderr, "AST_IF_ELSE\n");
        break;
    case AST_ARGS:
        fprintf(stderr, "AST_ARGS\n");
        break;

    default:
        fprintf(stderr, "AST_UNKNOWN %d, %s\n", node->type, node->symbol->text);
        break;
    }
    for (i = 0; i < MAX_SONS; ++i)
    {
        astPrint(node->son[i], level + 1);
    }
}

void compile(AST *node, FILE *out)
{

    AST *temp = node;
    if (temp == 0)
    {
        return;
    }
    switch (temp->type)
    {
    case AST_DEC_FUNC_LIST:
        compile(temp->son[0], out);
        fprintf(out, ";\n");
        compile(temp->son[1], out);
        break;
    case AST_DEC_VAR_LIST:
        compile(temp->son[0], out);
        fprintf(out, ";\n");
        compile(temp->son[1], out);
        break;
    case AST_GLOBAL_DEC_INIT:
        compile(temp->son[0], out);
        fprintf(out, "%s ", temp->symbol->text);
        fprintf(out, "=");
        compile(temp->son[1], out);
        break;
    case AST_BYTE:
        fprintf(out, "byte ");
        break;
    case AST_INT:
        fprintf(out, "int ");
        break;
    case AST_FLOAT:
        fprintf(out, "float ");
        break;
    case AST_LIT_INTEGER:
        fprintf(out, " %s", temp->symbol->text);
        break;
    case AST_LIT_FLOAT:
        fprintf(out, " %s", temp->symbol->text);
        break;
    case AST_LIT_CHAR:
        fprintf(out, " %s", temp->symbol->text);
        break;
    case AST_LIT_STRING:
        fprintf(out, " %s", temp->symbol->text);
        break;
    case AST_GLOBAL_DEC:
        compile(temp->son[0], out);
        fprintf(out, "%s", temp->symbol->text);
        compile(temp->son[1], out);
        break;
    case AST_ARRAY:
        fprintf(out, "[%s]", temp->symbol->text);
        if (temp->son[1])
        {
            fprintf(out, ":");
            compile(temp->son[1], out);
        }
        fprintf(out, ";\n");
        break;
    case AST_ARRAY_VALUE:
        compile(temp->son[0], out);
        compile(temp->son[1], out);
        break;
    case AST_FUNCTION:
        compile(temp->son[0], out);
        compile(temp->son[1], out);
        break;
    case AST_HEADER:
        compile(temp->son[0], out);
        fprintf(out, "%s", temp->symbol->text);
        fprintf(out, "(");
        compile(temp->son[1], out);
        fprintf(out, ")");
        break;
    case AST_PARAMS:
        compile(temp->son[0], out);
        if (temp->son[1])
        {
            fprintf(out, ", ");
            compile(temp->son[1], out);
        }
        break;
    case AST_PARAM:
        compile(temp->son[0], out);
        fprintf(out, "%s", temp->symbol->text);
        break;
    case AST_BLOCK:
        fprintf(out, "{\n");
        compile(temp->son[0], out);
        fprintf(out, "\n}");
        break;
    case AST_CMDS:
        compile(temp->son[0], out);
        if (temp->son[1])
        {
            fprintf(out, ";\n");
            compile(temp->son[1], out);
        }
        break;
    case AST_RETURN:
        fprintf(out, "return ");
        compile(temp->son[0], out);
        break;
    case AST_EQ:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "==");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_ADD:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "+");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_SUB:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "-");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_DIV:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "/");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_MUL:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "*");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_AND:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, " and ");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_OR:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, " or ");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_GE:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, ">=");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_LE:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "<=");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_DIF:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "!=");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_GREATER:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, ">");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_LESS:
        fprintf(out, "( ");
        compile(temp->son[0], out);
        fprintf(out, "<");
        compile(temp->son[1], out);
        fprintf(out, ") ");
        break;
    case AST_NOT:
        fprintf(out, "not ");
        compile(temp->son[0], out);
        break;
    case AST_IDENTIFIER:
        fprintf(out, "%s", node->symbol->text);
        break;
    case AST_ATTRIB:
        fprintf(out, "%s = ", node->symbol->text);
        compile(temp->son[0], out);
        break;
    case AST_ARR_ATTRIB:
        compile(temp->son[0], out);
        fprintf(out, " = ");
        compile(temp->son[1], out);
        break;
    case AST_ARR_POS:
        fprintf(out, "%s", node->symbol->text);
        fprintf(out, "[");
        compile(temp->son[0], out);
        fprintf(out, "]");
        break;
    case AST_PRINT:
        fprintf(out, "print ");
        compile(temp->son[0], out);
        break;
    case AST_PRINT_ELEM:
        compile(temp->son[0], out);
        if (temp->son[1])
        {
            fprintf(out, ", ");
            compile(temp->son[1], out);
        }
        break;
    case AST_READ:
        fprintf(out, "read ");
        fprintf(out, "%s", node->symbol->text);
        break;
    case AST_LEAP:
        fprintf(out, "leap");
        break;
    case AST_LOOP:
        fprintf(out, "loop (");
        compile(temp->son[0], out);
        fprintf(out, ") \n");
        compile(temp->son[1], out);
        break;
    case AST_FUNC_CALL:
        fprintf(out, "%s (", node->symbol->text);
        compile(temp->son[0], out);
        fprintf(out, ") ");
        break;
    case AST_ARGS:
        compile(temp->son[0], out);
        if (temp->son[1])
        {
            fprintf(out, ", ");
            compile(temp->son[1], out);
        }
        break;
    case AST_IF:
        fprintf(out, "if (");
        compile(temp->son[0], out);
        fprintf(out, ") then\n");
        compile(temp->son[1], out);
        break;
    case AST_IF_ELSE:
        fprintf(out, "if (");
        compile(temp->son[0], out);
        fprintf(out, ") then \n");
        compile(temp->son[1], out);
        fprintf(out, "else \n");
        compile(temp->son[2], out);
        break;
    default:
        //fprintf(stderr, "AST_UNKNOWN\n");
        break;
    }
}
