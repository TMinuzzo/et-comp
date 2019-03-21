
%{
#include "tokens.h"
#include "hash.h"

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
"byte"				{ hashInsert(0, yytext); return KW_BYTE; }
"int"				{ hashInsert(0, yytext); return KW_INT; }
"float"				{ hashInsert(0, yytext); return KW_FLOAT; }
"if"				{ hashInsert(0, yytext); return KW_IF; }
"then"				{ hashInsert(0, yytext); return KW_THEN; }
"else"				{ hashInsert(0, yytext); return KW_ELSE; }
"loop"				{ hashInsert(0, yytext); return KW_LOOP; }
"leap"				{ hashInsert(0, yytext); return KW_LEAP; }
"or"				{ hashInsert(0, yytext); return OPERATOR_OR; }
"and"				{ hashInsert(0, yytext); return OPERATOR_AND; }
"not"				{ hashInsert(0, yytext); return OPERATOR_NOT; }
"read"				{ hashInsert(0, yytext); return KW_READ; }
"print"				{ hashInsert(0, yytext); return KW_PRINT; }
"return"			{ hashInsert(0, yytext); return KW_RETURN; }
"<="				{ hashInsert(0, yytext); return OPERATOR_LE; }
">="				{ hashInsert(0, yytext); return OPERATOR_GE; }
"=="				{ hashInsert(0, yytext); return OPERATOR_EQ; }
" "
"!="				{ hashInsert(0, yytext); return OPERATOR_DIF; }
{OPERATOR}			{ hashInsert(0, yytext); return yytext[0]; }
{IDENTIFIER}			{ hashInsert(0, yytext); return TK_IDENTIFIER; }
{INTEGER}			{ hashInsert(0, yytext); return LIT_INTEGER; }
{INTEGER}"."{INTEGER}		{ hashInsert(0, yytext); return LIT_FLOAT; }		
['].[']				{ hashInsert(0, yytext); return LIT_CHAR; }
{STRING}			{ hashInsert(0, yytext); return LIT_STRING; }
{LINE_COMMENT}				
{BLOCK_COMMENT}			{ BEGIN(BLOCK_COMMENT); }
<BLOCK_COMMENT>{BLOCK_COMMENT}	{ BEGIN(INITIAL); }
<BLOCK_COMMENT>.
<BLOCK_COMMENT>"\n"
.				{ hashInsert(0, yytext); return TOKEN_ERROR; }

%%

int yywrap()
{
	running = 0;
	return 1;
}

