#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "y.tab.h"

extern int running;
extern int lineNumber;
int yylex();
int yyparse();
extern char* yytext;
extern FILE *yyin;

int isRunning(void);
int getLineNumber(void);
void initMe(void);

int main (int argc, char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Call: ./a.out filename\n");
		exit(1);
	}
	if((yyin = fopen(argv [1], "r")) == 0)
	{
		fprintf(stderr, "Cannot open file\n");
		exit(2);
	}
	initMe();
	
	int result  = yyparse();
	if(result == 0)
	{
		fprintf(stderr, "Sucesso");
	}
	else
	{
		fprintf(stderr, "Código de erro %d", result );
	}
}
	
