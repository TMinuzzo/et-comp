
%{
#include "tokens.h"
int lineNumber = 1;
int running = 1; 
%}


%%
"if"		{ return KW_IF; }
"then"		{ return KW_THEN; }
"else"		{ return KW_ELSE; }
"while"		{ return KW_WHILE; } 
"read"		{ return KW_READ; }
"return"	{ return KW_RETURN; }
"print"		{ return KW_PRINT; }

%%

int yywrap()
{
	running = 0;
	return 1;
}
