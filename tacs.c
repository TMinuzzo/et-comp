#include "tacs.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *out;

TAC* tacGenerate(AST *node, NODE *jmpLeapLoop){

  int i;
  TAC *result[MAX_SONS];
 
  if (!node) { return 0; }
  
 if(node->type == AST_LOOP){
	jmpLeapLoop = makeLabel();
 }
  
  for (i = 0; i < MAX_SONS; i++)
  {
    result[i] = tacGenerate(node->son[i], jmpLeapLoop);
   
  }
  switch (node->type) 
  {

      //lista de declarações
      case AST_DEC_FUNC_LIST:
      case AST_DEC_VAR_LIST:
        return tacJoin(result[1], result[0]);
        break;
      case AST_GLOBAL_DEC_INIT:
        return tacJoin(tacCreate(TAC_SYMBOL, node->symbol, 0, 0, 0), result[1]);
        break;
    // Inicialização de vetor
      case AST_GLOBAL_DEC:
        return tacCreate(TAC_SYMBOL_VEC, node->symbol, result[1]?result[1]->result : 0 , 0, 0);
        break;
     //cabeçalho de função
      case AST_HEADER:
        return funcDeclaration(node, result[1], result[2]);
        break;
    //TK_IDENTIFIER
      case AST_IDENTIFIER:
        return tacCreate(TAC_SYMBOL, node->symbol, 0, 0, 0);
        break;
    // atribuição de vetor
      case AST_ARR_POS:
        return tacJoin(result[0], tacCreate(TAC_ARR, makeTemp(), node->symbol, result[0]?result[0]->result : 0, 0));
        break;

      case AST_LIT_INTEGER:
      case AST_LIT_FLOAT:
      case AST_LIT_CHAR:
        return tacCreate(TAC_SYMBOL_LIT, node->symbol, 0, 0, 0);
        break;
      case AST_LIT_STRING:
        return tacCreate(TAC_PRINT_PARAM, node->symbol, 0, 0, 0);
        break;
    // lista de parâmetros passados para funções
      case AST_PARAMS:
        return tacJoin(result[0], result[1]);
        break;
    // cada parâmetro passado para funções
      case AST_PARAM:
        return tacCreate(TAC_PARAM, node->symbol, 0, 0, 0);
        break;
    // a chamada de função
      case AST_FUNC_CALL:
        return callFunction(node, result[0]);
        break;
    // os argumentos da chamada de função
      case AST_ARGS:
        return tacJoin(result[0], result[1]);
        break;
      case AST_SUB:
        return makeBinOp(TAC_SUB, result[0], result[1]);
        break;
      case AST_ADD:
        return makeBinOp(TAC_ADD, result[0], result[1]);
        break;
      case AST_MUL:
        return makeBinOp(TAC_MUL, result[0], result[1]);
        break;
      case AST_DIV:
        return makeBinOp(TAC_DIV, result[0], result[1]);
        break;
      case AST_LESS:
        return makeBinOp(TAC_LESS, result[0], result[1]);
        break;
      case AST_GREATER:
        return makeBinOp(TAC_GREATER, result[0], result[1]);
        break;
      case AST_EQ:
        return makeBinOp(TAC_EQ, result[0], result[1]);
        break;
      case AST_OR:
        return makeBinOp(TAC_OR, result[0], result[1]);
        break;
      case AST_AND:
        return makeBinOp(TAC_AND, result[0], result[1]);
        break;
      case AST_NOT:
        return makeBinOp(TAC_NOT, result[0], result[1]);
        break;
      case AST_LE:
        return makeBinOp(TAC_LE, result[0], result[1]);
        break;
      case AST_GE:
        return makeBinOp(TAC_GE, result[0], result[1]);
        break;
      case AST_CMDS:
        return tacJoin(result[0], result[1]);
        break;
      case AST_ATTRIB:
        return tacJoin(result[0],tacCreate(TAC_ASSIGN, node->symbol, result[0]?result[0]->result : 0, 0, 0));
        break;
      case AST_ARR_ATTRIB:
        return tacJoin(tacJoin(result[0], result[1]), tacCreate(TAC_VEC_MOV, node->symbol, result[1]?result[1]->result:0, result[0]?result[0]->result:0, 0));
        break;
    case AST_RETURN:
        return tacJoin(result[0], tacCreate(TAC_RET, result[0]?result[0]->result:0, 0, 0, 0) );
        break;
      case AST_IF:
        return makeIf(result[0], result[1]);
        break;
      case AST_IF_ELSE:
        return makeIfElse(result[0], result[1], result[2]);
        break;
      case AST_READ:
        return tacCreate(TAC_READ, node->symbol, 0, 0, 0);
        break;
    case AST_PRINT:
        return tacJoin(tacCreate(TAC_PRINT,0, 0,0, (int)node->value), result[0]);
        break;
      case AST_PRINT_ELEM:
        return tacJoin(result[0],result[1]);
        break;
      case AST_BLOCK:
        return result[0];
        break;
      case AST_LEAP:
       return makeLeap(jmpLeapLoop);
       break; 
      case AST_LOOP:
        return makeLoop(result[0], result[1],jmpLeapLoop);
        break;
      default : 
        return tacJoin( tacJoin( tacJoin(result[0], result[1]), result[2]), result[3]); 
        break;
    }
    return 0;
}

