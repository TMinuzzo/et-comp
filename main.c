
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "y.tab.h"


extern FILE *yyin;
extern int SemanticErr;
extern void initMe(void);
extern void defineOut(FILE*);

int main(int argc, char** argv)
{
  initMe();
	FILE* output;
  if (argc < 3)
  {
    printf("Erro: ./etapa4 <input> <output>\n");
    exit(1);
  }
  if (0==(yyin = fopen(argv[1],"r")))
  {
    printf("Não foi possível abrir arquivo %s\n",argv[1]);
    exit(2);
  }
  if (0==(output = fopen(argv[2],"w")))
  {
    printf("Não foi possível criar arquivo %s\n",argv[2]);
    exit(2);
  }
  defineOut(output);
  yyparse();
  if (SemanticErr == 1)
  {
    exit(4);
  }
  printf(" \nSucesso! :D - \n Compilation end.\n");
  return 0;
}
