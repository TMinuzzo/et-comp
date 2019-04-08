%{
	#include <stdio.h>
	#include <stdlib.h>
	//#include "hash.h"
	void yyerror(char const *s);
	int yylex();
	extern int lineNumber;
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
	type	: KW_BYTE
		| KW_FLOAT
		| KW_INT
		;
	lit	: LIT_FLOAT
		| LIT_CHAR
		| LIT_INTEGER
		;

	program	: decList
		|
		;

	decList : globalVarDeclaration decList
		| globalVecDeclaration decList
		| function decList
		|
		;

	globalVarDeclaration	: type TK_IDENTIFIER '=' lit ';'
				;
	globalVecDeclaration	: type TK_IDENTIFIER '[' LIT_INTEGER ']' vecInit ';'
				;
	vecInit		: ':' lit restVecInit 
			| 
			;
	restVecInit	: lit restVecInit
			|
			; 

	function	: type TK_IDENTIFIER '(' funcParamList ')' cmdBlock
			;
	funcParamList	: type TK_IDENTIFIER restParamList
			|
			;
	restParamList	: ',' type TK_IDENTIFIER restParamList
			|
			;

	cmdBlock	: '{' cmds '}' ';'
			| simpleCmd
			;
	cmds		: simpleCmd ';' cmds
			| cmdBlock cmds
			|
			;
	simpleCmd	: cmdPrint
			| cmdRead
			| cmdReturn
			| cmdAttrib
			| cmdCtrlFlow
			|	
			;
	
	cmdCtrlFlow	: KW_IF aritExpression KW_THEN cmdBlock
			| KW_IF aritExpression KW_THEN cmdBlock KW_ELSE cmdBlock
			| KW_LOOP aritExpression cmdBlock
			| KW_LEAP
			;

	cmdReturn	: KW_RETURN aritExpression
			;
	cmdRead		: KW_READ TK_IDENTIFIER
			;
	cmdPrint	: KW_PRINT printElements
			;
	cmdAttrib	: TK_IDENTIFIER vector '=' aritExpression
			;
	vector		: '[' aritExpression ']'
			|
			;
	printElements	: element restElements
			;
	restElements	: ',' element restElements
			|
			;
 	element		: LIT_STRING
			| aritExpression
			;
	aritExpression	: TK_IDENTIFIER
			| TK_IDENTIFIER '[' aritExpression ']'
			| funcCall
			| aritExpression operator aritExpression
			| '(' aritExpression ')'
			| LIT_INTEGER
			| LIT_CHAR
			| LIT_FLOAT
			;
	funcCall	: TK_IDENTIFIER '(' funcCallParam ')'
			;
	funcCallParam	: TK_IDENTIFIER restFuncCallParam
			| LIT_INTEGER restFuncCallParam
			| LIT_CHAR restFuncCallParam
			| LIT_FLOAT restFuncCallParam
			;
	restFuncCallParam	: ',' TK_IDENTIFIER restFuncCallParam
			| ',' LIT_INTEGER restFuncCallParam
			| ',' LIT_CHAR restFuncCallParam
			| ',' LIT_FLOAT restFuncCallParam
			|
			;
	operator	: OPERATOR_LE   
			| OPERATOR_GE   
			| OPERATOR_EQ   
			| OPERATOR_DIF  
			| OPERATOR_OR   
			| OPERATOR_AND  
			| OPERATOR_NOT  
			| '+'
			| '-'
			| '*'
			| '/'
			| '<'
			| '>'
			;
	
%%



void yyerror (char const *s)
{

  fprintf (stderr, "%s ", s);
  fprintf (stderr, "on line %d\n", lineNumber);
}
