all:y.tab.c y.tab.h lex.yy.c parser
y.tab.c y.tab.h : parser.y
	-yacc -d parser.y
lex.yy.c : parser.l
	-lex parser.l
parser : lex.yy.c y.tab.c y.tab.h
	-g++ lex.yy.c -ll y.tab.c -o parser
clean:
	-rm y.tab.c y.tab.h lex.yy.c parser
