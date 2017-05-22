all:clean y.tab.c y.tab.h lex.yy.c parser
y.tab.c y.tab.h : parser.y
	-yacc -d parser.y
lex.yy.c : parser.l
	-lex parser.l
parser : lex.yy.c y.tab.c y.tab.h cfg.c cfg.h point_analysis.c point_analysis.h clone_build_graph.c clone_build_graph.h first_analysis.c first_analysis.h second_analysis.c second_analysis.h
	-g++ -std=c++11 -w lex.yy.c -ll y.tab.c cfg.c point_analysis.c clone_build_graph.c first_analysis.c second_analysis.c -o parser -g
clean:
	-rm y.tab.c y.tab.h lex.yy.c parser
