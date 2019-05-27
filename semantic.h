#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "hash.h"

#define VAR_FLOAT	600
#define VAR_INT 	601
#define VAR_BYTE	602


int setAndCheckRedeclared(AST *node);
