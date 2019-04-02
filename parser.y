%{
	#include <stdio.h>
	#include "hash.h"
	int yyerror();
	int yylex();
%}

%token KW_IF

%left '<' '>' '='
%left '+' '-'
%left '*' '/'

%%

	program:
	       ;


%%

int yyerror()
{
	fprintf(stderr, "Syntax error");
	exit(3);
}
