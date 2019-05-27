#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "y.tab.h"
#include "semantic.h"


extern int running;
extern int lineNumber;
int yylex();
int yyparse();
int yydebug = 1;
extern char *yytext;
extern FILE *yyin;
extern void defineOut(FILE*);

int isRunning(void);
int getLineNumber(void);
void initMe(void);

int main(int argc, char **argv)
{
	int semantic;
	FILE *output;
	if (argc < 3)
	{
		fprintf(stderr, "Call: ./etapa3 input output\n");
		exit(1);
	}
	if ((yyin = fopen(argv[1], "r")) == 0)
	{
		fprintf(stderr, "Cannot open file\n");
		exit(2);
	}
	initMe();

	if (0 == (output = fopen(argv[2], "w")))
	{
		printf("Cannot create file %s... \n", argv[2]);
		exit(1);
	}
	defineOut(output);

	int result = yyparse();
	if (result == 0)
	{
		fprintf(stderr, "Sucesso\n");
		//exit(0);
	}
	semantic = setAndCheckRedeclared(root);
	hashPrint();
	fprintf(stderr,"semantic result = %d\n", semantic);
}
