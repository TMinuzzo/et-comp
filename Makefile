etapa2: lex.yy.o main.o hash.o y.tab.o
	gcc -o etapa2 lex.yy.o main.o hash.o y.tab.o
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c
y.tab.c: parser.y
	yacc parser.y
y.tab.o: y.tab.c
	gcc -c y.tab.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
lex.yy.c: scanner.l
	lex scanner.l
clean:
	rm *.o lex.yy.c  etapa2
