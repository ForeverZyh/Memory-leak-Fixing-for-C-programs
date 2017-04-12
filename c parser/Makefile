all:y.tab.c y.tab.h lex.yy.c parser
y.tab.c y.tab.h : parser.y
	-yacc -d parser.y
lex.yy.c : parser.l
	-lex parser.l
parser : lex.yy.c y.tab.c y.tab.h cfg.c cfg.h
	-g++ lex.yy.c -ll y.tab.c cfg.c -o parser
clean:
	-rm y.tab.c y.tab.h lex.yy.c parser
