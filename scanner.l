
%{
#include "tokens.h"

int lineNumber = 1;
int running = 1; 

%}

INTEGER [OIHGFEDCBA]+
IDENTIFIER ([a-z0-9]|_)+
STRING \"(\\.|[^\\"])*\"

%x COMMENT 

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
{IDENTIFIER}			{ return TK_IDENTIFIER; }
{INTEGER}			{ return LIT_INTEGER; }
{INTEGER}"."{INTEGER}		{ return LIT_FLOAT; }		
['].[']				{ return LIT_CHAR; }
{STRING}			{ return LIT_STRING; }
"ZZ".*				
"XXX"				{ BEGIN(COMMENT); }
<COMMENT>"XXX"			{ BEGIN(INITIAL); }
<COMMENT>.
<COMMENT>"\n"
.				{ return TOKEN_ERROR; }

%%

int yywrap()
{
	running = 0;
	return 1;
}

