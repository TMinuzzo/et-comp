%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	#include "ast.h"
	#include "hash.h"

	void yyerror(char const *s);
	int yylex();
	int getLineNumber();
%}

%code requires {
	#include "ast.h"
}


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

	type: KW_INT							{$$ = astCreate(AST_INT, 0, 0, 0, 0, 0);}
		| KW_FLOAT						{$$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0);}
		| KW_BYTE						{$$ = astCreate(AST_BYTE, 0, 0, 0, 0, 0);}
		;
	
	lit: LIT_FLOAT							{$$ = astCreate(AST_CONST, $1, 0, 0, 0, 0);}
	  	| LIT_INTEGER						{$$ = astCreate(AST_CONST, $1, 0, 0, 0, 0);}
	 	| LIT_CHAR						{$$ = astCreate(AST_CONST, $1, 0, 0, 0, 0);}
		;

	program: declist	 					{ astPrint($1, 0); if(out != NULL){ compile($1, out); } } 
		   ;

	declist: dec declist 						{ $$ = astCreate(AST_DEC_LIST, 0, $1, $2, 0, 0); } 
		   | 							{ $$ = 0; }
		   ;		

	dec: global_var_declaration 					{ $$ = $1; }
		| function						{ $$ = $1; }
		;	

	array: '[' LIT_INTEGER ']' array_opt_init			{ $$ = astCreate(AST_ARRAY, $2, $4, 0, 0, 0); }
		 ;

	array_opt_init: ':' value_array					{ $$ = astCreate(AST_ARRAY_INIT, 0, $2, 0, 0, 0); } 
		|							{ $$ = 0; }
		;

	value_array: lit value_array_opt				{ $$ = astCreate(AST_ARRAY_VALUE, 0, $1, $2, 0, 0); }
			   ;

	value_array_opt: lit value_array_opt 				{ $$ = astCreate(AST_ARRAY_VALUE, 0, $1, $2, 0, 0); } //rever
		|							{ $$ = 0; }
		;

	global_var_declaration: type TK_IDENTIFIER array ';'		{ $$ = astCreate(AST_GLOBAL_DEC, $2, $1, $3, 0, 0); } 
		| type TK_IDENTIFIER '=' lit ';'			{ $$ = astCreate(AST_GLOBAL_DEC_INIT, $2, $1, $4, 0, 0); }
		;

	function: header body						{ $$ = astCreate(AST_FUNCTION, 0, $1, $2, 0, 0); }
			;

	header: type TK_IDENTIFIER '(' params ')'			{ $$ = astCreate(AST_HEADER, $2, $1, $4, 0, 0);  }
		  ;

	body: block ';'							{ $$ = $1; }
		;

	params: param next_param 					{ $$ = astCreate(AST_PARAMS, 0, $1, $2, 0, 0); }
		|							{ $$ = 0; }
		;

	next_param: ',' param next_param				{ $$ = astCreate(AST_PARAMS, 0, $2, $3, 0, 0); }
		|							{ $$ = 0; }
		;

	param: type TK_IDENTIFIER					{ $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0); }
		 ;

	block: '{' cmds '}'						{ $$ = astCreate(AST_CMDS, 0, $2, 0, 0, 0); }
		 ;

	cmds: cmd cmds 							{$$ = astCreate(AST_CMDS, 0, $1, $2, 0, 0); }
		|							{ $$ = 0; }
		;

	cmd: 	cmd_return ';'						{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| assign ';' 						{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| cmd_print ';' 					{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| cmd_read ';' 						{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| func_call ';' 					{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| block ';'						{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| ctrl_flow ';'						{$$ = astCreate(AST_CMD, 0, $1, 0, 0, 0);}
		| ';'							{$$ = astCreate(AST_CMD, 0, 0, 0, 0, 0);}
		;

	assign: TK_IDENTIFIER '=' expr 					{$$ = astCreate(AST_ATTRIB, $1, $3, 0, 0, 0);}
		| array_pos '=' expr					{$$ = astCreate(AST_ARR_ATTRIB, 0, $1, $3, 0, 0);}
		;

	cmd_read: KW_READ TK_IDENTIFIER 				{$$ = astCreate(AST_READ, $2, 0, 0, 0, 0);}
			;

	cmd_print: KW_PRINT print_elements				{$$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0);}
			 ;

	print_elements: print_element print_elements_opt		{$$ = astCreate(AST_PRINT_ELEM, 0, $1, $2, 0, 0);}
				  ;

	print_elements_opt: ',' print_element print_elements_opt 	{$$ = astCreate(AST_PRINT_ELEM, 0, $2, $3, 0, 0);}
		|							{ $$ = 0; }
		;

	print_element: LIT_STRING 					{$$ = astCreate(AST_CONST, $1, 0, 0, 0, 0);}
		| expr							{ $$ = astCreate(AST_EXPRESSION, 0, $1, 0, 0, 0);}
		;

	cmd_return: KW_RETURN expr 					{$$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0);}
			  ;

	expr: rel_expr 							{ $$ = astCreate(AST_EXPRESSION, 0, $1, 0, 0, 0);}
	    ;

	rel_expr
		: log_expr 						{ $$ = astCreate(AST_EXPRESSION, 0, $1, 0, 0, 0);}
		| rel_expr OPERATOR_GE log_expr				{ $$ = astCreate(AST_GE, 0, $1, $3, 0, 0);}
		| rel_expr OPERATOR_LE log_expr				{ $$ = astCreate(AST_LE, 0, $1, $3, 0, 0);}
		| rel_expr OPERATOR_EQ log_expr				{ $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0);}
		| rel_expr OPERATOR_DIF log_expr			{ $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0);}
		| rel_expr '>' log_expr					{ $$ = astCreate(AST_GREATER, 0, $1, $3, 0, 0);}
		| rel_expr '<' log_expr					{ $$ = astCreate(AST_LESS, 0, $1, $3, 0, 0);}
		;
	log_expr
		: arit_expr						{ $$ = astCreate(AST_EXPRESSION, 0, $1, 0, 0, 0);}
		| log_expr OPERATOR_AND arit_expr			{ $$ = astCreate(AST_AND, 0, $1, $3, 0, 0);}
		| log_expr OPERATOR_OR arit_expr			{ $$ = astCreate(AST_OR, 0, $1, $3, 0, 0);}
		;

	arit_expr
		: un_expr 						{ $$ = astCreate(AST_EXPRESSION, 0, $1, 0, 0, 0);}
		| arit_expr '+' un_expr					{ $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0);}
		| arit_expr '-' un_expr					{ $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0);}		
		| arit_expr '/' un_expr					{ $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0);}
		| arit_expr '*' un_expr					{ $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0);}
		;

	
	un_expr: OPERATOR_NOT operand 					{ $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0);}
		| operand						{ $$ = astCreate(AST_OPERAND, 0, $1, 0, 0, 0);}
		;

	operand: TK_IDENTIFIER 						{ $$ = astCreate(AST_IDENTIFIER, $1, 0, 0, 0, 0);}
		| lit 							{ $$ = astCreate(AST_OPERAND, 0, $1, 0, 0, 0);}
		| array_pos 						{ $$ = astCreate(AST_OPERAND, 0, $1, 0, 0, 0);}
		| func_call 						{ $$ = astCreate(AST_OPERAND, 0, $1, 0, 0, 0);}
		| '(' expr ')'						{ $$ = astCreate(AST_OPERAND, 0, $2, 0, 0, 0);}
		;

	array_pos: TK_IDENTIFIER '[' arit_expr ']'			{$$ = astCreate(AST_ARR_POS, $1, $3, 0, 0, 0);}
			 ;
	
	func_call: TK_IDENTIFIER '(' args ')'				{$$ = astCreate(AST_FUNC_CALL, $1, $3, 0, 0, 0);}
			 ;

	args: expr next_arg 						{$$ = astCreate(AST_EXPRESSION, 0, $1, $2, 0, 0);}
		|							{ $$ = 0; }
		;

	next_arg: ',' expr next_arg 					{$$ = astCreate(AST_EXPRESSION, 0, $2, $3, 0, 0);}
		|							{$$ = 0;}
		;

	ctrl_flow: KW_IF '(' expr ')' KW_THEN then_opt			{$$ = astCreate(AST_IF, 0, $3, $6, 0, 0);}
		| KW_LOOP '(' expr ')' cmd				{$$ = astCreate(AST_LOOP, 0, $3, $5, 0, 0);}
		| KW_LEAP						{$$ = astCreate(AST_LEAP, 0, 0, 0, 0, 0);}
		;

	then_opt: cmd else_opt						{$$ = astCreate(AST_CMDS, 0, $1, 0, 0, 0);}
		| KW_ELSE cmd						{$$ = astCreate(AST_ELSE, 0, $2, 0, 0, 0);}
		;

	else_opt: KW_ELSE cmd 						{$$ = astCreate(AST_ELSE, 0, $2, 0, 0, 0);}
		|							{ $$ = 0; }
		;
 
%%


void yyerror (char const *s)
{
  fprintf (stderr, "%s , line: %d\n", s, getLineNumber());
  exit(3);
}
