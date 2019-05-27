#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "hash.h"

#define VAR_FLOAT	600
#define VAR_INT 	601
#define VAR_BYTE	602
#define LIT_STR		603
#define LIT_INTEG	604
#define LIT_FLOATING	605
#define LIT_BYTE	606
#define VAR_UNDECLARED	607
#define VAR_BOOLEAN	608


int setAndCheckRedeclared(AST *node);
int checkUndeclared(AST *node);
int checkFunctions(AST *node);
int checkAritExpressions(AST *node);
// int checkVectorIndex
int checkStrings();
// int checkAttribs
// int checkReturns
// int checkIfBooleans
// replace find hash node
