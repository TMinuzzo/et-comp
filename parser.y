%{
	#include <stdio.h>
	#include <stdlib.h>
	//#include "hash.h"
	void yyerror(char const *s);
	int yylex();
%}

%token KW_BYTE       
%token KW_INT        
%token KW_FLOAT      
%token KW_IF         
%token KW_THEN       
%token KW_ELSE       
%token KW_LOOP       
%token KW_LEAP       
%token KW_READ       
%token KW_RETURN     
%token KW_PRINT      
%token OPERATOR_LE   
%token OPERATOR_GE   
%token OPERATOR_EQ   
%token OPERATOR_DIF  
%token OPERATOR_OR   
%token OPERATOR_AND  
%token OPERATOR_NOT  
%token TK_IDENTIFIER 
%token LIT_INTEGER   
%token LIT_FLOAT     
%token LIT_CHAR      
%token LIT_STRING    
%token TOKEN_ERROR   



%left '<' '>' '='
%left '+' '-'
%left '*' '/'

%start program
%error-verbose

%%

	type: KW_INT
		| KW_FLOAT
		| KW_BYTE;
	
	lit: LIT_FLOAT
	   | LIT_INTEGER
	   | LIT_CHAR;
	
	program: global_var_declaration | %empty ;
	//declist: dec declist ;
	//dec: type TK_IDENTIFIER | type TK_IDENTIFIER '(' params ')' cmd
	array: '[' LIT_INTEGER ']' array_init;
	array_init: ':' value_array | %empty;
	array_optional: array | %empty;
	value_array: lit value_array | %empty;

	global_var_declaration: global_array | global_var ;
	global_array: type TK_IDENTIFIER array ';'; 
	global_var: type TK_IDENTIFIER '=' lit ';' { fprintf(stderr, "global"); } ;

	function: header body;
	header: type TK_IDENTIFIER '(' params ')';
	body: block ;
	params: param comma;
	comma: ',' param comma | %empty;
	param: type TK_IDENTIFIER;

	//cmd: assign | flux_ctrl | %empty;   
	block: '{'  '}';
	//cmds: cmd ';' cmds | %empty ;

	//assign: TK_IDENTIFIER '=' expr | TK_IDENTIFIER '[' expr ']' '=' expr;
	//flux_ctrl: ;
	//expr:
	
 
%%



void yyerror (char const *s)
{

  fprintf (stderr, "%s\n", s);
}
