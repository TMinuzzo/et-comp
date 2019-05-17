#include "semantic.h"
#include "ast.h"


void setAndCheckRedeclared(AST *node)
{
    int i;
    if(node == 0)
        return;
    
    for(i = 0; i < MAX_SONS; ++i )
    {
        setAndCheckRedeclared(node->son[i]);
    }

    switch(node->type)
    {
        case AST_GLOBAL_DEC:
        break;
            
    }
}