TAC* funcDeclaration(AST *node, TAC *params, TAC *block)
{
  if(!node) return 0;
  NODE *fBegin = makeLabel();
  NODE *fEnd = makeLabel();

  TAC *funBegin = tacCreate(TAC_BEGIN_FUN, node->symbol, 0 , 0, (int)node->value);
  TAC *funEnd = tacCreate(TAC_END_FUN, node->symbol,0,0, 0);

  return tacJoin(tacJoin( tacJoin(funBegin,params), block), funEnd);
}

TAC* tacCreate(int type, NODE *result, NODE *firstOp, NODE *secondOp, int num)
{
  TAC *newTac;
  newTac = (TAC*)calloc(1, sizeof(TAC));
  newTac->type = type;
  newTac->num = num;
  newTac->result = result;
  newTac->prev = 0;
  newTac->next = 0;
  newTac->firstOp = firstOp;
  newTac->secondOp = secondOp;
  return newTac;
}

void tacPrintSingle(TAC *tac)
{
  if(!tac)
  {
    return;
  }
  switch (tac->type)
  {
    case TAC_SYMBOL: 
		    return;
    case TAC_SYMBOL_LIT: 
			  return;
    case TAC_ADD: 
        fprintf(stderr, "\nTAC_ADD ");
        break;
    case TAC_SUB:
        fprintf(stderr, "\nTAC_SUB ");
        break;
    case TAC_MUL:
        fprintf(stderr, "\nTAC_MULT ");
        break;
    case TAC_DIV:
        fprintf(stderr, "\nTAC_DIV ");
        break;
    case TAC_LESS:
        fprintf(stderr, "\nTAC_LESS ");
        break;
    case TAC_GREATER:
        fprintf(stderr, "\nTAC_GREATER ");
        break;
    case TAC_AND:
        fprintf(stderr, "\nTAC_AND ");
        break;
    case TAC_OR:    
        fprintf(stderr, "\nTAC_OR ");
        break;
    case TAC_NOT:
        fprintf(stderr, "\nTAC_NOT ");
        break;
    case TAC_LE:
        fprintf(stderr, "\nTAC_LE ");
        break;
    case TAC_GE:
        fprintf(stderr, "\nTAC_GE ");
        break;
    case TAC_EQ:
        fprintf(stderr, "\nTAC_EQ ");
        break;
    case TAC_ARR:
        fprintf(stderr, "\nTAC_ARRAY ");
        break;
    case TAC_SYMBOL_VEC :
			  return;
    case TAC_SYMBOL_KW :
        fprintf(stderr, "\nTAC_SYMBOL_KW ");
        break;
    case TAC_IFZ:
        fprintf(stderr, "\nTAC_IFZ ");
        break;
    case TAC_IFZ_ELSE:
        fprintf(stderr, "\nTAC_IFZ_ELSE ");
        break;
    case TAC_LEAP:
        return;
	  case TAC_LOOP:
        fprintf(stderr, "\nTAC_LOOP ");
        break;
    case TAC_ASSIGN :
        fprintf(stderr, "\nTAC_ATTR ");
        break;
    case TAC_READ :
        fprintf(stderr, "\nTAC_READ ");
        break;
    case TAC_RET :
        fprintf(stderr, "\nTAC_RET ");
        break;
    case TAC_PRINT :
        fprintf(stderr, "\nTAC_PRINT ");
        break;
    case TAC_PRINT_PARAM:
        fprintf(stderr, "\nTAC_PRINT_PARAM ");
        break;
    case TAC_PARAM:
        fprintf(stderr, "\nTAC_PARAM ");
        break;
    case TAC_CALL_PARAM:
        fprintf(stderr, "\nTAC_CALL_PARAM ");
        break;
    case TAC_BEGIN_FUN:
        fprintf(stderr, "\nTAC_FUNC_BEGIN ");
        break;
    case TAC_END_FUN:
        fprintf(stderr, "\nTAC_FUNC_END ");
        break;
    case TAC_LABEL :
        fprintf(stderr, "\nTAC_LABEL ");
        break;
    case TAC_JUMP:
        fprintf(stderr, "\nTAC_JUMP " );
        break;
    case TAC_CALL:
        fprintf(stderr, "\nTAC_CALL " );
        break;	
	  case TAC_VEC_MOV:
        fprintf(stderr, "\nTAC_VEC_MOV " );
        break;
    default:
        fprintf(stderr, "\nTAC_UNKNOWN ");
        break;
  }
  if (tac->result)
  {
    fprintf(stderr, "%s ", tac->result->text);
  }
  if (tac->firstOp)
  {
    fprintf(stderr, "%s ", tac->firstOp->text);
  }
  if (tac->secondOp)
  {
    fprintf(stderr, "%s ", tac->secondOp->text);
  }
  if(tac->num != 0)
  {
    fprintf(stderr, "%i ", (int )tac->num);
  }
}

