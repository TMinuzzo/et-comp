
#ifndef TACS_H
#define TACS_H

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_SUB 2
#define TAC_ADD 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_ASSIGN 6
#define TAC_IFZ 7
#define TAC_LABEL 8

#define TAC_LESS 9
#define TAC_GREATER 10
#define TAC_AND 11
#define TAC_OR  12
#define TAC_NOT 13
#define TAC_LE 14
#define TAC_GE 15
#define TAC_EQ 16
#define TAC_ARR 17
#define TAC_CALL 18
#define TAC_STRING 19
#define TAC_READ 20
#define TAC_PRINT 21

#define TAC_SYMBOL_LIT 22
#define TAC_RET 23
#define TAC_PARAM 24

#define TAC_LEAP 25
#define TAC_JUMP 26
#define TAC_SYMBOL_VEC 27
#define TAC_IFZ_ELSE 28
#define TAC_SYMBOL_KW 29
#define TAC_BEGINFUN 30
#define TAC_ENDFUN 31
#define TAC_VEC_MOV 32
#define TAC_PRINT_PARAM 33
#define TAC_FUNC_PARAM 34
#define TAC_CALL_PARAM 35
#define TAC_LOOP 36
#define TAC_LABEL_LEAP 37

#define TAC_DEC_VAR 38
#define TAC_DEC_VEC 39

typedef struct tac{
  int type;
  int argsCount;
  int num;
  NODE *result;
  NODE *firstOp;
  NODE *secondOp;
  struct tac *prev;
  struct tac *next;
}TAC;

TAC* tacGenerate(AST *node,NODE *jmpLeap);
TAC* tacCreate(int type, NODE *result, NODE *firstOp, NODE *secondOp, int num);
void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC* tacJoin(TAC *t1, TAC *t2);

TAC* tacReverse(TAC *tac);
void tacPrintForward(TAC *tac);
int sameType(AST *tempnode, TAC *aux);
TAC* funcDeclaration(AST *node, TAC* params, TAC *block);
TAC* makeCallFunction(AST* node, TAC* listParam);
int sameType(AST *tempnode, TAC *aux);
TAC *makeBinOp(int op, TAC *result0, TAC *result1);
TAC* makeLeap(NODE *jmpLeap);
TAC* makeLoop(TAC *expr, TAC *cmd, NODE *jmpLeapLoop);
TAC* makeIf(TAC *result0, TAC *result1);
TAC *makeIfElse(TAC *expr, TAC *cmdIf, TAC *cmdElse);

#endif
