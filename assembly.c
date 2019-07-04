#include "assembly.h"

int asmGenerate(TAC* first, char* filename)
{
	FILE* fout;
	TAC* tac;

	if(!(fout = fopen(filename, "w")))
	{
		fprintf(stderr, "Cannot open file %s...\n", filename);
		exit(5);
	}
	
	fprintf(fout, "## START ASM GEN: FIXED ALWAYS\n"
			".section	__TEXT, __cstring,cstring_literals\n"
			"meuString: .asciz	\"%%d\" \n"
			".section	__DATA, __data\n\n");

	hashPrintAsm(fout);

	fprintf(fout, "## START ASM GEN: FIXED ALWAYS\n"
			".section __TEXT, __text, regular, pure_instructions\n\n");

	for(tac = first; tac; tac = tac->next)
		switch(tac->type)
		{
			case TAC_PRINT:	fprintf(fout, "## PRINT\n"
							"leaq	meuString(%%rip), %%rdi\n"
							"movl	_%s(%%rip), %%esi\n"
							"movb	$0, %%al\n"
							"callq	_printf\n\n", tac->firstOp?tac->firstOp->text:"");
				break;

			case TAC_BEGIN_FUN:	fprintf(fout, "## BEGIN FUNCTION\n"
								".globl	_main\n"
								"_main:\n"
								".cfi_startproc\n"
								"pushq	%%rbp\n"
								"subq	$16, %%rsp\n\n");
				break;
			case TAC_END_FUN:	fprintf(fout, "## END FUNCTION\n"
								"addq	$16, %%rsp\n"
								"popq	%%rbp\n"
								"retq\n"
								".cfi_endproc\n\n");
				break;
			case TAC_LABEL:		fprintf(fout, "%s:\n", tac->result?tac->result->text:"");
				break;
			case TAC_ADD:	fprintf(fout, "## ADD\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"addl %%edx, %%eax\n"
							"movl %%eax, _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;

			case TAC_SUB:	fprintf(fout, "## SUB\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"subl %%edx, %%eax\n"
							"movl %%eax, _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;

			case TAC_MUL:	fprintf(fout, "## MUL\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"imull %%edx, %%eax\n"
							"movl %%eax, _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;

			case TAC_DIV:	fprintf(fout, "## DIV\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"idivl %%edx, %%eax\n"
							"movl %%eax, _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;

			case TAC_ASSIGN:	fprintf(fout, "## ASSIGN\n"
							"movl _%s, %%edx\n"
							"movl _%%edx, _%%s\n\n", tac->firstOp->text, tac->result->text);
				break;

			case TAC_LESS:	fprintf(fout, "## LESS\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"cmpl %%edx, %%eax\n"
							"jge _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;

			case TAC_GREATER:	fprintf(fout, "## GREATER\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"cmpl %%edx, %%eax\n"
							"jle _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;
			case TAC_EQ:	fprintf(fout, "## GREATER\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"cmpl %%edx, %%eax\n"
							"jne _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;
			case TAC_LE:	fprintf(fout, "## LESSER EQUAL\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"cmpl %%edx, %%eax\n"
							"jg _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;
			case TAC_GE:	fprintf(fout, "## GREATER EQUAL\n"
							"movl _%s, %%edx\n"
							"movl _%s, %%eax\n"
							"cmpl %%edx, %%eax\n"
							"jl _%s\n\n", tac->firstOp->text, tac->secondOp->text, tac->result->text);
				break;
			case TAC_CALL:	fprintf(fout, "## FUNCTION CALL\n"
							"movl $0, %%eax\n"
							"call _%s\n\n", tac->result?tac->result->text:"");
				break;
		}
}

void hashPrintAsm(FILE * fout)
{
	int i;
	NODE *node;
	for(i=0;i<HASH_SIZE;++i)
	{
		for(node=Table[i];node;node=node->next)
		{
			fprintf(stderr, "%d %d\n", node->type, node->datatype);
			if(node->type == SYMBOL_IDENTIFIER || node->type == SYMBOL_LITINT)
					fprintf(fout, "_%s:\t.long\t_%s\n", node->text, node->text);
			else
				if (node->type == SYMBOL_LITSTRING)
				{
					fprintf(fout, "_%s:\t.string\t%s\n", node->text, node->text);
					
				}
					
//				else if (node->datatype == DATATYPE_FLOAT)
//					fprintf(fout, "_%s:\t.float\t%s\n", node->text, node->text);				
		}
	}
}