void tacPrintBackwards(TAC *tac)
{
  if (!tac)
  {
    return;
  }
  tacPrintSingle(tac);
  tacPrintBackwards(tac->prev);
}


TAC* tacJoin(TAC *t1, TAC *t2)
{
  TAC *aux = t2;
  if (!t1)
  {
      return t2;
  }
  if (!t2)
  {
    return t1;
  }
  while(aux->prev)
  {
    aux = aux->prev;
  }
  aux->prev = t1;
  return t2;
}

TAC* tacInvert(TAC *tac)
{
  if(!tac)
  {
    return 0;
  }
  for(; tac->prev; tac = tac->prev)
  {
    tac->prev->next = tac;
  }
  return tac;
}


void tacPrintForward(TAC *tac)
{
  if (!tac)
  {
    return;
  }
  tacPrintSingle(tac);
  tacPrintForward(tac->next);
}

int sameType(AST *tempnode, TAC *aux)
{
    if(!tempnode)
    {
        return 0;
    }
    if(!aux)
    {
        return 0;
    }
    fprintf(stderr, "%i ", tempnode->type);
    fprintf(stderr, "%i\n", aux->type);
    if(tempnode->type == AST_LIT_INTEGER && aux->type == TAC_SYMBOL_LIT)
    {
      return 1;
    }
    if(tempnode->type == AST_LIT_CHAR && aux->type == TAC_SYMBOL_LIT)
    {
      return 1;
    }
    if(tempnode->type == AST_LIT_FLOAT && aux->type == TAC_SYMBOL_LIT)
    {
      return 1;
    }
    if(tempnode->type == AST_IDENTIFIER && aux->type == TAC_SYMBOL)
    {
      return 1;
    }
    if(tempnode->type == AST_ADD && aux->type == TAC_ADD)
    {
      return 1;
    }
    if(tempnode->type == AST_SUB && aux->type == TAC_SUB)
    {
      return 1;
    }
    if(tempnode->type == AST_MUL && aux->type == TAC_MUL)
    {
      return 1;
    }
    if(tempnode->type == AST_DIV && aux->type == TAC_DIV)
    {
      return 1;
    }
    if(tempnode->type == AST_LESS && aux->type == TAC_LESS)
    {
      return 1;
    }
    if(tempnode->type == AST_GREATER && aux->type == TAC_GREATER)
    {
      return 1;
    }
    if(tempnode->type == AST_AND && aux->type == TAC_AND)
    {
      return 1;
    }
    if(tempnode->type == AST_OR && aux->type == TAC_OR)
    {
      return 1;
    }
    if(tempnode->type == AST_NOT && aux->type == TAC_NOT)
    {
      return 1;
    }
    if(tempnode->type == AST_LE && aux->type == TAC_LE)
    {
      return 1;
    }
    if(tempnode->type == AST_GE && aux->type == TAC_GE)
    {
      return 1;
    }
    if(tempnode->type == AST_EQ && aux->type == TAC_EQ)
    {
      return 1;
    }
    if(tempnode->type == AST_FUNC_CALL && aux->type == TAC_CALL)
    {
      return 1;
    }
    return 0;
}

