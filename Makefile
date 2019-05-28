etapa4: lex.yy.o main.o hash.o y.tab.o ast.o semantic.o
	@echo "\n Link parser"
	gcc -o etapa4 lex.yy.o main.o hash.o y.tab.o ast.o semantic.o
ast.o: ast.c
	gcc -c ast.c
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c

lex.yy.o: parser.y scanner.l
	@echo "\n Compile parser"
	yacc -d  parser.y
	lex --header-file=lex.yy.h scanner.l 
	gcc -c lex.yy.c y.tab.c

semantic.o: semantic.c
	gcc -c semantic.c

zip:
	tar cvzf etapa4.tgz Makefile main.c parser.y scanner.l hash.c hash.h ast.c ast.h semantic.c semantic.h

clean:
	rm -f *.o lex.yy.* y.tab.*  etapa4
