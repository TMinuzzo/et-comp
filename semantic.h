
#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "ast.h"

void setDeclaration(AST* node);
void checkUndeclared();
void checkOperands(AST* node);

int isInt(int type);
int isFloat(int type);
int isBoolean(int type);
int isExpr(AST* node);

int checkFunctionParams(AST* node);
int checkPrintParams(AST* node);
AST* getFunctionDef(AST* rootNode, AST* node);
int compareFunctionDef(AST* def, AST* node);

int correctFunctionReturn(AST* node);
int getFunctionReturn(AST* node, int funcDataType);


int checkArray(AST* node);
int sizeAndInitialisationArray(AST* param, int* sizeArray, int arrayType);
char* convertValueToNumberString(char* stringIn); 


#endif