TAC* callFunction(AST* node, TAC* listParam)
{
  int numParams = (int)node->value;
  int i = 0;
  AST *tempNode = node;
  AST *left = tempNode->son[0] ? tempNode->son[0]->son[1] : 0;
  AST *right = tempNode->son[0] ?tempNode->son[0]->son[0] : 0;
  TAC *aux = listParam;
  TAC *p  = aux;
  int done = 0;

  if(node->value == 0)
  {
    return tacCreate(TAC_CALL, node->symbol, 0,0,0);
  }
  else if(node->value == 1)
  {
    return tacJoin(tacJoin(p, tacCreate(TAC_CALL_PARAM, p->result, 0, 0, 0)),tacCreate(TAC_CALL, node->symbol, 0,0,1));
  }
  do
  {
    if (sameType(left, aux))
    {
      p = tacJoin(p,tacCreate(TAC_CALL_PARAM, aux->result?aux->result : 0, 0, 0, numParams - i));
      i++;
      aux = aux->prev;
    }
    if (right->type == AST_ARGS)
    {
      left = right->son[1];
      right = right->son[0];
    }
    else 
    {
      aux = aux->prev;
      if (sameType(right, aux))
      {
        p = tacJoin(p,tacCreate(TAC_CALL_PARAM, aux->result?aux->result : 0, 0, 0, numParams - i));
        i++;
      }
    }
    if(i == numParams)
    {
      done = 1;
    }
  }
  while(!done);
  return tacJoin(p, tacCreate(TAC_CALL, node->symbol, 0,0,(int)node->value));
}

TAC* makeBinOp(int op, TAC *result0, TAC *result1)
{
    TAC* opRes = tacCreate(op, makeTemp() , result0?result0->result:0, result1?result1->result:0,0);
    return tacJoin( tacJoin(result0, result1), opRes);

}


TAC* makeIf(TAC *result0, TAC *result1)
{
  TAC *newIfTac = 0;
  TAC *newLabelTac = 0;
  NODE *newLabel = 0;
  newLabel = makeLabel();
  newIfTac = tacCreate(TAC_IFZ, result0?result0->result:0, newLabel , 0,0);
  newLabelTac = tacCreate(TAC_LABEL, newLabel, 0, 0,0);
  return tacJoin(tacJoin( tacJoin(result0 , newIfTac), result1), newLabelTac);
}


TAC *makeIfElse(TAC *expr, TAC *cmdIf, TAC *cmdElse)
{
  NODE *labelElse = makeLabel();
  NODE *labelContinue = makeLabel();
  TAC *newIfElseTac = 0;
  TAC *lbContinue = 0;
  TAC *lbElse = 0;
  TAC *jmp = 0;
  newIfElseTac = tacCreate(TAC_IFZ_ELSE, expr?expr->result:0, labelElse, 0,0);
  lbContinue = tacCreate(TAC_LABEL, labelContinue, 0,0,0);
  lbElse = tacCreate(TAC_LABEL, labelElse, 0, 0,0);
  jmp = tacCreate(TAC_JUMP, labelContinue, 0, 0,0);
  return tacJoin(tacJoin( tacJoin(tacJoin(tacJoin(tacJoin(expr, newIfElseTac), cmdIf), jmp), lbElse), cmdElse ),lbContinue);
}



TAC* makeLeap(NODE *jmpLeapLoop)
{

  TAC *newLeapTac = 0;
  TAC *jump = 0;
  newLeapTac = tacCreate(TAC_LEAP,jmpLeapLoop, 0,0,0);
  jump = tacCreate(TAC_JUMP, jmpLeapLoop, 0, 0,0);
  return tacJoin(newLeapTac,jump);
}

TAC* makeLoop(TAC *expr, TAC *cmd, NODE *jmpLeapLoop )
{
  NODE *jmpFalse = makeLabel();
  NODE *jmpBegin = jmpLeapLoop;
  TAC *newLoopTac = 0;
  TAC *labelFalse = 0;
  TAC *labelBegin = 0;
  TAC *jump = 0;
  newLoopTac = tacCreate(TAC_LOOP, expr?expr->result:0, jmpFalse, jmpBegin,0);
  labelFalse = tacCreate(TAC_LABEL, jmpFalse, 0, 0,0);
  labelBegin = tacCreate(TAC_LABEL, jmpBegin, 0, 0,0);
  jump = tacCreate(TAC_JUMP, jmpBegin, 0, 0,0);
  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelBegin, expr), newLoopTac), cmd), jump), labelFalse);
}
