
%{
#include "hash.h"
#include "ast.h"
#include "y.tab.h"
#include "main.c"


int lineNumber = 1;
int running = 1; 

%}

INTEGER [OIHGFEDCBA]+
IDENTIFIER ([a-z0-9]|_)+
STRING \"(\\.|[^\\"])*\"
OPERATOR ","|";"|":"|"("|")"|"["|"]"|"{"|"}"|"+"|"-"|"*"|"/"|"<"|">"|"="|"&"|"$"|"#"
LINE_COMMENT "ZZ".*
BLOCK_COMMENT "XXX"

%x BLOCK_COMMENT 

%%
"byte"				{ return KW_BYTE; }
"int"				{ return KW_INT; }
"float"				{ return KW_FLOAT; }
"if"				{ return KW_IF; }
"then"				{ return KW_THEN; }
"else"				{ return KW_ELSE; }
"loop"				{ return KW_LOOP; }
"leap"				{ return KW_LEAP; }
"or"				{ return OPERATOR_OR; }
"and"				{ return OPERATOR_AND; }
"not"				{ return OPERATOR_NOT; }
"read"				{ return KW_READ; }
"print"				{ return KW_PRINT; }
"return"			{ return KW_RETURN; }
"<="				{ return OPERATOR_LE; }
">="				{ return OPERATOR_GE; }
"=="				{ return OPERATOR_EQ; }
"!="				{ return OPERATOR_DIF; }
" "
"\t"
"\n"					{ lineNumber++; }
{OPERATOR}				{ return yytext[0]; }
{IDENTIFIER}			{ yylval.symbol = hashInsert(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }
{INTEGER}				{ yylval.symbol = hashInsert(SYMBOL_LITINT, yytext); return LIT_INTEGER; }
{INTEGER}"."{INTEGER}	{ yylval.symbol = hashInsert(SYMBOL_LITFLOAT, yytext); return LIT_FLOAT; }		
['].[']					{ yylval.symbol = hashInsert(SYMBOL_LITCHAR, yytext); return LIT_CHAR; }
{STRING}				{ yylval.symbol = hashInsert(SYMBOL_LITSTRING, yytext); return LIT_STRING; }
{LINE_COMMENT}				
{BLOCK_COMMENT}					{ BEGIN(BLOCK_COMMENT); }
<BLOCK_COMMENT>{BLOCK_COMMENT}	{ BEGIN(INITIAL); }
<BLOCK_COMMENT>.
<BLOCK_COMMENT>"\n"				{ lineNumber++; }
.								{ return TOKEN_ERROR; }

%%

int yywrap()
{
	running = 0;
	return 1;
}

int getLineNumber(void)
{
	return lineNumber;
}

int isRunning(void)
{
	return running;
}

void initMe(void)
{
	hashInit();
}



