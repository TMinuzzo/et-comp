#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

extern int running;
extern int lineNumber;
int yylex();
extern FILE *yyin;


//int lineNumber = 1;
//int running = 1;

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
	int tok = 0;
	while (running)
	{
		tok = yylex();
		if(!running) break;
		switch(tok)
		{
			case KW_IF: fprintf(stdout, "KW_IF"); break;
			default: fprintf(stdout, "UNKNOWN"); break;
		}
	}
}
	
