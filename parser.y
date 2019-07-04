%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "lex.yy.h"
	#include "hash.h"
	#include "ast.h"
	#include "semantic.h"
	#include "tacs.h"

	FILE* out= NULL;
	void yyerror(char const *s);
	int yylex();
	int getLineNumber();
	extern int SemanticErr;
	TAC* tac;
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



%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ '<' '>'
%left OPERATOR_OR OPERATOR_AND
%left '+' '-'
%left '*' '/'
%left '(' ')' '[' ']'


%union {
	NODE *symbol;
	AST *ast;
}

%type<ast> type
%type<ast> lit
%type<ast> program
%type<ast> declist
%type<ast> dec_function
%type<ast> dec_var
%type<ast> array
%type<ast> array_opt_init
%type<ast> value_array
%type<ast> value_array_opt
%type<ast> value_ind
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
%type<ast> more_cmds
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

%start program
//%error-verbose


%nonassoc IFX
%nonassoc KW_ELSE


%%

	type: KW_INT							{$$ = astCreate(AST_INT, 0, 0, 0, 0, 0, getLineNumber());}
		| KW_FLOAT							{$$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0, getLineNumber());}
		| KW_BYTE							{$$ = astCreate(AST_BYTE, 0, 0, 0, 0, 0, getLineNumber());}
		;
	
	lit: LIT_FLOAT							{$$ = astCreate(AST_LIT_FLOAT, $1, 0, 0, 0, 0, getLineNumber());}
	  	| LIT_INTEGER						{$$ = astCreate(AST_LIT_INTEGER, $1, 0, 0, 0, 0, getLineNumber()); }
	 	| LIT_CHAR							{$$ = astCreate(AST_LIT_CHAR, $1, 0, 0, 0, 0, getLineNumber());}
		;

	program: declist	 					
			{	//astPrint($1, 0);
				setDeclaration($1);
				checkUndeclared();
				//checkOperands($1);
				tac = tacGenerate($1,0);
				tacPrintForward(tac);
				//if(out != NULL && SemanticErr != 1){ compile($1, out) ; }
			} 
		   ;

	declist: 
		    dec_var declist				{ $$ = astCreate(AST_DEC_VAR_LIST, 0, $1, $2, 0, 0, getLineNumber()); }
			|	header declist 			{ $$ = astCreate(AST_DEC_FUNC_LIST, 0, $1, $2, 0, 0, getLineNumber()); } 
		   | 								{ $$ = 0; }
		   ;		
	dec_function:  function						{ $$ = $1; }
				;

	dec_var: global_var_declaration 			{ $$ = $1; }
		;	

	array: '[' value_ind ']' array_opt_init			{ $$ = astCreate(AST_ARRAY, 0, $2, $4, 0, 0, getLineNumber()); }
		 ;

	value_ind: LIT_INTEGER                          { $$ = astCreate(AST_LIT_INTEGER, $1, 0, 0, 0, 0, getLineNumber()); }	
            ;

	array_opt_init: ':' value_array						{ $$ = $2; } 
		|												{ $$ = 0; }
		;

	value_array: lit value_array_opt					{ $$ = astCreate(AST_ARRAY_VALUE, 0, $1, $2, 0, 0, getLineNumber()); }
			   ;

	value_array_opt: lit value_array_opt 				{ $$ = astCreate(AST_ARRAY_VALUE, 0, $1, $2, 0, 0, getLineNumber()); }
		|												{ $$ = 0; }
		;

	global_var_declaration: type TK_IDENTIFIER array ';'		{ $$ = astCreate(AST_GLOBAL_DEC, $2, $1, $3, 0, 0, getLineNumber()); } 
		| type TK_IDENTIFIER '=' lit ';'						{ $$ = astCreate(AST_GLOBAL_DEC_INIT, $2, $1, $4, 0, 0, getLineNumber()); }
		;

	function: header body										{ $$ = astCreate(AST_FUNCTION, 0, $1, $2, 0, 0, getLineNumber()); }
			;

	header: type TK_IDENTIFIER '(' params ')' body					{ $$ = astCreate(AST_HEADER, $2, $1, $4, $6, 0, getLineNumber());  }
		  ;

	body: block ';'												{ $$ = $1; }
		;

	params: param next_param 									{ $$ = astCreate(AST_PARAMS, 0, $1, $2, 0, 0, getLineNumber()); }
		|														{ $$ = 0; }
		;

	next_param: ',' param next_param							{ $$ = astCreate(AST_PARAMS, 0, $2, $3, 0, 0, getLineNumber()); }
		|														{ $$ = 0; }
		;

	param: type TK_IDENTIFIER									{ $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0, getLineNumber()); }
		 ;

	block: '{' cmds '}'											{ $$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0, getLineNumber()); }
		 ;

	cmds: cmd more_cmds 										{ $$ = astCreate(AST_CMDS, 0, $1, $2, 0, 0, getLineNumber()); }
		|														{ $$ = 0; }
		;

	more_cmds: ';' cmd more_cmds								{ $$ = astCreate(AST_CMDS, 0, $2, $3, 0, 0, getLineNumber()); }
			 |													{ $$ = 0; }
			 ;
	
	cmd: 	cmd_return 										{ $$ = $1; }
		| block 											{ $$ = $1; }
		| assign  											{ $$ = $1; }
		| cmd_print  										{ $$ = $1; }
		| cmd_read  										{ $$ = $1; }
		| func_call  										{ $$ = $1; }
		| ctrl_flow 										{ $$ = $1; }
		| 													{ $$ = 0; }
		;

	assign: TK_IDENTIFIER '=' expr 								{$$ = astCreate(AST_ATTRIB, $1, $3, 0, 0, 0, getLineNumber());}
		| array_pos '=' expr									{$$ = astCreate(AST_ARR_ATTRIB, 0, $1, $3, 0, 0, getLineNumber());}
		;

	cmd_read: KW_READ TK_IDENTIFIER 							{$$ = astCreate(AST_READ, $2, 0, 0, 0, 0, getLineNumber());}
			;

	cmd_print: KW_PRINT print_elements							{$$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0, getLineNumber());}
			 ;

	print_elements: print_element print_elements_opt			{$$ = astCreate(AST_PRINT_ELEM, 0, $1, $2, 0, 0, getLineNumber());}
				  ;

	print_elements_opt: ',' print_element print_elements_opt 	{$$ = astCreate(AST_PRINT_ELEM, 0, $2, $3, 0, 0, getLineNumber());}
		|														{ $$ = 0; }
		;

	print_element: LIT_STRING 									{$$ = astCreate(AST_LIT_STRING, $1, 0, 0, 0, 0, getLineNumber());}
		| expr													{ $$ = $1;}
		;

	cmd_return: KW_RETURN expr 									{$$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0, getLineNumber());}
			  ;

	expr: rel_expr 												{ $$ = $1; }
	    ;

	rel_expr
		: log_expr 									{ $$ = $1 ;}
		| rel_expr OPERATOR_GE log_expr				{ $$ = astCreate(AST_GE, 0, $1, $3, 0, 0, getLineNumber());}
		| rel_expr OPERATOR_LE log_expr				{ $$ = astCreate(AST_LE, 0, $1, $3, 0, 0, getLineNumber());}
		| rel_expr OPERATOR_EQ log_expr				{ $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0, getLineNumber());}
		| rel_expr OPERATOR_DIF log_expr			{ $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0, getLineNumber());}
		| rel_expr '>' log_expr					{ $$ = astCreate(AST_GREATER, 0, $1, $3, 0, 0, getLineNumber());}
		| rel_expr '<' log_expr					{ $$ = astCreate(AST_LESS, 0, $1, $3, 0, 0, getLineNumber());}
		;
	log_expr
		: arit_expr									{ $$ = $1;}
		| log_expr OPERATOR_AND arit_expr			{ $$ = astCreate(AST_AND, 0, $1, $3, 0, 0, getLineNumber());}
		| log_expr OPERATOR_OR arit_expr			{ $$ = astCreate(AST_OR, 0, $1, $3, 0, 0, getLineNumber());}
		;

	arit_expr
		: un_expr 								{ $$ = $1;}
		| arit_expr '+' un_expr					{ $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0, getLineNumber());}
		| arit_expr '-' un_expr					{ $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0, getLineNumber());}		
		| arit_expr '/' un_expr					{ $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0, getLineNumber());}
		| arit_expr '*' un_expr					{ $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0, getLineNumber());}
		;

	
	un_expr: OPERATOR_NOT operand 					{ $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0, getLineNumber());}
		| operand									{ $$ = $1;}
		;

	operand: TK_IDENTIFIER 					{ $$ = astCreate(AST_IDENTIFIER, $1, 0, 0, 0, 0, getLineNumber());}
		| lit 								{ $$ = $1;}
		| array_pos 						{ $$ = $1;}
		| func_call 						{ $$ = $1;}
		| '(' expr ')'						{ $$ = $2;}
		;

	array_pos: TK_IDENTIFIER '[' arit_expr ']'			{$$ = astCreate(AST_ARR_POS, $1, $3, 0, 0, 0, getLineNumber());}
			 ;
	
	func_call: TK_IDENTIFIER '(' args ')'				{$$ = astCreate(AST_FUNC_CALL, $1, $3, 0, 0, 0, getLineNumber());}
			 ;

	args: expr next_arg 						{$$ = astCreate(AST_ARGS, 0, $1, $2, 0, 0, getLineNumber());}
		|							{ $$ = 0; }
		;

	next_arg: ',' expr next_arg 					{$$ = astCreate(AST_ARGS, 0, $2, $3, 0, 0, getLineNumber());}
		|							{$$ = 0;}
		;

	ctrl_flow
			: KW_IF '(' expr ')' KW_THEN cmd %prec IFX		              { $$ = astCreate(AST_IF, 0, $3, $6, 0, 0, getLineNumber());}
            | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd                  { $$ = astCreate(AST_IF_ELSE, 0, $3, $6, $8, 0, getLineNumber());}
			| KW_LOOP '(' expr ')' cmd									  { $$ = astCreate(AST_LOOP, 0, $3, $5, 0, 0, getLineNumber()); }
			| KW_LEAP 													  { $$ = astCreate(AST_LEAP, 0, 0, 0, 0, 0, getLineNumber()); }
			;
 
%%

void defineOut(FILE *o){
    out = o;
}

void yyerror (char const *s)
{
  fprintf (stderr, "%s , line: %d\n", s, getLineNumber());
  exit(3);
}
