etapa5: lex.yy.o hash.o y.tab.o ast.o semantic.o tacs.o
	@echo "\n Link parser"
	gcc -o etapa5 lex.yy.o hash.o y.tab.o ast.o semantic.o tacs.o
ast.o: ast.c
	gcc -c ast.c
hash.o: hash.c
	gcc -c hash.c

lex.yy.o: parser.y scanner.l
	@echo "\n Compile parser"
	yacc -d  parser.y
	lex --header-file=lex.yy.h scanner.l 
	gcc -c lex.yy.c y.tab.c

semantic.o: semantic.c
	gcc -c semantic.c

tacs.o: tacs.c
	gcc -c tacs.c

zip:
	tar cvzf etapa5.tgz Makefile main.c parser.y scanner.l hash.c hash.h ast.c ast.h semantic.c semantic.h tacs.c tacs.h

clean:
	rm -f *.o lex.yy.* y.tab.* etapa5
