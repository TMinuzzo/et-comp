%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	void yyerror(char const *s);
	int yylex();
	int getLineNumber();
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

	arit_op: '+' | '-' | '*' | '/';
	rel_op: '<' | '>' | OPERATOR_LE | OPERATOR_GE | OPERATOR_EQ | OPERATOR_DIF;
	log_op: OPERATOR_OR | OPERATOR_AND;
	un_op: OPERATOR_NOT;

	program: declist;
	declist: dec declist | %empty;
	dec: global_var_declaration | function;

	array: '[' LIT_INTEGER ']' array_opt_init;
	array_opt_init: ':' value_array {fprintf(stderr, "\narray_opt_init value\n");}
				| %empty {fprintf(stderr, "\narray_opt_init empty\n");};
	value_array: lit value_array_opt {fprintf(stderr, "\nvalue_array\n");};
	value_array_opt: lit value_array_opt {fprintf(stderr, "\nvalue_array_opt lit\n");}
				| %empty {fprintf(stderr, "\nvalue_array_opt emty\n");};

	global_var_declaration: type TK_IDENTIFIER array ';' | type TK_IDENTIFIER '=' lit ';';
	function: header body;
	header: type TK_IDENTIFIER '(' params ')';
	body: block ';';
	params: param next_param | %empty;
	next_param: ',' param next_param | %empty;
	param: type TK_IDENTIFIER;

	block: '{' cmds '}';
	cmds: cmd cmds | %empty;
	cmd: cmd_return | assign ';' | cmd_print ';' | cmd_read ';' | func_call ';' | block ';'| ctrl_flow | ';';
	assign: TK_IDENTIFIER '=' expr | array_pos '=' expr;
	cmd_read: KW_READ TK_IDENTIFIER ;

	cmd_print: KW_PRINT print_elements ;
	print_elements: print_element print_elements_opt;
	print_elements_opt: ',' print_element print_elements_opt | %empty;
	print_element: LIT_STRING | expr;

	cmd_return: KW_RETURN expr ;
	expr: rel_expr ;

	rel_expr: log_expr | rel_expr rel_op log_expr;
	log_expr: arit_expr
			  | log_expr log_op arit_expr;
	arit_expr: operand 
			 | arit_expr arit_op un_expr;
	
	un_expr: un_op operand | operand;

	operand: TK_IDENTIFIER | lit | array_pos | func_call | '(' expr ')';
	array_pos: TK_IDENTIFIER '[' arit_expr ']';
	
	func_call: TK_IDENTIFIER '(' args ')';
	args: expr next_arg | %empty;
	next_arg: ',' expr next_arg | %empty;

	ctrl_flow: KW_IF '(' expr ')' KW_THEN then_opt
			   | KW_LOOP '(' expr ')' cmd
			   | KW_LEAP;
	then_opt: cmd else_opt| KW_ELSE cmd;
	else_opt: KW_ELSE cmd | %empty;
 
%%



void yyerror (char const *s)
{
  fprintf (stderr, "%s , line: %d\n", s, getLineNumber());
  exit(3);
}
