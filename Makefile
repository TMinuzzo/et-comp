etapa2: lex.yy.o main.o hash.o y.tab.o
	@echo "\n Link parser"
	gcc -o etapa2 lex.yy.o main.o hash.o y.tab.o
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c

lex.yy.o: parser.y scanner.l
	@echo "\n Compile parser"
	yacc -d parser.y
	lex scanner.l
	gcc -c lex.yy.c y.tab.c


clean:
	rm *.o lex.yy.* y.tab.*  etapa2
