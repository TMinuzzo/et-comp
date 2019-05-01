%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	#include "ast.h"

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
%token <symbol>TK_IDENTIFIER 
%token <symbol>LIT_INTEGER   
%token <symbol>LIT_FLOAT     
%token <symbol>LIT_CHAR      
%token <symbol>LIT_STRING    
%token TOKEN_ERROR   



%left '<' '>' '='
%left '+' '-'
%left '*' '/'

%union {
	NODE *symbol;
    AST *ast;
}

%type<ast> type
%type<ast> lit
%type<ast> arit_op
%type<ast> rel_op
%type<ast> log_op
%type<ast> un_op
%type<ast> program
%type<ast> declist
%type<ast> dec
%type<ast> array
%type<ast> array_opt_init
%type<ast> value_array
%type<ast> value_array_opt
%type<ast> global_var_declaration
%type<ast> function
%type<ast> header
%type<ast> body
%type<ast> params
%type<ast> next_param
%type<ast> param
%type<ast> block
%type<ast> cmds
%type<ast> cmd
%type<ast> assign
%type<ast> cmd_read
%type<ast> cmd_print
%type<ast> print_elements
%type<ast> print_elements_opt
%type<ast> print_element
%type<ast> cmd_return
%type<ast> expr
%type<ast> rel_expr
%type<ast> log_expr
%type<ast> arit_expr
%type<ast> un_expr
%type<ast> operand
%type<ast> array_pos
%type<ast> func_call
%type<ast> args
%type<ast> next_arg
%type<ast> ctrl_flow
%type<ast> then_opt
%type<ast> else_opt

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

	program: declist	 						{ astPrint($1, 0); if(out != NULL){ compile($1, out); } } 
		   ;

	declist: dec declist 						{ $$ = astCreate(AST_DEC_LIST, 0, $1, $2, 0, 0); } 
		   | 									{ $$ = 0; }
		   ;		

	dec: global_var_declaration 				{ $$ = $1; }
		| function								{ $$ = $1; }
		;	

	array: '[' LIT_INTEGER ']' array_opt_init	{ $$ = astCreate(AST_ARRAY, $2, $4, 0, 0, 0); }
		 ;

	array_opt_init: ':' value_array				{ $$ = astCreate(AST_ARRAY_INIT, 0, $2, 0, 0, 0); } 
		|										{ $$ = 0; }
		;

	value_array: lit value_array_opt			{ $$ = astCreate(AST_ARRAY_VALUE, 0, $1, $2, 0, 0); }
			   ;

	value_array_opt: lit value_array_opt 		{ $$ = astCreate(AST_ARRAY_VALUE, 0, $1, $2, 0, 0); } //rever
		|										{ $$ = 0; }
		;

	global_var_declaration: type TK_IDENTIFIER array ';'		{ $$ = astCreate(AST_GLOBAL_DEC, $2, $1, $3, 0, 0); } 
		| type TK_IDENTIFIER '=' lit ';'						{ $$ = astCreate(AST_GLOBAL_DEC_INIT, $2, $1, $4, 0, 0); }
		;

	function: header body										{ $$ = astCreate(AST_FUNCTION, 0, $1, $2, 0, 0); }
			;

	header: type TK_IDENTIFIER '(' params ')'					{ $$ = astCreate(AST_HEADER_, $2, $1, $4, 0, 0);  }
		  ;

	body: block ';'												{ $$ = $1; }
		;

	params: param next_param 									{ $$ = astCreate(AST_PARAMS, 0, $1, $2, 0, 0); }
		|														{ $$ = 0; }
		;

	next_param: ',' param next_param							{ $$ = astCreate(AST_PARAMS, 0, $2, $3, 0, 0); }
		|														{ $$ = 0; }
		;

	param: type TK_IDENTIFIER									{ $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0); }
		 ;

	block: '{' cmds '}'											{ $$ = astCreate(AST_CMDS, 0, $2, 0, 0, 0); }
		 ;

	cmds: cmd cmds 												{}
		|														{ $$ = 0; }
		;

	cmd: cmd_return 											{}
		| assign ';' 											{}
		| cmd_print ';' 										{}
		| cmd_read ';' 											{}
		| func_call ';' 										{}
		| block ';'												{}
		| ctrl_flow 											{}
		| ';'													{}
		;

	assign: TK_IDENTIFIER '=' expr 								{}
		| array_pos '=' expr									{}
		;

	cmd_read: KW_READ TK_IDENTIFIER 							{}
			;

	cmd_print: KW_PRINT print_elements							{}
			 ;

	print_elements: print_element print_elements_opt			{}
				  ;

	print_elements_opt: ',' print_element print_elements_opt 	{}
		|														{ $$ = 0; }
		;

	print_element: LIT_STRING 									{}
		| expr													{}
		;

	cmd_return: KW_RETURN expr 									{}
			  ;

	expr: rel_expr 												{}
	    ;

	rel_expr: log_expr 											{}
		| rel_expr rel_op log_expr								{}
		;

	log_expr: arit_expr											{}
		| log_expr log_op arit_expr								{}
		;

	arit_expr: operand 											{}
		| arit_expr arit_op un_expr								{}
		;

	program
		: declist;
	declist
		: dec declist
		|;
	dec
		: global_var_declaration 
		| function;

	array
		: '[' LIT_INTEGER ']' array_opt_init;
	array_opt_init
		: ':' value_array 
		|;
	value_array
		: lit value_array_opt;
	value_array_opt
		: lit value_array_opt 
		|;

	global_var_declaration
		: type TK_IDENTIFIER array ';' 
		| type TK_IDENTIFIER '=' lit ';';
	function
		: header body;
	header
		: type TK_IDENTIFIER '(' params ')';
	body
		: block ';';
	params
		: param next_param 
		|;
	next_param
		: ',' param next_param 
		|;
	param
		: type TK_IDENTIFIER;

	block
		: '{' cmds '}';
	cmds
		: cmd cmds 
		|;
	cmd
		: cmd_return 
		| assign ';' 
		| cmd_print ';' 
		| cmd_read ';' 
		| func_call ';' 
		| block ';'
		| ctrl_flow 
		| ';';

	assign
		: TK_IDENTIFIER '=' expr 
		| array_pos '=' expr;
	cmd_read
		: KW_READ TK_IDENTIFIER ;

	cmd_print
		: KW_PRINT print_elements ;
	print_elements
		: print_element print_elements_opt;
	print_elements_opt
		: ',' print_element print_elements_opt 
		|;
	print_element
		: LIT_STRING 
		| expr;

	cmd_return
		: KW_RETURN expr ;
	expr
		: rel_expr ;

	rel_expr
		: log_expr 
		| rel_expr rel_op log_expr;
	log_expr
		: arit_expr
		| log_expr log_op arit_expr;
	arit_expr
		: operand 
		| arit_expr arit_op un_expr;
	
	un_expr: un_op operand 										{}
		| operand												{}
		;

	operand: TK_IDENTIFIER 										{}
		| lit 													{}
		| array_pos 											{}
		| func_call 											{}
		| '(' expr ')'											{}
		;

	array_pos: TK_IDENTIFIER '[' arit_expr ']'					{}
			 ;
	
	func_call: TK_IDENTIFIER '(' args ')'						{}
			 ;

	args: expr next_arg 										{}
		|														{ $$ = 0; }
		;

	next_arg: ',' expr next_arg 								{}
		|														{}
		;

	ctrl_flow: KW_IF '(' expr ')' KW_THEN then_opt				{}
		| KW_LOOP '(' expr ')' cmd								{}
		| KW_LEAP												{}
		;

	then_opt: cmd else_opt										{}
		| KW_ELSE cmd											{}
		;

	else_opt: KW_ELSE cmd 										{}
			|													{ $$ = 0; }
			;
	func_call
		: TK_IDENTIFIER '(' args ')';
	args
		: expr next_arg 
		|;
	next_arg
		: ',' expr next_arg 
		|;

	ctrl_flow
		: KW_IF '(' expr ')' KW_THEN then_opt
		| KW_LOOP '(' expr ')' cmd
		| KW_LEAP;

	then_opt
		: cmd else_opt
		| KW_ELSE cmd;
	else_opt
	: KW_ELSE cmd 
	|;
 
%%


void yyerror (char const *s)
{
  fprintf (stderr, "%s , line: %d\n", s, getLineNumber());
  exit(3);
}
