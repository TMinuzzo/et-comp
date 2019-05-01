etapa3: lex.yy.o main.o hash.o y.tab.o ast.o
	@echo "\n Link parser"
	gcc -o etapa3 lex.yy.o main.o hash.o y.tab.o ast.o
ast.o: ast.c
	gcc -c ast.c
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c

lex.yy.o: parser.y scanner.l
	@echo "\n Compile parser"
	yacc -d  --verbose --debug parser.y
	lex --header-file=lex.yy.h scanner.l 
	gcc -c lex.yy.c y.tab.c

zip:
	tar cvzf etapa2.tgz Makefile main.c parser.y scanner.l hash.c hash.h ast.c ast.h

clean:
	rm -f *.o lex.yy.* y.tab.*  etapa3
