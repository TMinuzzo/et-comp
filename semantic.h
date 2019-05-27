#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "hash.h"

#define VAR_FLOAT	600
#define VAR_INT 	601
#define VAR_BYTE	602
#define LIT_STR		603
#define LIT_INTEGER	604
#define LIT_FLOATING	605
#define LIT_BYTE	606


int setAndCheckRedeclared(AST *node);
int checkUndeclared(AST *node);
